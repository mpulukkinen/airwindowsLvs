#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "ButterComp2Dsp.h"

namespace Steinberg::LVS {

class CompressorProcessor final : public Vst::AudioEffect {
public:
    CompressorProcessor();
    ~CompressorProcessor() SMTG_OVERRIDE = default;

    static FUnknown* createInstance(void*) {
        return static_cast<Vst::IAudioProcessor*>(new CompressorProcessor());
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

    lvs::dsp::ButterComp2 dsp_;
    Vst::ParamValue compress_{0.0};
    Vst::ParamValue output_{0.5};
    Vst::ParamValue mix_{1.0};
    bool bypass_{false};
};

} // namespace Steinberg::LVS
