#include "CompressorProcessor.h"
#include "CompressorController.h"
#include "CompressorCids.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "LVS Compressor"
#define FULL_VERSION_STR "0.2.0"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace Steinberg::LVS;

BEGIN_FACTORY_DEF(
    "Lyric Video Studio",
    "https://lyricvideo.studio",
    "mailto:support@lyricvideo.studio")

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kCompressorProcessorUID),
    PClassInfo::kManyInstances,
    kVstAudioEffectClass,
    stringPluginName,
    Vst::kDistributable,
    LVS_COMPRESSOR_CATEGORY,
    FULL_VERSION_STR,
    kVstVersionString,
    CompressorProcessor::createInstance)

DEF_CLASS2(
    INLINE_UID_FROM_FUID(kCompressorControllerUID),
    PClassInfo::kManyInstances,
    kVstComponentControllerClass,
    stringPluginName " Controller",
    0,
    "",
    FULL_VERSION_STR,
    kVstVersionString,
    CompressorController::createInstance)

END_FACTORY
