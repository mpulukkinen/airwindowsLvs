#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg::LVS {

enum ReverbParams : Vst::ParamID {
    kReverbBypassId = 100,
    kReverbSizeId = 4000,
    kReverbMixId = 4001
};

static const FUID kReverbProcessorUID(0x4C565352, 0x45564231, 0x93A4D721, 0x6B5C8E10);
static const FUID kReverbControllerUID(0x4C565352, 0x45564331, 0xAF72C491, 0x35D86B20);

#define LVS_REVERB_CATEGORY "Fx|Reverb"

} // namespace Steinberg::LVS
