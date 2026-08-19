#include "../../common/LegacyAirwindowsVst3.h"
#include "Tremolo.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x26682F28,0x93922D00,0x44704442,0x42E31F1A); const FUID kControllerUID(0x55CD26EC,0x2B0D36DD,0x5B8B70E9,0x452CEC71); const LegacyParameterSpec kParameters[]={{STR16("Speed"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Depth"),1001,1,0.75,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::Tremolo(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,2,nullptr,0,&createLegacyEffect,"Tremolo.uidesc"}; } class TremoloProcessor final:public LegacyEffectProcessor{public:TremoloProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new TremoloProcessor());}}; class TremoloController final:public LegacyEffectController{public:TremoloController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new TremoloController());}}; }
#define stringPluginName "LVS Tremolo"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Modulation",FULL_VERSION_STR,kVstVersionString,TremoloProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,TremoloController::createInstance)
END_FACTORY
