#pragma once

#include "LvsAudioProcessor.h"

#include <array>
#include <cstdint>

namespace lvs::dsp {

// DSP adaptation of Airwindows TapeDelay2 (MIT), with an LVS-facing
// conventional dry/wet crossfade and intuitive short->long time control.
class TapeDelay2 final : public AudioProcessor {
public:
    void prepare(double sampleRate, std::uint32_t maxBlockSize) override;
    void reset() override;
    void process(StereoBlock block) noexcept override;

    void setTime(float value) noexcept;
    void setFeedback(float value) noexcept;
    void setTone(float value) noexcept;
    void setResonance(float value) noexcept;
    void setFlutter(float value) noexcept;
    void setMix(float value) noexcept;

private:
    static float clamp01(float value) noexcept;
    static std::uint32_t nextNoise(std::uint32_t value) noexcept;

    double sampleRate_{44100.0};
    float time_{0.5f};
    float feedback_{0.25f};
    float tone_{0.5f};
    float resonance_{0.0f};
    float flutter_{0.0f};
    float mix_{0.35f};

    std::array<double, 88211> delayBufferL_{};
    std::array<double, 88211> delayBufferR_{};
    double prevSampleL_{0.0};
    double delayL_{0.0};
    double sweepL_{0.0};
    double prevSampleR_{0.0};
    double delayR_{0.0};
    double sweepR_{0.0};
    std::array<double, 9> regenFilterL_{};
    std::array<double, 9> regenFilterR_{};
    std::array<double, 9> outFilterL_{};
    std::array<double, 9> outFilterR_{};
    std::array<double, 10> lastRefL_{};
    std::array<double, 10> lastRefR_{};
    int cycle_{0};
    std::uint32_t fpdL_{0x13579BDFu};
    std::uint32_t fpdR_{0x2468ACE1u};
};

} // namespace lvs::dsp
