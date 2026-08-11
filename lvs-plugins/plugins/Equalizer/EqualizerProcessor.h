#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "PearLiteEqDsp.h"

namespace Steinberg::LVS {

class EqualizerProcessor final : public Vst::AudioEffect {
public:
    EqualizerProcessor();
    ~EqualizerProcessor() SMTG_OVERRIDE = default;

    static FUnknown* createInstance(void*) {
        return static_cast<Vst::IAudioProcessor*>(new EqualizerProcessor());
    }

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API setupProcessing(Vst::ProcessSetup& setup) SMTG_OVERRIDE;
    tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
    tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
    tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

private:
    void applyParameters(Vst::IParameterChanges* changes);
    void syncDspParameters() noexcept;

    lvs::dsp::PearLiteEq dsp_;
    Vst::ParamValue high_{0.5};
    Vst::ParamValue highMid_{0.5};
    Vst::ParamValue lowMid_{0.5};
    Vst::ParamValue bass_{0.5};
    bool bypass_{false};
};

} // namespace Steinberg::LVS
