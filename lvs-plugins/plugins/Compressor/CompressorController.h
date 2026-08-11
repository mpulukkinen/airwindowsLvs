#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Steinberg::LVS {

class CompressorController final : public Vst::EditControllerEx1 {
public:
    CompressorController() = default;
    ~CompressorController() SMTG_OVERRIDE = default;

    static FUnknown* createInstance(void*) {
        return static_cast<Vst::IEditController*>(new CompressorController());
    }

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
    IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;

    DEFINE_INTERFACES
    END_DEFINE_INTERFACES(EditController)
    DELEGATE_REFCOUNT(EditController)
};

} // namespace Steinberg::LVS
