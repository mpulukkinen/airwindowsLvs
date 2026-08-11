#include "CompressorProcessor.h"
#include "CompressorCids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include <algorithm>
#include <cstring>

namespace Steinberg::LVS {

CompressorProcessor::CompressorProcessor() {
    setControllerClass(kCompressorControllerUID);
}

tresult PLUGIN_API CompressorProcessor::initialize(FUnknown* context) {
    const auto result = AudioEffect::initialize(context);
    if (result != kResultOk)
        return result;

    addAudioInput(STR16("Stereo In"), Vst::SpeakerArr::kStereo);
    addAudioOutput(STR16("Stereo Out"), Vst::SpeakerArr::kStereo);
    return kResultOk;
}

tresult PLUGIN_API CompressorProcessor::setupProcessing(Vst::ProcessSetup& setup) {
    const auto result = AudioEffect::setupProcessing(setup);
    if (result == kResultOk) {
        dsp_.prepare(setup.sampleRate, static_cast<std::uint32_t>(std::max<int32>(setup.maxSamplesPerBlock, 1)));
        syncDspParameters();
    }
    return result;
}

tresult PLUGIN_API CompressorProcessor::setActive(TBool state) {
    if (state)
        dsp_.reset();
    return AudioEffect::setActive(state);
}

tresult PLUGIN_API CompressorProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
    return symbolicSampleSize == Vst::kSample32 ? kResultTrue : kResultFalse;
}

void CompressorProcessor::applyParameters(Vst::IParameterChanges* changes) {
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
            case kCompressId: compress_ = value; break;
            case kCompressorOutputId: output_ = value; break;
            case kCompressorMixId: mix_ = value; break;
            case kCompressorBypassId: bypass_ = value >= 0.5; break;
            default: break;
        }
    }

    syncDspParameters();
}

void CompressorProcessor::syncDspParameters() noexcept {
    dsp_.setCompress(static_cast<float>(compress_));
    dsp_.setOutput(static_cast<float>(output_));
    dsp_.setMix(static_cast<float>(mix_));
}

tresult PLUGIN_API CompressorProcessor::process(Vst::ProcessData& data) {
    applyParameters(data.inputParameterChanges);

    if (data.numSamples <= 0 || data.numInputs < 1 || data.numOutputs < 1)
        return kResultOk;
    if (data.symbolicSampleSize != Vst::kSample32)
        return kResultFalse;

    auto& input = data.inputs[0];
    auto& output = data.outputs[0];
    if (input.numChannels < 2 || output.numChannels < 2)
        return kResultFalse;

    float* inL = input.channelBuffers32[0];
    float* inR = input.channelBuffers32[1];
    float* outL = output.channelBuffers32[0];
    float* outR = output.channelBuffers32[1];
    if (!inL || !inR || !outL || !outR)
        return kResultFalse;

    const auto bytes = static_cast<std::size_t>(data.numSamples) * sizeof(float);
    if (outL != inL) std::memcpy(outL, inL, bytes);
    if (outR != inR) std::memcpy(outR, inR, bytes);

    if (!bypass_)
        dsp_.process({outL, outR, static_cast<std::uint32_t>(data.numSamples)});

    output.silenceFlags = input.silenceFlags;
    return kResultOk;
}

tresult PLUGIN_API CompressorProcessor::setState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float compress, output, mix;
    int32 bypass;
    if (!stream.readFloat(compress) || !stream.readFloat(output) ||
        !stream.readFloat(mix) || !stream.readInt32(bypass))
        return kResultFalse;

    compress_ = std::clamp<double>(compress, 0.0, 1.0);
    output_ = std::clamp<double>(output, 0.0, 1.0);
    mix_ = std::clamp<double>(mix, 0.0, 1.0);
    bypass_ = bypass != 0;
    syncDspParameters();
    return kResultOk;
}

tresult PLUGIN_API CompressorProcessor::getState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    stream.writeFloat(static_cast<float>(compress_));
    stream.writeFloat(static_cast<float>(output_));
    stream.writeFloat(static_cast<float>(mix_));
    stream.writeInt32(bypass_ ? 1 : 0);
    return kResultOk;
}

} // namespace Steinberg::LVS
