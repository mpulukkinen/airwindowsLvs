#pragma once

#include "LvsAudioProcessor.h"

#include <array>
#include <cstdint>

namespace lvs::dsp {

class PearLiteEq final : public AudioProcessor {
public:
    void prepare(double sampleRate, std::uint32_t maxBlockSize) override;
    void reset() override;
    void process(StereoBlock block) noexcept override;

    void setHigh(float value) noexcept;
    void setHighMid(float value) noexcept;
    void setLowMid(float value) noexcept;
    void setBass(float value) noexcept;

private:
    static constexpr int kPearMax = 32;
    static constexpr int kFigL = 32;
    static constexpr int kFigR = 33;
    static constexpr int kGndL = 34;
    static constexpr int kGndR = 35;
    static constexpr int kSlew = 36;
    static constexpr int kFreq = 37;
    static constexpr int kLevel = 38;
    static constexpr int kPearTotal = 39;

    static float clamp01(float value) noexcept;
    static std::uint32_t nextNoise(std::uint32_t value) noexcept;

    double sampleRate_{44100.0};
    float high_{0.5f};
    float highMid_{0.5f};
    float lowMid_{0.5f};
    float bass_{0.5f};

    std::array<double, kPearTotal> pearA_{};
    std::array<double, kPearTotal> pearB_{};
    std::array<double, kPearTotal> pearC_{};
    std::uint32_t fpdL_{0x13579BDFu};
    std::uint32_t fpdR_{0x2468ACE1u};
};

} // namespace lvs::dsp
