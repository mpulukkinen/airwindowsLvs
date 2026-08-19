# LVS Audio Suite

A Windows x64 VST3 effect suite for **Lyric Video Studio**, built around selected MIT-licensed Airwindows DSP algorithms with LVS-owned VST3 identities, automation/state handling, parameter mapping and independent VSTGUI editors.

Version 0.2.0 expands the original four essentials into a **24-plugin audio toolkit** for music, vocals, video production and creative sound design.

## Plugins

| LVS plugin | Airwindows DSP | Main controls |
| --- | --- | --- |
| **LVS Equalizer** | PearLiteEQ | High, High Mid, Low Mid, Bass |
| **LVS Compressor** | ButterComp2 | Compression, Output, Mix |
| **LVS Delay** | TapeDelay2 | Time, Feedback, Tone, Flutter, Mix |
| **LVS Reverb** | Reverb | Size, Mix |
| **LVS Limiter** | Recurve | Drive, Output, Mix |
| **LVS Clipper** | ADClip9 | Boost, Match, Noise, Ceiling, Mode |
| **LVS De-Esser** | DeBess | Intensity, Sharpness, Depth, Filter, Mode |
| **LVS Gate** | SoftGate | Threshold, Darken, Silence |
| **LVS Chorus** | StereoChorus | Speed, Depth, Mix |
| **LVS Stereo Width** | Wider | Width, Center, Mix |
| **LVS Saturation** | Density3 | Drive, Highpass, Output, Mix |
| **LVS Tape** | ToTape9 | Input, Tilt, Shape, Flutter, Flutter Speed, Bias, Head Bump, Head Freq, Output |
| **LVS Doubler** | StereoDoubler | Detune, Mix |
| **LVS Filter** | BiquadDouble | Type, Frequency, Q, Mix / Invert |
| **LVS Tremolo** | Tremolo | Speed, Depth |
| **LVS Auto Pan** | AutoPan | Rate, Phase, Width, Mix |
| **LVS Exciter** | Air3 | Air, Ground |
| **LVS Distortion** | Drive | Drive, Highpass, Output, Mix |
| **LVS Vocal Leveler** | Podcast | Boost, Mix |
| **LVS Ensemble** | StereoEnsemble | Depth, Effect Level |
| **LVS Pitch Delay** | PitchDelay | Time, Feedback, Tone, Resonance, Pitch, Mix |
| **LVS Ring Modulator** | RingModulator | Frequency A, Frequency B, Soar, Mix |
| **LVS Lo-Fi** | DeRez4 | Downsample, Brightness, Bass, Output |
| **LVS Guitar Amp** | FireAmp | Gain, Tone, Output, Mix |

Every plugin also exposes a standard VST3 bypass parameter.

## Design

The public plugins are **LVS VST3 plugins**. Airwindows supplies the DSP implementation layer, not the public plugin identity or UI.

The original four plugins keep their existing processor/controller IDs for project compatibility. The expanded effects use a shared `LegacyAirwindowsVst3` adapter that provides:

- stable LVS VST3 component identities
- automation and state persistence
- explicit stereo-only bus negotiation
- 32-bit floating-point processing
- standard VST3 bypass
- independent VSTGUI editor windows
- optional LVS-side input/output gain and conventional wet/dry mixing
- clean DSP reset when a plugin is activated

`legacy/audioeffectx.h` is a minimal private compatibility shim used only to compile selected Airwindows VST2-era source as DSP implementation. **No VST2 plugin ABI or VST2 binary is shipped.**

A few controls are intentionally made more intuitive at the LVS layer. For example, Delay and Pitch Delay expose time in the expected short-to-long direction, and effects that need it can use a conventional dry/wet crossfade instead of a send-style mix law.

## Build

Requirements:

- Windows
- Visual Studio 2022 with C++ desktop tools
- CMake 3.22 or newer
- Git, so CMake can fetch the pinned Steinberg VST3 SDK revision

From the repository root:

```powershell
cmake -S lvs-plugins -B build/lvs-plugins -G "Visual Studio 17 2022" -A x64 -DSMTG_CREATE_PLUGIN_LINK=OFF
cmake --build build/lvs-plugins --config Release --parallel
```

The build produces **24 `LVS*.vst3` bundles**. The project intentionally fails configuration on non-Windows systems.

## CI and test releases

`.github/workflows/lvs-plugins-build.yml` builds and validates the complete Windows x64 suite for plugin-related pull requests and for plugin changes landing on `master`.

CI refuses to package a release unless it finds exactly **24 LVS VST3 bundles**. Same-repository pull requests also publish/update a downloadable test pre-release tagged:

```text
lvs-v0.2.0-pr<PR number>-test
```

The test asset is `LVS-Audio-Suite-Windows-x64.zip`, allowing manual testing in Lyric Video Studio or another Windows VST3 host without setting up a local C++ build environment.

`.github/workflows/lvs-plugins-release.yml` uses the same build/package checks for production tags matching `lvs-v*`.

## Validation

The original four-plugin 0.1.0 suite passed Steinberg's VST3 validator with **47 tests passed, 0 failed per plugin** and was manually smoke-tested in Lyric Video Studio.

The expanded 0.2.0 suite is validated through the same Windows GitHub Actions + Steinberg validator path before release.

## Licensing

See `THIRD_PARTY_NOTICES.md` for the Airwindows and Steinberg VST3 SDK notices and the exact Airwindows algorithms used by the suite.
