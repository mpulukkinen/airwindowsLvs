#include "TapeDelay2Dsp.h"

#include "TapeDelay2.h"

#include <algorithm>

namespace lvs::dsp {

struct TapeDelay2Dsp::Impl {
    ::TapeDelay2 legacy{nullptr};
    float postGain{1.0f};
};

TapeDelay2Dsp::TapeDelay2Dsp() : impl_(std::make_unique<Impl>()) {
    applyParameters();
}

TapeDelay2Dsp::~TapeDelay2Dsp() = default;

float TapeDelay2Dsp::clamp01(float value) noexcept {
    return std::clamp(value, 0.0f, 1.0f);
}

void TapeDelay2Dsp::prepare(double sampleRate, std::uint32_t) {
    sampleRate_ = sampleRate > 1.0 ? sampleRate : 44100.0;
    impl_->legacy.setSampleRate(static_cast<float>(sampleRate_));
    applyParameters();
}

void TapeDelay2Dsp::reset() {
    impl_ = std::make_unique<Impl>();
    impl_->legacy.setSampleRate(static_cast<float>(sampleRate_));
    applyParameters();
}

void TapeDelay2Dsp::setTime(float value) noexcept {
    time_ = clamp01(value);
    applyParameters();
}

void TapeDelay2Dsp::setFeedback(float value) noexcept {
    feedback_ = clamp01(value);
    applyParameters();
}

void TapeDelay2Dsp::setTone(float value) noexcept {
    tone_ = clamp01(value);
    applyParameters();
}

void TapeDelay2Dsp::setFlutter(float value) noexcept {
    flutter_ = clamp01(value);
    applyParameters();
}

void TapeDelay2Dsp::setMix(float value) noexcept {
    mix_ = clamp01(value);
    applyParameters();
}

void TapeDelay2Dsp::applyParameters() noexcept {
    if (!impl_)
        return;

    // TapeDelay2's raw Time parameter runs opposite to the convention users
    // expect: larger A means a faster read head and therefore a shorter delay.
    impl_->legacy.setParameter(kParamA, 1.0f - time_);
    impl_->legacy.setParameter(kParamB, feedback_);
    impl_->legacy.setParameter(kParamC, tone_);
    impl_->legacy.setParameter(kParamD, 0.0f); // keep the basic plugin neutral
    impl_->legacy.setParameter(kParamE, flutter_);

    // Airwindows TapeDelay2 uses a send-friendly mix law: at raw 0.5 both dry
    // and wet are unity. Convert our conventional linear crossfade to the raw
    // control plus a post gain so 0/50/100% mean dry/equal/wet as expected.
    float rawMix = 0.0f;
    if (mix_ <= 0.5f) {
        rawMix = mix_ <= 0.0f ? 0.0f : mix_ / (2.0f * (1.0f - mix_));
        impl_->postGain = 1.0f - mix_;
    } else {
        rawMix = 1.0f - ((1.0f - mix_) / (2.0f * mix_));
        impl_->postGain = mix_;
    }
    impl_->legacy.setParameter(kParamF, clamp01(rawMix));
}

void TapeDelay2Dsp::process(StereoBlock block) noexcept {
    if (!impl_ || !block.left || !block.right || block.frames == 0)
        return;

    float* inputs[2]{block.left, block.right};
    float* outputs[2]{block.left, block.right};
    impl_->legacy.processReplacing(inputs, outputs, static_cast<VstInt32>(block.frames));

    if (impl_->postGain != 1.0f) {
        for (std::uint32_t i = 0; i < block.frames; ++i) {
            block.left[i] *= impl_->postGain;
            block.right[i] *= impl_->postGain;
        }
    }
}

} // namespace lvs::dsp
