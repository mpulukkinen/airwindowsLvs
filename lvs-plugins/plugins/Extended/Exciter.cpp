#include "../../common/LegacyAirwindowsVst3.h"
#include "Air3.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x90669E27,0xE5E8F81B,0x4A0A6650,0x204B4420); const FUID kControllerUID(0x7560C070,0x3F154217,0x08F6F9FD,0x7285CCE4); const LegacyParameterSpec kParameters[]={{STR16("Air"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Ground"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::Air3(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,2,nullptr,0,&createLegacyEffect,"Exciter.uidesc"}; } class ExciterProcessor final:public LegacyEffectProcessor{public:ExciterProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new ExciterProcessor());}}; class ExciterController final:public LegacyEffectController{public:ExciterController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new ExciterController());}}; }
#define stringPluginName "LVS Exciter"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|EQ",FULL_VERSION_STR,kVstVersionString,ExciterProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,ExciterController::createInstance)
END_FACTORY
