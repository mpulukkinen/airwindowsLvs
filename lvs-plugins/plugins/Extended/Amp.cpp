#include "../../common/LegacyAirwindowsVst3.h"
#include "FireAmp.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x69A9F2A8,0x097E6E89,0x5EE7F7D0,0xCFD15A18); const FUID kControllerUID(0x7B293FFE,0xF22ED73B,0x8DD2CE7A,0x0CA21F3B); const LegacyParameterSpec kParameters[]={{STR16("Gain"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Tone"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Output"),1002,2,0.8,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix"),1003,3,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::FireAmp(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,4,nullptr,0,&createLegacyEffect,"Amp.uidesc"}; } class AmpProcessor final:public LegacyEffectProcessor{public:AmpProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new AmpProcessor());}}; class AmpController final:public LegacyEffectController{public:AmpController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new AmpController());}}; }
#define stringPluginName "LVS Guitar Amp"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Distortion",FULL_VERSION_STR,kVstVersionString,AmpProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,AmpController::createInstance)
END_FACTORY
