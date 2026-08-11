#pragma once

#include "LvsAudioProcessor.h"

#include <cstdint>
#include <memory>

namespace lvs::dsp {

// Thin adapter around the original Airwindows TapeDelay2 processing code.
// The legacy VST2-facing class is compiled privately behind our compatibility
// shim; the LVS VST3 layer sees only this host-agnostic DSP interface.
class TapeDelay2Dsp final : public AudioProcessor {
public:
    TapeDelay2Dsp();
    ~TapeDelay2Dsp() override;

    TapeDelay2Dsp(const TapeDelay2Dsp&) = delete;
    TapeDelay2Dsp& operator=(const TapeDelay2Dsp&) = delete;

    void prepare(double sampleRate, std::uint32_t maxBlockSize) override;
    void reset() override;
    void process(StereoBlock block) noexcept override;

    void setTime(float value) noexcept;
    void setFeedback(float value) noexcept;
    void setTone(float value) noexcept;
    void setFlutter(float value) noexcept;
    void setMix(float value) noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    double sampleRate_{44100.0};
    float time_{0.35f};
    float feedback_{0.25f};
    float tone_{0.5f};
    float flutter_{0.0f};
    float mix_{0.25f};

    static float clamp01(float value) noexcept;
    void applyParameters() noexcept;
};

} // namespace lvs::dsp
