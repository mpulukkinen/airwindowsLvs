#pragma once

#include "LvsAudioProcessor.h"

#include <cstdint>

namespace lvs::dsp {

// DSP adaptation of Airwindows ButterComp2 (MIT).
// The VST wrapper intentionally stays independent of the legacy Airwindows VST2 class.
class ButterComp2 final : public AudioProcessor {
public:
    void prepare(double sampleRate, std::uint32_t maxBlockSize) override;
    void reset() override;
    void process(StereoBlock block) noexcept override;

    void setCompress(float value) noexcept;
    void setOutput(float value) noexcept;
    void setMix(float value) noexcept;

private:
    static float clamp01(float value) noexcept;
    static std::uint32_t nextNoise(std::uint32_t value) noexcept;
    static double denormalNoise(int& source) noexcept;

    double sampleRate_{44100.0};
    float compress_{0.0f};
    float output_{0.5f};
    float mix_{1.0f};

    double controlAposL_{1.0};
    double controlAnegL_{1.0};
    double controlBposL_{1.0};
    double controlBnegL_{1.0};
    double targetPosL_{1.0};
    double targetNegL_{1.0};
    double lastOutputL_{0.0};

    double controlAposR_{1.0};
    double controlAnegR_{1.0};
    double controlBposR_{1.0};
    double controlBnegR_{1.0};
    double targetPosR_{1.0};
    double targetNegR_{1.0};
    double lastOutputR_{0.0};

    bool flip_{false};
    int noiseSourceL_{0};
    int noiseSourceR_{850010};
    std::uint32_t fpdL_{0x13579BDFu};
    std::uint32_t fpdR_{0x2468ACE1u};
};

} // namespace lvs::dsp
