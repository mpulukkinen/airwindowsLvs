#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "AirwindowsReverbDsp.h"

namespace Steinberg::LVS {

class ReverbProcessor final : public Vst::AudioEffect {
public:
    ReverbProcessor();
    ~ReverbProcessor() SMTG_OVERRIDE = default;

    static FUnknown* createInstance(void*) {
        return static_cast<Vst::IAudioProcessor*>(new ReverbProcessor());
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

    lvs::dsp::AirwindowsReverbDsp dsp_;
    Vst::ParamValue size_{0.5};
    Vst::ParamValue mix_{0.25};
    bool bypass_{false};
};

} // namespace Steinberg::LVS
