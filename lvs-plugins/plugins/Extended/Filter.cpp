#include "../../common/LegacyAirwindowsVst3.h"
#include "BiquadDouble.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x0596AD04,0xBAA6ACD7,0x1B6B43F1,0x5D5B01FE); const FUID kControllerUID(0xB875D3BC,0xBE6F3FE1,0x14E8C317,0x86A18AED); const LegacyParameterSpec kParameters[]={{STR16("Type"),1000,0,1.0,3,LegacyParameterTarget::Legacy,mapDirect},{STR16("Frequency"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Q"),1002,2,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix / Invert"),1003,3,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::BiquadDouble(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,4,nullptr,0,&createLegacyEffect,"Filter.uidesc"}; } class FilterProcessor final:public LegacyEffectProcessor{public:FilterProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new FilterProcessor());}}; class FilterController final:public LegacyEffectController{public:FilterController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new FilterController());}}; }
#define stringPluginName "LVS Filter"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Filter",FULL_VERSION_STR,kVstVersionString,FilterProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,FilterController::createInstance)
END_FACTORY
