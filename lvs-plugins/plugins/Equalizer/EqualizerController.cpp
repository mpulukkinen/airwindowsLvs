#include "EqualizerController.h"
#include "EqualizerCids.h"

#include "base/source/fstreamer.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Steinberg::LVS {

tresult PLUGIN_API EqualizerController::initialize(FUnknown* context) {
    const auto result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0,
        Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass, kBypassId);
    parameters.addParameter(STR16("High"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kHighId);
    parameters.addParameter(STR16("High Mid"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kHighMidId);
    parameters.addParameter(STR16("Low Mid"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kLowMidId);
    parameters.addParameter(STR16("Bass"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kBassId);

    return kResultOk;
}

tresult PLUGIN_API EqualizerController::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float high, highMid, lowMid, bass;
    int32 bypass;
    if (!stream.readFloat(high) || !stream.readFloat(highMid) || !stream.readFloat(lowMid) ||
        !stream.readFloat(bass) || !stream.readInt32(bypass))
        return kResultFalse;

    setParamNormalized(kHighId, high);
    setParamNormalized(kHighMidId, highMid);
    setParamNormalized(kLowMidId, lowMid);
    setParamNormalized(kBassId, bass);
    setParamNormalized(kBypassId, bypass ? 1.0 : 0.0);
    return kResultOk;
}

IPlugView* PLUGIN_API EqualizerController::createView(FIDString name) {
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        return new VSTGUI::VST3Editor(this, "view", "Equalizer.uidesc");
    return nullptr;
}

} // namespace Steinberg::LVS
