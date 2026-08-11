#include "PearLiteEqDsp.h"

#include <cmath>

namespace lvs::dsp {

float PearLiteEq::clamp01(float value) noexcept {
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

std::uint32_t PearLiteEq::nextNoise(std::uint32_t value) noexcept {
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    return value;
}

void PearLiteEq::prepare(double sampleRate, std::uint32_t) {
    sampleRate_ = sampleRate > 1.0 ? sampleRate : 44100.0;
    reset();
}

void PearLiteEq::reset() {
    pearA_.fill(0.0);
    pearB_.fill(0.0);
    pearC_.fill(0.0);
    fpdL_ = 0x13579BDFu;
    fpdR_ = 0x2468ACE1u;
}

void PearLiteEq::setHigh(float value) noexcept { high_ = clamp01(value); }
void PearLiteEq::setHighMid(float value) noexcept { highMid_ = clamp01(value); }
void PearLiteEq::setLowMid(float value) noexcept { lowMid_ = clamp01(value); }
void PearLiteEq::setBass(float value) noexcept { bass_ = clamp01(value); }

void PearLiteEq::process(StereoBlock block) noexcept {
    if (!block.left || !block.right || block.frames == 0) return;

    const double overallScale = sampleRate_ / 44100.0;
    const double topLevel = std::sqrt(static_cast<double>(high_) + 0.5);
    pearA_[kLevel] = std::sqrt(static_cast<double>(highMid_) + 0.5);
    pearB_[kLevel] = std::sqrt(static_cast<double>(lowMid_) + 0.5);
    pearC_[kLevel] = std::sqrt(static_cast<double>(bass_) + 0.5);

    const double freqFactor = std::sqrt(overallScale) + (overallScale * 0.5);
    pearA_[kFreq] = std::pow(0.564, freqFactor + 0.85);
    pearB_[kFreq] = std::pow(0.564, freqFactor + 4.1);
    pearC_[kFreq] = std::pow(0.564, freqFactor + 7.1);

    for (std::uint32_t i = 0; i < block.frames; ++i) {
        double inputL = block.left[i];
        double inputR = block.right[i];

        if (std::fabs(inputL) < 1.18e-23) inputL = static_cast<double>(fpdL_) * 1.18e-17;
        if (std::fabs(inputR) < 1.18e-23) inputR = static_cast<double>(fpdR_) * 1.18e-17;

        for (int x = 0; x < kPearMax; x += 4) {
            pearA_[kFigL] = inputL;
            pearA_[kFigR] = inputR;
            pearA_[kSlew] = ((pearA_[kFigL] - pearA_[x]) + pearA_[x + 1]) * pearA_[kFreq] * 0.5;
            pearA_[x] = pearA_[kFigL] = (pearA_[kFreq] * pearA_[kFigL]) + ((1.0 - pearA_[kFreq]) * (pearA_[x] + pearA_[x + 1]));
            pearA_[x + 1] = pearA_[kSlew];
            pearA_[kSlew] = ((pearA_[kFigR] - pearA_[x + 2]) + pearA_[x + 3]) * pearA_[kFreq] * 0.5;
            pearA_[x + 2] = pearA_[kFigR] = (pearA_[kFreq] * pearA_[kFigR]) + ((1.0 - pearA_[kFreq]) * (pearA_[x + 2] + pearA_[x + 3]));
            pearA_[x + 3] = pearA_[kSlew];
            inputL -= pearA_[kFigL];
            inputR -= pearA_[kFigR];

            pearB_[kFigL] = pearA_[kFigL];
            pearB_[kFigR] = pearA_[kFigR];
            pearB_[kSlew] = ((pearB_[kFigL] - pearB_[x]) + pearB_[x + 1]) * pearB_[kFreq] * 0.5;
            pearB_[x] = pearB_[kFigL] = (pearB_[kFreq] * pearA_[kFigL]) + ((1.0 - pearB_[kFreq]) * (pearB_[x] + pearB_[x + 1]));
            pearB_[x + 1] = pearB_[kSlew];
            pearB_[kSlew] = ((pearB_[kFigR] - pearB_[x + 2]) + pearB_[x + 3]) * pearB_[kFreq] * 0.5;
            pearB_[x + 2] = pearB_[kFigR] = (pearB_[kFreq] * pearA_[kFigR]) + ((1.0 - pearB_[kFreq]) * (pearB_[x + 2] + pearB_[x + 3]));
            pearB_[x + 3] = pearB_[kSlew];
            pearA_[kFigL] -= pearB_[kFigL];
            pearA_[kFigR] -= pearB_[kFigR];

            pearC_[kFigL] = pearB_[kFigL];
            pearC_[kFigR] = pearB_[kFigR];
            pearC_[kSlew] = ((pearC_[kFigL] - pearC_[x]) + pearC_[x + 1]) * pearC_[kFreq] * 0.5;
            pearC_[x] = pearC_[kFigL] = (pearC_[kFreq] * pearB_[kFigL]) + ((1.0 - pearC_[kFreq]) * (pearC_[x] + pearC_[x + 1]));
            pearC_[x + 1] = pearC_[kSlew];
            pearC_[kSlew] = ((pearC_[kFigR] - pearC_[x + 2]) + pearC_[x + 3]) * pearC_[kFreq] * 0.5;
            pearC_[x + 2] = pearC_[kFigR] = (pearC_[kFreq] * pearB_[kFigR]) + ((1.0 - pearC_[kFreq]) * (pearC_[x + 2] + pearC_[x + 3]));
            pearC_[x + 3] = pearC_[kSlew];
            pearB_[kFigL] -= pearC_[kFigL];
            pearB_[kFigR] -= pearC_[kFigR];

            inputL *= topLevel;
            inputR *= topLevel;
            inputL += pearA_[kFigL] * pearA_[kLevel];
            inputR += pearA_[kFigR] * pearA_[kLevel];
            inputL += pearB_[kFigL] * pearB_[kLevel];
            inputR += pearB_[kFigR] * pearB_[kLevel];
            inputL += pearC_[kFigL] * pearC_[kLevel];
            inputR += pearC_[kFigR] * pearC_[kLevel];
        }

        fpdL_ = nextNoise(fpdL_);
        fpdR_ = nextNoise(fpdR_);

        block.left[i] = static_cast<float>(inputL);
        block.right[i] = static_cast<float>(inputR);
    }
}

} // namespace lvs::dsp
