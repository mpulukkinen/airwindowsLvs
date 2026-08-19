#include "LegacyAirwindowsVst3.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "vstgui/plugin-bindings/vst3editor.h"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace Steinberg::LVS {

namespace {
double clamp01(double value) noexcept {
    return std::clamp(value, 0.0, 1.0);
}
}

float mapDirect(double value) noexcept {
    return static_cast<float>(clamp01(value));
}

float mapInverse(double value) noexcept {
    return static_cast<float>(1.0 - clamp01(value));
}

float mapGain0To18(double value) noexcept {
    return static_cast<float>(std::pow(10.0, (clamp01(value) * 18.0) / 20.0));
}

float mapGainMinus12To0(double value) noexcept {
    return static_cast<float>(std::pow(10.0, (-12.0 + (clamp01(value) * 12.0)) / 20.0));
}

LegacyEffectProcessor::LegacyEffectProcessor(const LegacyPluginSpec& spec)
    : spec_(spec), values_(spec.parameterCount) {
    setControllerClass(*spec.controllerUid);
    for (std::size_t i = 0; i < spec_.parameterCount; ++i)
        values_[i] = clamp01(spec_.parameters[i].defaultValue);
    recreateLegacy();
}

tresult PLUGIN_API LegacyEffectProcessor::initialize(FUnknown* context) {
    const auto result = AudioEffect::initialize(context);
    if (result != kResultOk)
        return result;

    addAudioInput(STR16("Stereo In"), Vst::SpeakerArr::kStereo);
    addAudioOutput(STR16("Stereo Out"), Vst::SpeakerArr::kStereo);
    return kResultOk;
}

tresult PLUGIN_API LegacyEffectProcessor::setBusArrangements(
    Vst::SpeakerArrangement* inputs, int32 numIns,
    Vst::SpeakerArrangement* outputs, int32 numOuts) {
    if (!inputs || !outputs || numIns != 1 || numOuts != 1 ||
        inputs[0] != Vst::SpeakerArr::kStereo ||
        outputs[0] != Vst::SpeakerArr::kStereo)
        return kResultFalse;
    return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
}

tresult PLUGIN_API LegacyEffectProcessor::setupProcessing(Vst::ProcessSetup& setup) {
    const auto result = AudioEffect::setupProcessing(setup);
    if (result == kResultOk) {
        sampleRate_ = setup.sampleRate > 1.0 ? setup.sampleRate : 44100.0;
        ensureScratch(static_cast<std::size_t>(std::max<int32>(setup.maxSamplesPerBlock, 1)));
        recreateLegacy();
    }
    return result;
}

tresult PLUGIN_API LegacyEffectProcessor::setActive(TBool state) {
    if (state)
        recreateLegacy();
    return AudioEffect::setActive(state);
}

tresult PLUGIN_API LegacyEffectProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
    return symbolicSampleSize == Vst::kSample32 ? kResultTrue : kResultFalse;
}

void LegacyEffectProcessor::applyParameterChanges(Vst::IParameterChanges* changes) {
    if (!changes)
        return;

    bool changed = false;
    const int32 count = changes->getParameterCount();
    for (int32 i = 0; i < count; ++i) {
        auto* queue = changes->getParameterData(i);
        if (!queue || queue->getPointCount() <= 0)
            continue;

        int32 sampleOffset = 0;
        Vst::ParamValue value = 0.0;
        if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) != kResultTrue)
            continue;

        if (queue->getParameterId() == kLegacyBypassId) {
            bypass_ = value >= 0.5;
            continue;
        }

        for (std::size_t p = 0; p < spec_.parameterCount; ++p) {
            if (spec_.parameters[p].id == queue->getParameterId()) {
                values_[p] = clamp01(value);
                changed = true;
                break;
            }
        }
    }

    if (changed)
        syncParameters();
}

void LegacyEffectProcessor::recreateLegacy() {
    legacy_.reset(spec_.factory ? spec_.factory() : nullptr);
    if (legacy_)
        legacy_->setSampleRate(static_cast<float>(sampleRate_));
    syncParameters();
}

void LegacyEffectProcessor::syncParameters() {
    inputGain_ = 1.0f;
    outputGain_ = 1.0f;
    wetMix_ = 1.0f;
    usesWetMix_ = false;

    if (legacy_) {
        for (std::size_t i = 0; i < spec_.fixedParameterCount; ++i)
            legacy_->setParameter(spec_.fixedParameters[i].legacyIndex,
                                  spec_.fixedParameters[i].value);
    }

    for (std::size_t i = 0; i < spec_.parameterCount; ++i) {
        const auto& parameter = spec_.parameters[i];
        const float mapped = parameter.mapper
            ? parameter.mapper(values_[i])
            : mapDirect(values_[i]);

        switch (parameter.target) {
            case LegacyParameterTarget::Legacy:
                if (legacy_ && parameter.legacyIndex >= 0)
                    legacy_->setParameter(parameter.legacyIndex, mapped);
                break;
            case LegacyParameterTarget::InputGain:
                inputGain_ = mapped;
                break;
            case LegacyParameterTarget::OutputGain:
                outputGain_ = mapped;
                break;
            case LegacyParameterTarget::WetMix:
                wetMix_ = std::clamp(mapped, 0.0f, 1.0f);
                usesWetMix_ = true;
                break;
        }
    }
}

