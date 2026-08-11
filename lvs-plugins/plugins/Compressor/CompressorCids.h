#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
namespace LVS {

enum CompressorParams : Vst::ParamID {
    kCompressorBypassId = 100,
    kCompressId = 2000,
    kCompressorOutputId = 2001,
    kCompressorMixId = 2002
};

static const FUID kCompressorProcessorUID(0x4C565343, 0x4F4D5031, 0x9E5A6B14, 0xA2374C81);
static const FUID kCompressorControllerUID(0x4C565343, 0x4F4D4331, 0xB720D39A, 0x53E18F42);

#define LVS_COMPRESSOR_CATEGORY "Fx|Dynamics"

} // namespace LVS
} // namespace Steinberg
