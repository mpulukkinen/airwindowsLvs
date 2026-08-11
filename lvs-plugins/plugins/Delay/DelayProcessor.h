#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "TapeDelay2Dsp.h"

namespace Steinberg::LVS {

class DelayProcessor final : public Vst::AudioEffect {
public:
    DelayProcessor();
    ~DelayProcessor() SMTG_OVERRIDE = default;

    static FUnknown* createInstance(void*) {
        return static_cast<Vst::IAudioProcessor*>(new DelayProcessor());
    }

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API setBusArrangements(Vst::SpeakerArrangement* inputs, int32 numIns,
                                          Vst::SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;
    tresult PLUGIN_API setupProcessing(Vst::ProcessSetup& setup) SMTG_OVERRIDE;
    tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
    tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
    tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

private:
    void applyParameters(Vst::IParameterChanges* changes);
    void syncDspParameters() noexcept;

    lvs::dsp::TapeDelay2Dsp dsp_;
    Vst::ParamValue time_{0.35};
    Vst::ParamValue feedback_{0.25};
    Vst::ParamValue tone_{0.5};
    Vst::ParamValue flutter_{0.0};
    Vst::ParamValue mix_{0.25};
    bool bypass_{false};
};

} // namespace Steinberg::LVS