void LegacyEffectProcessor::ensureScratch(std::size_t samples) {
    if (dryLeft_.size() < samples)
        dryLeft_.resize(samples);
    if (dryRight_.size() < samples)
        dryRight_.resize(samples);
}

tresult PLUGIN_API LegacyEffectProcessor::process(Vst::ProcessData& data) {
    applyParameterChanges(data.inputParameterChanges);

    if (data.numSamples <= 0 || data.numInputs < 1 || data.numOutputs < 1)
        return kResultOk;
    if (data.symbolicSampleSize != Vst::kSample32)
        return kResultFalse;

    auto& input = data.inputs[0];
    auto& output = data.outputs[0];
    if (input.numChannels != 2 || output.numChannels != 2)
        return kResultFalse;

    float* inL = input.channelBuffers32[0];
    float* inR = input.channelBuffers32[1];
    float* outL = output.channelBuffers32[0];
    float* outR = output.channelBuffers32[1];
    if (!inL || !inR || !outL || !outR)
        return kResultFalse;

    const auto sampleCount = static_cast<std::size_t>(data.numSamples);
    const auto bytes = sampleCount * sizeof(float);
    if (outL != inL) std::memcpy(outL, inL, bytes);
    if (outR != inR) std::memcpy(outR, inR, bytes);

    if (bypass_ || !legacy_) {
        output.silenceFlags = input.silenceFlags;
        return kResultOk;
    }

    if (usesWetMix_) {
        ensureScratch(sampleCount);
        std::memcpy(dryLeft_.data(), outL, bytes);
        std::memcpy(dryRight_.data(), outR, bytes);
    }

    if (inputGain_ != 1.0f) {
        for (std::size_t i = 0; i < sampleCount; ++i) {
            outL[i] *= inputGain_;
            outR[i] *= inputGain_;
        }
    }

    float* inputs[2]{outL, outR};
    float* outputs[2]{outL, outR};
    legacy_->processReplacing(inputs, outputs, data.numSamples);

    if (usesWetMix_ && wetMix_ < 1.0f) {
        const float dry = 1.0f - wetMix_;
        for (std::size_t i = 0; i < sampleCount; ++i) {
            outL[i] = (outL[i] * wetMix_) + (dryLeft_[i] * dry);
            outR[i] = (outR[i] * wetMix_) + (dryRight_[i] * dry);
        }
    }

    if (outputGain_ != 1.0f) {
        for (std::size_t i = 0; i < sampleCount; ++i) {
            outL[i] *= outputGain_;
            outR[i] *= outputGain_;
        }
    }

    output.silenceFlags = 0;
    return kResultOk;
}

tresult PLUGIN_API LegacyEffectProcessor::setState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    for (auto& value : values_) {
        float stored = 0.0f;
        if (!stream.readFloat(stored))
            return kResultFalse;
        value = clamp01(stored);
    }

    int32 bypass = 0;
    if (!stream.readInt32(bypass))
        return kResultFalse;
    bypass_ = bypass != 0;
    syncParameters();
    return kResultOk;
}

tresult PLUGIN_API LegacyEffectProcessor::getState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    for (const auto value : values_)
        stream.writeFloat(static_cast<float>(value));
    stream.writeInt32(bypass_ ? 1 : 0);
    return kResultOk;
}

LegacyEffectController::LegacyEffectController(const LegacyPluginSpec& spec)
    : spec_(spec) {}

tresult PLUGIN_API LegacyEffectController::initialize(FUnknown* context) {
    const auto result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    parameters.addParameter(
        STR16("Bypass"), nullptr, 1, 0.0,
        Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass,
        kLegacyBypassId);

    for (std::size_t i = 0; i < spec_.parameterCount; ++i) {
        const auto& parameter = spec_.parameters[i];
        parameters.addParameter(
            parameter.name, nullptr, parameter.stepCount, parameter.defaultValue,
            Vst::ParameterInfo::kCanAutomate, parameter.id);
    }

    return kResultOk;
}

tresult PLUGIN_API LegacyEffectController::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    for (std::size_t i = 0; i < spec_.parameterCount; ++i) {
        float value = 0.0f;
        if (!stream.readFloat(value))
            return kResultFalse;
        setParamNormalized(spec_.parameters[i].id, clamp01(value));
    }

    int32 bypass = 0;
    if (!stream.readInt32(bypass))
        return kResultFalse;
    setParamNormalized(kLegacyBypassId, bypass ? 1.0 : 0.0);
    return kResultOk;
}

IPlugView* PLUGIN_API LegacyEffectController::createView(FIDString name) {
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        return new VSTGUI::VST3Editor(this, "view", spec_.editorFile);
    return nullptr;
}

} // namespace Steinberg::LVS
