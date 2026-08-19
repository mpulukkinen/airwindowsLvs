#include "EqualizerProcessor.h"
#include "EqualizerController.h"
#include "EqualizerCids.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "LVS Equalizer"
#define FULL_VERSION_STR "0.2.0"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace Steinberg::LVS;

BEGIN_FACTORY_DEF(
    "Lyric Video Studio",
    "https://lyricvideo.studio",
    "mailto:support@lyricvideo.studio")

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kEqualizerProcessorUID),
    PClassInfo::kManyInstances,
    kVstAudioEffectClass,
    stringPluginName,
    Vst::kDistributable,
    LVS_EQUALIZER_CATEGORY,
    FULL_VERSION_STR,
    kVstVersionString,
    EqualizerProcessor::createInstance)

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kEqualizerControllerUID),
    PClassInfo::kManyInstances,
    kVstComponentControllerClass,
    stringPluginName " Controller",
    0,
    "",
    FULL_VERSION_STR,
    kVstVersionString,
    EqualizerController::createInstance)

END_FACTORY
