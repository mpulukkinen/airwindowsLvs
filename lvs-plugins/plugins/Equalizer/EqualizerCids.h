#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
namespace LVS {

enum EqualizerParams : Vst::ParamID {
    kBypassId = 100,
    kHighId = 1000,
    kHighMidId = 1001,
    kLowMidId = 1002,
    kBassId = 1003
};

static const FUID kEqualizerProcessorUID(0x4C565345, 0x51445031, 0xA14C782B, 0x19B05C11);
static const FUID kEqualizerControllerUID(0x4C565345, 0x51444331, 0xBB91D0E4, 0x4D82072A);

#define LVS_EQUALIZER_CATEGORY "Fx|EQ"

} // namespace LVS
} // namespace Steinberg
