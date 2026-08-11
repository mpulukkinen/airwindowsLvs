#pragma once

#include <cstdint>
#include <span>

namespace lvs {

struct StereoBlock {
    float* left{};
    float* right{};
    std::uint32_t frames{};
};

class AudioProcessor {
public:
    virtual ~AudioProcessor() = default;

    virtual void prepare(double sampleRate, std::uint32_t maxBlockSize) = 0;
    virtual void reset() = 0;
    virtual void process(StereoBlock block) noexcept = 0;
};

// Stable IDs are part of the plugin's public contract. Once a released plugin
// uses an ID, do not recycle it for a different parameter: DAW automation and
// saved projects may depend on it.
using ParameterId = std::uint32_t;

} // namespace lvs
