#include "EqualizerProcessor.h"
#include "EqualizerCids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include <algorithm>
#include <cstring>

namespace Steinberg::LVS {

EqualizerProcessor::EqualizerProcessor() {
    setControllerClass(kEqualizerControllerUID);
}

tresult PLUGIN_API EqualizerProcessor::initialize(FUnknown* context) {
    const auto result = AudioEffect::initialize(context);
    if (result != kResultOk)
        return result;

    addAudioInput(STR16("Stereo In"), Vst::SpeakerArr::kStereo);
    addAudioOutput(STR16("Stereo Out"), Vst::SpeakerArr::kStereo);
    return kResultOk;
}

tresult PLUGIN_API EqualizerProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs, int32 numIns,
                                                           Vst::SpeakerArrangement* outputs, int32 numOuts) {
    if (numIns != 1 || numOuts != 1 || !inputs || !outputs ||
        inputs[0] != Vst::SpeakerArr::kStereo || outputs[0] != Vst::SpeakerArr::kStereo)
        return kResultFalse;

    return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
}

tresult PLUGIN_API EqualizerProcessor::setupProcessing(Vst::ProcessSetup& setup) {
    const auto result = AudioEffect::setupProcessing(setup);
    if (result == kResultOk) {
        dsp_.prepare(setup.sampleRate, static_cast<std::uint32_t>(std::max<int32>(setup.maxSamplesPerBlock, 1)));
        syncDspParameters();
    }
    return result;
}

tresult PLUGIN_API EqualizerProcessor::setActive(TBool state) {
    if (state)
        dsp_.reset();
    return AudioEffect::setActive(state);
}

tresult PLUGIN_API EqualizerProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
    return symbolicSampleSize == Vst::kSample32 ? kResultTrue : kResultFalse;
}

void EqualizerProcessor::applyParameters(Vst::IParameterChanges* changes) {
    if (!changes)
        return;

    const int32 count = changes->getParameterCount();
    for (int32 i = 0; i < count; ++i) {
        auto* queue = changes->getParameterData(i);
        if (!queue || queue->getPointCount() <= 0)
            continue;

        int32 sampleOffset = 0;
        Vst::ParamValue value = 0.0;
        if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) != kResultTrue)
            continue;

        switch (queue->getParameterId()) {
            case kHighId: high_ = value; break;
            case kHighMidId: highMid_ = value; break;
            case kLowMidId: lowMid_ = value; break;
            case kBassId: bass_ = value; break;
            case kBypassId: bypass_ = value >= 0.5; break;
            default: break;
        }
    }

    syncDspParameters();
}

void EqualizerProcessor::syncDspParameters() noexcept {
    dsp_.setHigh(static_cast<float>(high_));
    dsp_.setHighMid(static_cast<float>(highMid_));
    dsp_.setLowMid(static_cast<float>(lowMid_));
    dsp_.setBass(static_cast<float>(bass_));
}

tresult PLUGIN_API EqualizerProcessor::process(Vst::ProcessData& data) {
    applyParameters(data.inputParameterChanges);

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

    const auto bytes = static_cast<std::size_t>(data.numSamples) * sizeof(float);
    if (outL != inL)
        std::memcpy(outL, inL, bytes);
    if (outR != inR)
        std::memcpy(outR, inR, bytes);

    if (!bypass_)
        dsp_.process({outL, outR, static_cast<std::uint32_t>(data.numSamples)});

    output.silenceFlags = input.silenceFlags;
    return kResultOk;
}

tresult PLUGIN_API EqualizerProcessor::setState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float high, highMid, lowMid, bass;
    int32 bypass;
    if (!stream.readFloat(high) || !stream.readFloat(highMid) || !stream.readFloat(lowMid) ||
        !stream.readFloat(bass) || !stream.readInt32(bypass))
        return kResultFalse;

    high_ = std::clamp<double>(high, 0.0, 1.0);
    highMid_ = std::clamp<double>(highMid, 0.0, 1.0);
    lowMid_ = std::clamp<double>(lowMid, 0.0, 1.0);
    bass_ = std::clamp<double>(bass, 0.0, 1.0);
    bypass_ = bypass != 0;
    syncDspParameters();
    return kResultOk;
}

tresult PLUGIN_API EqualizerProcessor::getState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    stream.writeFloat(static_cast<float>(high_));
    stream.writeFloat(static_cast<float>(highMid_));
    stream.writeFloat(static_cast<float>(lowMid_));
    stream.writeFloat(static_cast<float>(bass_));
    stream.writeInt32(bypass_ ? 1 : 0);
    return kResultOk;
}

} // namespace Steinberg::LVS
