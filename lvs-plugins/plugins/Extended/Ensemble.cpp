#include "../../common/LegacyAirwindowsVst3.h"
#include "StereoEnsemble.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0x7E85BBF8,0x9EB8B486,0x3E1B78FF,0x2D7A4C76); const FUID kControllerUID(0x79830059,0x73EC2212,0x23902AF4,0x65A580F7); const LegacyParameterSpec kParameters[]={{STR16("Depth"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Effect Level"),1001,1,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::StereoEnsemble(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,2,nullptr,0,&createLegacyEffect,"Ensemble.uidesc"}; } class EnsembleProcessor final:public LegacyEffectProcessor{public:EnsembleProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new EnsembleProcessor());}}; class EnsembleController final:public LegacyEffectController{public:EnsembleController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new EnsembleController());}}; }
#define stringPluginName "LVS Ensemble"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Modulation",FULL_VERSION_STR,kVstVersionString,EnsembleProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,EnsembleController::createInstance)
END_FACTORY
