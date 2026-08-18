#include "../../common/LegacyAirwindowsVst3.h"
#include "DeBess.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace {
const FUID kProcessorUID(0x87FCBA73,0x2A07CD24,0xC8738334,0x01AFBFC7); const FUID kControllerUID(0xBF509750,0x443F5DBD,0xCEF7ADE0,0xAC45A540);
const LegacyParameterSpec kParameters[]={{STR16("Intensity"),1000,0,0.0,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Sharpness"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Depth"),1002,2,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Filter"),1003,3,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mode"),1004,4,0.0,1,LegacyParameterTarget::Legacy,mapDirect}};
AudioEffectX* createLegacyEffect(){return new ::DeBess(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,5,nullptr,0,&createLegacyEffect,"DeEsser.uidesc"}; }
class DeEsserProcessor final:public LegacyEffectProcessor{public:DeEsserProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new DeEsserProcessor());}}; class DeEsserController final:public LegacyEffectController{public:DeEsserController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new DeEsserController());}}; }
#define stringPluginName "LVS De-Esser"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Dynamics",FULL_VERSION_STR,kVstVersionString,DeEsserProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,DeEsserController::createInstance)
END_FACTORY
