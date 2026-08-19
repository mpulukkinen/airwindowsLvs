#include "../../common/LegacyAirwindowsVst3.h"
#include "StereoChorus.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x0B030120,0x1CA42C5D,0xB39FC3D2,0x277B6114); const FUID kControllerUID(0x7A7A879D,0xD007E87A,0x9BADDCB7,0x29ABF17A); const LegacyParameterSpec kParameters[]={{STR16("Speed"),1000,0,0.25,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Depth"),1001,1,0.35,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix"),1002,-1,0.5,0,LegacyParameterTarget::WetMix,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::StereoChorus(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,3,nullptr,0,&createLegacyEffect,"Chorus.uidesc"}; } class ChorusProcessor final:public LegacyEffectProcessor{public:ChorusProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new ChorusProcessor());}}; class ChorusController final:public LegacyEffectController{public:ChorusController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new ChorusController());}}; }
#define stringPluginName "LVS Chorus"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Modulation",FULL_VERSION_STR,kVstVersionString,ChorusProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,ChorusController::createInstance)
END_FACTORY
