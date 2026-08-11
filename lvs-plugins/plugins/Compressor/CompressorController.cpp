#include "CompressorController.h"
#include "CompressorCids.h"

#include "base/source/fstreamer.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Steinberg::LVS {

tresult PLUGIN_API CompressorController::initialize(FUnknown* context) {
    const auto result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0,
        Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass, kCompressorBypassId);
    parameters.addParameter(STR16("Compression"), nullptr, 0, 0.0,
        Vst::ParameterInfo::kCanAutomate, kCompressId);
    parameters.addParameter(STR16("Output"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kCompressorOutputId);
    parameters.addParameter(STR16("Mix"), nullptr, 0, 1.0,
        Vst::ParameterInfo::kCanAutomate, kCompressorMixId);

    return kResultOk;
}

tresult PLUGIN_API CompressorController::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float compress, output, mix;
    int32 bypass;
    if (!stream.readFloat(compress) || !stream.readFloat(output) ||
        !stream.readFloat(mix) || !stream.readInt32(bypass))
        return kResultFalse;

    setParamNormalized(kCompressId, compress);
    setParamNormalized(kCompressorOutputId, output);
    setParamNormalized(kCompressorMixId, mix);
    setParamNormalized(kCompressorBypassId, bypass ? 1.0 : 0.0);
    return kResultOk;
}

IPlugView* PLUGIN_API CompressorController::createView(FIDString name) {
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        return new VSTGUI::VST3Editor(this, "view", "Compressor.uidesc");
    return nullptr;
}

} // namespace Steinberg::LVS
