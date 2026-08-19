#include "../../common/LegacyAirwindowsVst3.h"
#include "DeRez4.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0xF7D26336,0xFE2E896E,0x906EF678,0x238191F8); const FUID kControllerUID(0xBF6238BD,0xA923FD9D,0x6D2228CF,0xFA054F3C); const LegacyParameterSpec kParameters[]={{STR16("Downsample"),1000,0,0.618,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Brightness"),1001,1,0.618,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Bass"),1002,2,0.382,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Output"),1003,3,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::DeRez4(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,4,nullptr,0,&createLegacyEffect,"LoFi.uidesc"}; } class LoFiProcessor final:public LegacyEffectProcessor{public:LoFiProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new LoFiProcessor());}}; class LoFiController final:public LegacyEffectController{public:LoFiController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new LoFiController());}}; }
#define stringPluginName "LVS Lo-Fi"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Distortion",FULL_VERSION_STR,kVstVersionString,LoFiProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,LoFiController::createInstance)
END_FACTORY
