#include "DelayController.h"
#include "DelayCids.h"

#include "base/source/fstreamer.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Steinberg::LVS {

tresult PLUGIN_API DelayController::initialize(FUnknown* context) {
    const auto result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0,
        Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass, kDelayBypassId);
    parameters.addParameter(STR16("Time"), nullptr, 0, 0.35,
        Vst::ParameterInfo::kCanAutomate, kDelayTimeId);
    parameters.addParameter(STR16("Feedback"), nullptr, 0, 0.25,
        Vst::ParameterInfo::kCanAutomate, kDelayFeedbackId);
    parameters.addParameter(STR16("Tone"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kDelayToneId);
    parameters.addParameter(STR16("Flutter"), nullptr, 0, 0.0,
        Vst::ParameterInfo::kCanAutomate, kDelayFlutterId);
    parameters.addParameter(STR16("Mix"), nullptr, 0, 0.25,
        Vst::ParameterInfo::kCanAutomate, kDelayMixId);

    return kResultOk;
}

tresult PLUGIN_API DelayController::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float time, feedback, tone, flutter, mix;
    int32 bypass;
    if (!stream.readFloat(time) || !stream.readFloat(feedback) || !stream.readFloat(tone) ||
        !stream.readFloat(flutter) || !stream.readFloat(mix) || !stream.readInt32(bypass))
        return kResultFalse;

    setParamNormalized(kDelayTimeId, time);
    setParamNormalized(kDelayFeedbackId, feedback);
    setParamNormalized(kDelayToneId, tone);
    setParamNormalized(kDelayFlutterId, flutter);
    setParamNormalized(kDelayMixId, mix);
    setParamNormalized(kDelayBypassId, bypass ? 1.0 : 0.0);
    return kResultOk;
}

IPlugView* PLUGIN_API DelayController::createView(FIDString name) {
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        return new VSTGUI::VST3Editor(this, "view", "Delay.uidesc");
    return nullptr;
}

} // namespace Steinberg::LVS
