#include "ReverbProcessor.h"
#include "ReverbController.h"
#include "ReverbCids.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "LVS Reverb"
#define FULL_VERSION_STR "0.1.0"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace Steinberg::LVS;

BEGIN_FACTORY_DEF(
    "Lyric Video Studio",
    "https://lyricvideo.studio",
    "mailto:support@lyricvideo.studio")

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kReverbProcessorUID),
    PClassInfo::kManyInstances,
    kVstAudioEffectClass,
    stringPluginName,
    Vst::kDistributable,
    LVS_REVERB_CATEGORY,
    FULL_VERSION_STR,
    kVstVersionString,
    ReverbProcessor::createInstance)

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kReverbControllerUID),
    PClassInfo::kManyInstances,
    kVstComponentControllerClass,
    stringPluginName " Controller",
    0,
    "",
    FULL_VERSION_STR,
    kVstVersionString,
    ReverbController::createInstance)

END_FACTORY
