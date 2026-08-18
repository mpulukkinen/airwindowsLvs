#include "DelayProcessor.h"
#include "DelayController.h"
#include "DelayCids.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "LVS Delay"
#define FULL_VERSION_STR "0.2.0"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace Steinberg::LVS;

BEGIN_FACTORY_DEF(
    "Lyric Video Studio",
    "https://lyricvideo.studio",
    "mailto:support@lyricvideo.studio")

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kDelayProcessorUID),
    PClassInfo::kManyInstances,
    kVstAudioEffectClass,
    stringPluginName,
    Vst::kDistributable,
    LVS_DELAY_CATEGORY,
    FULL_VERSION_STR,
    kVstVersionString,
    DelayProcessor::createInstance)

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kDelayControllerUID),
    PClassInfo::kManyInstances,
    kVstComponentControllerClass,
    stringPluginName " Controller",
    0,
    "",
    FULL_VERSION_STR,
    kVstVersionString,
    DelayController::createInstance)

END_FACTORY
