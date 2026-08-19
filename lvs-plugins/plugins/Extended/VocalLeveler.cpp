#include "../../common/LegacyAirwindowsVst3.h"
#include "Podcast.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0xC67E7ED9,0xC3FC118E,0xD3DF2AAE,0x337818CD); const FUID kControllerUID(0xA28C7F5C,0xAC379BC9,0xA04661BA,0xB1F94799); const LegacyParameterSpec kParameters[]={{STR16("Boost"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix"),1001,1,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::Podcast(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,2,nullptr,0,&createLegacyEffect,"VocalLeveler.uidesc"}; } class VocalLevelerProcessor final:public LegacyEffectProcessor{public:VocalLevelerProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new VocalLevelerProcessor());}}; class VocalLevelerController final:public LegacyEffectController{public:VocalLevelerController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new VocalLevelerController());}}; }
#define stringPluginName "LVS Vocal Leveler"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Dynamics",FULL_VERSION_STR,kVstVersionString,VocalLevelerProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,VocalLevelerController::createInstance)
END_FACTORY
