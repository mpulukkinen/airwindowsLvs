#include "../../common/LegacyAirwindowsVst3.h"
#include "StereoDoubler.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0xA0BC5F02,0x2A4176B8,0xB92CC7EB,0x621A75BD); const FUID kControllerUID(0x223BF840,0x01772742,0x710381F8,0xD73157CE); const LegacyParameterSpec kParameters[]={{STR16("Detune"),1000,0,0.6,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix"),1001,1,0.6,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::StereoDoubler(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,2,nullptr,0,&createLegacyEffect,"Doubler.uidesc"}; } class DoublerProcessor final:public LegacyEffectProcessor{public:DoublerProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new DoublerProcessor());}}; class DoublerController final:public LegacyEffectController{public:DoublerController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new DoublerController());}}; }
#define stringPluginName "LVS Doubler"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Modulation",FULL_VERSION_STR,kVstVersionString,DoublerProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,DoublerController::createInstance)
END_FACTORY
