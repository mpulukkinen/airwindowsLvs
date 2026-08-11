#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg::LVS {

enum DelayParams : Vst::ParamID {
    kDelayBypassId = 100,
    kDelayTimeId = 3000,
    kDelayFeedbackId = 3001,
    kDelayToneId = 3002,
    kDelayFlutterId = 3003,
    kDelayMixId = 3004
};

static const FUID kDelayProcessorUID(0x4C565344, 0x454C5931, 0xA159C204, 0x7D3E8F61);
static const FUID kDelayControllerUID(0x4C565344, 0x454C4331, 0xB2719A35, 0x5E4D8C02);

#define LVS_DELAY_CATEGORY "Fx|Delay"

} // namespace Steinberg::LVS
