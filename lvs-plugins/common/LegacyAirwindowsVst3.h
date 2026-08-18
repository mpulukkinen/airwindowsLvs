#pragma once

#include "audioeffectx.h"

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/vsttypes.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace Steinberg::LVS {

inline constexpr Vst::ParamID kLegacyBypassId = 100;

enum class LegacyParameterTarget : std::uint8_t {
    Legacy,
    InputGain,
    OutputGain,
    WetMix
};

using LegacyParameterMapper = float (*)(double) noexcept;
using LegacyFactory = AudioEffectX* (*)();

struct LegacyParameterSpec {
    const TChar* name;
    Vst::ParamID id;
    int32 legacyIndex;
    double defaultValue;
    int32 stepCount;
    LegacyParameterTarget target;
    LegacyParameterMapper mapper;
};

struct LegacyFixedParameter {
    int32 legacyIndex;
    float value;
};

struct LegacyPluginSpec {
    const FUID* controllerUid;
    const LegacyParameterSpec* parameters;
    std::size_t parameterCount;
    const LegacyFixedParameter* fixedParameters;
    std::size_t fixedParameterCount;
    LegacyFactory factory;
    const char* editorFile;
};

float mapDirect(double value) noexcept;
float mapInverse(double value) noexcept;
float mapGain0To18(double value) noexcept;
float mapGainMinus12To0(double value) noexcept;

class LegacyEffectProcessor : public Vst::AudioEffect {
public:
    explicit LegacyEffectProcessor(const LegacyPluginSpec& spec);
    ~LegacyEffectProcessor() SMTG_OVERRIDE = default;

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
    void applyParameterChanges(Vst::IParameterChanges* changes);
    void recreateLegacy();
    void syncParameters();
    void ensureScratch(std::size_t samples);

    const LegacyPluginSpec& spec_;
    std::unique_ptr<AudioEffectX> legacy_;
    std::vector<Vst::ParamValue> values_;
    std::vector<float> dryLeft_;
    std::vector<float> dryRight_;
    double sampleRate_{44100.0};
    float inputGain_{1.0f};
    float outputGain_{1.0f};
    float wetMix_{1.0f};
    bool usesWetMix_{false};
    bool bypass_{false};
};

class LegacyEffectController : public Vst::EditControllerEx1 {
public:
    explicit LegacyEffectController(const LegacyPluginSpec& spec);
    ~LegacyEffectController() SMTG_OVERRIDE = default;

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
    IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;

    DEFINE_INTERFACES
    END_DEFINE_INTERFACES(EditController)
    DELEGATE_REFCOUNT(EditController)

private:
    const LegacyPluginSpec& spec_;
};

} // namespace Steinberg::LVS
