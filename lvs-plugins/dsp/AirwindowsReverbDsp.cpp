#include "AirwindowsReverbDsp.h"

#include "Reverb.h"

#include <algorithm>

namespace lvs::dsp {

struct AirwindowsReverbDsp::Impl {
    ::Reverb legacy{nullptr};
};

AirwindowsReverbDsp::AirwindowsReverbDsp() : impl_(std::make_unique<Impl>()) {
    applyParameters();
}

AirwindowsReverbDsp::~AirwindowsReverbDsp() = default;

float AirwindowsReverbDsp::clamp01(float value) noexcept {
    return std::clamp(value, 0.0f, 1.0f);
}

void AirwindowsReverbDsp::prepare(double sampleRate, std::uint32_t) {
    sampleRate_ = sampleRate > 1.0 ? sampleRate : 44100.0;
    impl_->legacy.setSampleRate(static_cast<float>(sampleRate_));
    applyParameters();
}

void AirwindowsReverbDsp::reset() {
    impl_ = std::make_unique<Impl>();
    impl_->legacy.setSampleRate(static_cast<float>(sampleRate_));
    applyParameters();
}

void AirwindowsReverbDsp::setSize(float value) noexcept {
    size_ = clamp01(value);
    applyParameters();
}

void AirwindowsReverbDsp::setMix(float value) noexcept {
    mix_ = clamp01(value);
    applyParameters();
}

void AirwindowsReverbDsp::applyParameters() noexcept {
    if (!impl_)
        return;
    impl_->legacy.setParameter(kParamA, size_);
    impl_->legacy.setParameter(kParamB, mix_);
}

void AirwindowsReverbDsp::process(StereoBlock block) noexcept {
    if (!impl_ || !block.left || !block.right || block.frames == 0)
        return;

    float* inputs[2]{block.left, block.right};
    float* outputs[2]{block.left, block.right};
    impl_->legacy.processReplacing(inputs, outputs, static_cast<VstInt32>(block.frames));
}

} // namespace lvs::dsp
