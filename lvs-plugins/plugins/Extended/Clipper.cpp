#include "../../common/LegacyAirwindowsVst3.h"

#include "ADClip9.h"
#include "public.sdk/source/main/pluginfactory.h"

namespace Steinberg::LVS {
namespace {
const FUID kProcessorUID(0xEB53B45A, 0x7E2F5F60, 0x77CD4DC0, 0xAA5E392E);
const FUID kControllerUID(0x819C0D3C, 0x29666D3C, 0x328E09E3, 0x34A01B4B);
const LegacyParameterSpec kParameters[] = {
    {STR16("Boost"),1000,0,0.0,0,LegacyParameterTarget::Legacy,mapDirect},
    {STR16("Match"),1001,1,0.0,0,LegacyParameterTarget::Legacy,mapDirect},
    {STR16("Noise"),1002,2,0.7,0,LegacyParameterTarget::Legacy,mapDirect},
    {STR16("Ceiling"),1003,3,0.75,0,LegacyParameterTarget::Legacy,mapDirect},
    {STR16("Mode"),1004,4,0.0,2,LegacyParameterTarget::Legacy,mapDirect},
};
AudioEffectX* createLegacyEffect(){return new ::ADClip9(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,5,nullptr,0,&createLegacyEffect,"Clipper.uidesc"};
}
class ClipperProcessor final:public LegacyEffectProcessor{public:ClipperProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new ClipperProcessor());}};
class ClipperController final:public LegacyEffectController{public:ClipperController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new ClipperController());}};
}
#define stringPluginName "LVS Clipper"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS;
BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Dynamics",FULL_VERSION_STR,kVstVersionString,ClipperProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,ClipperController::createInstance)
END_FACTORY
