#include "ReverbController.h"
#include "ReverbCids.h"

#include "base/source/fstreamer.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Steinberg::LVS {

tresult PLUGIN_API ReverbController::initialize(FUnknown* context) {
    const auto result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0,
        Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass, kReverbBypassId);
    parameters.addParameter(STR16("Size"), nullptr, 0, 0.5,
        Vst::ParameterInfo::kCanAutomate, kReverbSizeId);
    parameters.addParameter(STR16("Mix"), nullptr, 0, 0.25,
        Vst::ParameterInfo::kCanAutomate, kReverbMixId);

    return kResultOk;
}

tresult PLUGIN_API ReverbController::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float size, mix;
    int32 bypass;
    if (!stream.readFloat(size) || !stream.readFloat(mix) || !stream.readInt32(bypass))
        return kResultFalse;

    setParamNormalized(kReverbSizeId, size);
    setParamNormalized(kReverbMixId, mix);
    setParamNormalized(kReverbBypassId, bypass ? 1.0 : 0.0);
    return kResultOk;
}

IPlugView* PLUGIN_API ReverbController::createView(FIDString name) {
    if (FIDStringsEqual(name, Vst::ViewType::kEditor))
        return new VSTGUI::VST3Editor(this, "view", "Reverb.uidesc");
    return nullptr;
}

} // namespace Steinberg::LVS
