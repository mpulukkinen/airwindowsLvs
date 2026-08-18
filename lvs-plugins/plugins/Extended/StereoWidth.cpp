#include "../../common/LegacyAirwindowsVst3.h"
#include "Wider.h"
#include "public.sdk/source/main/pluginfactory.h"
namespace Steinberg::LVS { namespace { const FUID kProcessorUID(0xEDA45848,0x3B86B8D7,0x9C2D7856,0x55DD7DFF); const FUID kControllerUID(0x95446F68,0xE20B8649,0x615D2336,0x350502AF); const LegacyParameterSpec kParameters[]={{STR16("Width"),1000,0,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Center"),1001,1,0.5,0,LegacyParameterTarget::Legacy,mapDirect},{STR16("Mix"),1002,2,1.0,0,LegacyParameterTarget::Legacy,mapDirect}}; AudioEffectX* createLegacyEffect(){return new ::Wider(nullptr);} const LegacyPluginSpec kPluginSpec{&kControllerUID,kParameters,3,nullptr,0,&createLegacyEffect,"StereoWidth.uidesc"}; } class StereoWidthProcessor final:public LegacyEffectProcessor{public:StereoWidthProcessor():LegacyEffectProcessor(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IAudioProcessor*>(new StereoWidthProcessor());}}; class StereoWidthController final:public LegacyEffectController{public:StereoWidthController():LegacyEffectController(kPluginSpec){} static FUnknown* createInstance(void*){return static_cast<Vst::IEditController*>(new StereoWidthController());}}; }
#define stringPluginName "LVS Stereo Width"
#define FULL_VERSION_STR "0.2.0"
using namespace Steinberg; using namespace Steinberg::Vst; using namespace Steinberg::LVS; BEGIN_FACTORY_DEF("Lyric Video Studio","https://lyricvideo.studio","mailto:support@lyricvideo.studio")
DEF_CLASS2(INLINE_UID_FROM_FUID(kProcessorUID),PClassInfo::kManyInstances,kVstAudioEffectClass,stringPluginName,Vst::kDistributable,"Fx|Spatial",FULL_VERSION_STR,kVstVersionString,StereoWidthProcessor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(kControllerUID),PClassInfo::kManyInstances,kVstComponentControllerClass,stringPluginName " Controller",0,"",FULL_VERSION_STR,kVstVersionString,StereoWidthController::createInstance)
END_FACTORY
