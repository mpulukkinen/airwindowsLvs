#pragma once

#include "LvsAudioProcessor.h"

#include <cstdint>
#include <memory>

namespace lvs::dsp {

// Adapter around Airwindows Reverb: the deliberately simple, tuned MatrixVerb
// variant. Public LVS controls remain Size + Mix while the original algorithm
// stays private implementation detail.
class AirwindowsReverbDsp final : public AudioProcessor {
public:
    AirwindowsReverbDsp();
    ~AirwindowsReverbDsp() override;

    AirwindowsReverbDsp(const AirwindowsReverbDsp&) = delete;
    AirwindowsReverbDsp& operator=(const AirwindowsReverbDsp&) = delete;

    void prepare(double sampleRate, std::uint32_t maxBlockSize) override;
    void reset() override;
    void process(StereoBlock block) noexcept override;

    void setSize(float value) noexcept;
    void setMix(float value) noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    double sampleRate_{44100.0};
    float size_{0.5f};
    float mix_{0.25f};

    static float clamp01(float value) noexcept;
    void applyParameters() noexcept;
};

} // namespace lvs::dsp
