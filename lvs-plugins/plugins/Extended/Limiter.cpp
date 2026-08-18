#include "../../common/LegacyAirwindowsVst3.h"

#include "Recurve.h"
#include "public.sdk/source/main/pluginfactory.h"

namespace Steinberg::LVS {
namespace {
const FUID kProcessorUID(0x248BA419, 0x8151C02B, 0x9B4BC5E9, 0xB0C38AFE);
const FUID kControllerUID(0xEE6CBA61, 0xE8DE692E, 0xC8F60514, 0x6413F939);
const LegacyParameterSpec kParameters[] = {
    {STR16("Drive"), 1000, -1, 0.25, 0, LegacyParameterTarget::InputGain, mapGain0To18},
    {STR16("Output"), 1001, -1, 1.0, 0, LegacyParameterTarget::OutputGain, mapGainMinus12To0},
    {STR16("Mix"), 1002, -1, 1.0, 0, LegacyParameterTarget::WetMix, mapDirect},
};
AudioEffectX* createLegacyEffect() { return new ::Recurve(nullptr); }
const LegacyPluginSpec kPluginSpec{&kControllerUID, kParameters, 3, nullptr, 0, &createLegacyEffect, "Limiter.uidesc"};
}
class LimiterProcessor final : public LegacyEffectProcessor { public: LimiterProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*) { return static_cast<Vst::IAudioProcessor*>(new LimiterProcessor()); } };
class LimiterController final : public LegacyEffectController { public: LimiterController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*) { return static_cast<Vst::IEditController*>(new LimiterController()); } };
}
#define stringPluginName "LVS Limiter"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS;
BEGIN_FACTORY_DEF("Lyric Video Studio", "https://lyricvideo.studio", "mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID), PClassInfo::kManyInstances, kVstAudioEffectClass, stringPluginName, Vst::kDistributable, "Fx|Dynamics", FULL_VERSION_STR, kVstVersionString, LimiterProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID), PClassInfo::kManyInstances, kVstComponentControllerClass, stringPluginName " Controller", 0, "", FULL_VERSION_STR, kVstVersionString, LimiterController::createInstance)
END_FACTORY
