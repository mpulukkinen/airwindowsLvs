#include "ButterComp2Dsp.h"

#include <cmath>

namespace lvs::dsp {

float ButterComp2::clamp01(float value) noexcept {
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

std::uint32_t ButterComp2::nextNoise(std::uint32_t value) noexcept {
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    return value;
}

double ButterComp2::denormalNoise(int& source) noexcept {
    source = source % 1700021;
    ++source;
    int residue = source * source;
    residue %= 170003;
    residue *= residue;
    residue %= 17011;
    residue *= residue;
    residue %= 1709;
    residue *= residue;
    residue %= 173;
    residue *= residue;
    residue %= 17;
    return static_cast<double>(residue) * 1.0e-16;
}

void ButterComp2::prepare(double sampleRate, std::uint32_t) {
    sampleRate_ = sampleRate > 1.0 ? sampleRate : 44100.0;
    reset();
}

void ButterComp2::reset() {
    controlAposL_ = controlAnegL_ = controlBposL_ = controlBnegL_ = 1.0;
    targetPosL_ = targetNegL_ = 1.0;
    lastOutputL_ = 0.0;

    controlAposR_ = controlAnegR_ = controlBposR_ = controlBnegR_ = 1.0;
    targetPosR_ = targetNegR_ = 1.0;
    lastOutputR_ = 0.0;

    flip_ = false;
    noiseSourceL_ = 0;
    noiseSourceR_ = 850010;
    fpdL_ = 0x13579BDFu;
    fpdR_ = 0x2468ACE1u;
}

void ButterComp2::setCompress(float value) noexcept { compress_ = clamp01(value); }
void ButterComp2::setOutput(float value) noexcept { output_ = clamp01(value); }
void ButterComp2::setMix(float value) noexcept { mix_ = clamp01(value); }

void ButterComp2::process(StereoBlock block) noexcept {
    if (!block.left || !block.right || block.frames == 0)
        return;

    const double overallScale = sampleRate_ / 44100.0;
    const double inputGain = std::pow(10.0, (static_cast<double>(compress_) * 14.0) / 20.0);
    const double compFactor = 0.012 * (static_cast<double>(compress_) / 135.0);
    const double output = static_cast<double>(output_) * 2.0;
    const double wet = static_cast<double>(mix_);
    const double outputGain = ((inputGain - 1.0) / 1.5) + 1.0;

    for (std::uint32_t i = 0; i < block.frames; ++i) {
        double inputL = block.left[i];
        double inputR = block.right[i];

        const double residueL = denormalNoise(noiseSourceL_);
        const double residueR = denormalNoise(noiseSourceR_);
        inputL += residueL;
        inputR += residueR;
        if (std::fabs(inputL) < 1.2e-38) inputL -= residueL;
        if (std::fabs(inputR) < 1.2e-38) inputR -= residueR;

        const double dryL = inputL;
        const double dryR = inputR;

        inputL *= inputGain;
        inputR *= inputGain;

        double divisorL = compFactor / (1.0 + std::fabs(lastOutputL_));
        divisorL /= overallScale;
        const double remainderL = divisorL;
        divisorL = 1.0 - divisorL;

        double inputPosL = inputL + 1.0;
        if (inputPosL < 0.0) inputPosL = 0.0;
        double outputPosL = inputPosL / 2.0;
        if (outputPosL > 1.0) outputPosL = 1.0;
        inputPosL *= inputPosL;
        targetPosL_ = (targetPosL_ * divisorL) + (inputPosL * remainderL);
        const double calcPosL = std::pow(1.0 / targetPosL_, 2.0);

        double inputNegL = (-inputL) + 1.0;
        if (inputNegL < 0.0) inputNegL = 0.0;
        double outputNegL = inputNegL / 2.0;
        if (outputNegL > 1.0) outputNegL = 1.0;
        inputNegL *= inputNegL;
        targetNegL_ = (targetNegL_ * divisorL) + (inputNegL * remainderL);
        const double calcNegL = std::pow(1.0 / targetNegL_, 2.0);

        if (inputL > 0.0) {
            if (flip_) controlAposL_ = (controlAposL_ * divisorL) + (calcPosL * remainderL);
            else controlBposL_ = (controlBposL_ * divisorL) + (calcPosL * remainderL);
        } else {
            if (flip_) controlAnegL_ = (controlAnegL_ * divisorL) + (calcNegL * remainderL);
            else controlBnegL_ = (controlBnegL_ * divisorL) + (calcNegL * remainderL);
        }

        double divisorR = compFactor / (1.0 + std::fabs(lastOutputR_));
        divisorR /= overallScale;
        const double remainderR = divisorR;
        divisorR = 1.0 - divisorR;

        double inputPosR = inputR + 1.0;
        if (inputPosR < 0.0) inputPosR = 0.0;
        double outputPosR = inputPosR / 2.0;
        if (outputPosR > 1.0) outputPosR = 1.0;
        inputPosR *= inputPosR;
        targetPosR_ = (targetPosR_ * divisorR) + (inputPosR * remainderR);
        const double calcPosR = std::pow(1.0 / targetPosR_, 2.0);

        double inputNegR = (-inputR) + 1.0;
        if (inputNegR < 0.0) inputNegR = 0.0;
        double outputNegR = inputNegR / 2.0;
        if (outputNegR > 1.0) outputNegR = 1.0;
        inputNegR *= inputNegR;
        targetNegR_ = (targetNegR_ * divisorR) + (inputNegR * remainderR);
        const double calcNegR = std::pow(1.0 / targetNegR_, 2.0);

        if (inputR > 0.0) {
            if (flip_) controlAposR_ = (controlAposR_ * divisorR) + (calcPosR * remainderR);
            else controlBposR_ = (controlBposR_ * divisorR) + (calcPosR * remainderR);
        } else {
            if (flip_) controlAnegR_ = (controlAnegR_ * divisorR) + (calcNegR * remainderR);
            else controlBnegR_ = (controlBnegR_ * divisorR) + (calcNegR * remainderR);
        }

        double multiplierL;
        double multiplierR;
        if (flip_) {
            multiplierL = (controlAposL_ * outputPosL) + (controlAnegL_ * outputNegL);
            multiplierR = (controlAposR_ * outputPosR) + (controlAnegR_ * outputNegR);
        } else {
            multiplierL = (controlBposL_ * outputPosL) + (controlBnegL_ * outputNegL);
            multiplierR = (controlBposR_ * outputPosR) + (controlBnegR_ * outputNegR);
        }

        inputL = (inputL * multiplierL) / outputGain;
        inputR = (inputR * multiplierR) / outputGain;

        if (output != 1.0) {
            inputL *= output;
            inputR *= output;
        }

        if (wet != 1.0) {
            inputL = (inputL * wet) + (dryL * (1.0 - wet));
            inputR = (inputR * wet) + (dryR * (1.0 - wet));
        }

        lastOutputL_ = inputL;
        lastOutputR_ = inputR;
        flip_ = !flip_;

        int exponent = 0;
        std::frexp(static_cast<float>(inputL), &exponent);
        fpdL_ = nextNoise(fpdL_);
        inputL += (static_cast<double>(fpdL_) - 2147483647.0) * 5.5e-36 * std::pow(2.0, exponent + 62);
        std::frexp(static_cast<float>(inputR), &exponent);
        fpdR_ = nextNoise(fpdR_);
        inputR += (static_cast<double>(fpdR_) - 2147483647.0) * 5.5e-36 * std::pow(2.0, exponent + 62);

        block.left[i] = static_cast<float>(inputL);
        block.right[i] = static_cast<float>(inputR);
    }
}

} // namespace lvs::dsp
