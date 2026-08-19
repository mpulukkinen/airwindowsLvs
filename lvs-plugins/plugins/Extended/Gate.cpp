#include "../../common/LegacyAirwindowsVst3.h"
#include "SoftGate.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x6C06E218,0x0383E55E,0x3E9FBC86,0xF799A67B); const FUID kControllerUID(0x260A8D57,0xA9CE975D,0x8746EC06,0xAA8958FA); const LegacyParameterSpec kParameters[]={{STR16("Threshold"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Darken"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Silence"),1002,2,0.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::SoftGate(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,3,nullptr,0,&createLegacyEffect,"Gate.uidesc"}; } class GateProcessor final:public LegacyEffectProcessor{public:GateProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new GateProcessor());}}; class GateController final:public LegacyEffectController{public:GateController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new GateController());}}; }
#define stringPluginName "LVS Gate"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Dynamics",FULL_VERSION_STR,kVstVersionString,GateProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,GateController::createInstance)
END_FACTORY
