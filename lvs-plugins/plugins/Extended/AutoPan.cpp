#include "../../common/LegacyAirwindowsVst3.h"
#include "AutoPan.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x2FE3849B,0xD0B2C4A2,0xEB05F63B,0x36F43E8D); const FUID kControllerUID(0xF86434F2,0x8DFDB8A1,0xC97B0EAE,0x61D23BFB); const LegacyParameterSpec kParameters[]={{STR16("Rate"),1000,0,0.1,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Phase"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Width"),1002,2,0.0,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix"),1003,3,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::AutoPan(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,4,nullptr,0,&createLegacyEffect,"AutoPan.uidesc"}; } class AutoPanProcessor final:public LegacyEffectProcessor{public:AutoPanProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new AutoPanProcessor());}}; class AutoPanController final:public LegacyEffectController{public:AutoPanController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new AutoPanController());}}; }
#define stringPluginName "LVS Auto Pan"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Modulation",FULL_VERSION_STR,kVstVersionString,AutoPanProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,AutoPanController::createInstance)
END_FACTORY
