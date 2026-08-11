#pragma once

// Minimal compatibility shim for compiling selected legacy Airwindows VST2 DSP
// sources as private implementation code. This is NOT a VST2 host/plugin API and
// is never exposed by the LVS VST3 binaries.

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

using VstInt32 = std::int32_t;
using audioMasterCallback = void*;

enum VstPlugCategory {
    kPlugCategUnknown = 0,
    kPlugCategEffect = 1
};

inline constexpr int kVstMaxProgNameLen = 24;
inline constexpr int kVstMaxParamStrLen = 32;
inline constexpr int kVstMaxProductStrLen = 64;
inline constexpr int kVstMaxVendorStrLen = 64;

inline void vst_strncpy(char* dest, const char* src, int maxLen) {
    if (!dest || maxLen <= 0)
        return;
    std::strncpy(dest, src ? src : "", static_cast<std::size_t>(maxLen));
    dest[maxLen] = '\0';
}

inline void float2string(float value, char* text, int maxLen) {
    if (!text || maxLen <= 0)
        return;
    std::snprintf(text, static_cast<std::size_t>(maxLen) + 1, "%.6g", value);
}

inline void double2string(double value, char* text, int maxLen) {
    if (!text || maxLen <= 0)
        return;
    std::snprintf(text, static_cast<std::size_t>(maxLen) + 1, "%.6g", value);
}

inline void int2string(VstInt32 value, char* text, int maxLen) {
    if (!text || maxLen <= 0)
        return;
    std::snprintf(text, static_cast<std::size_t>(maxLen) + 1, "%d", value);
}

class AudioEffectX {
public:
    AudioEffectX(audioMasterCallback, VstInt32, VstInt32) {}
    virtual ~AudioEffectX() = default;

    virtual void processReplacing(float**, float**, VstInt32) {}
    virtual void processDoubleReplacing(double**, double**, VstInt32) {}

    void setNumInputs(VstInt32) {}
    void setNumOutputs(VstInt32) {}
    void setUniqueID(unsigned long) {}
    void canProcessReplacing() {}
    void canDoubleReplacing() {}
    void programsAreChunks(bool) {}
    void setInitialDelay(VstInt32) {}

    float getSampleRate() const noexcept { return sampleRate_; }
    void setSampleRate(float sampleRate) noexcept {
        sampleRate_ = sampleRate > 1.0f ? sampleRate : 44100.0f;
    }

private:
    float sampleRate_{44100.0f};
};

using AudioEffect = AudioEffectX;
