# LVS Essential Plugins

A small Windows VST3 effect suite for Lyric Video Studio, built around selected MIT-licensed Airwindows DSP algorithms with LVS-owned VST3 wrappers, automation/state handling, names and independent VSTGUI editors.

## Plugins

| Plugin | DSP | Controls |
| --- | --- | --- |
| LVS Equalizer | Airwindows PearLiteEQ | High, High Mid, Low Mid, Bass |
| LVS Compressor | Airwindows ButterComp2 | Compression, Output, Mix |
| LVS Delay | Airwindows TapeDelay2 | Time, Feedback, Tone, Flutter, Mix |
| LVS Reverb | Airwindows Reverb | Size, Mix |

All four plugins also expose a standard VST3 bypass parameter.

## Design

The public plugins are new LVS VST3 plugins. Airwindows is treated as the DSP implementation layer, not as the plugin identity or UI.

- `common/` contains shared host-independent LVS audio abstractions.
- `dsp/` contains the LVS DSP adapters.
- `legacy/audioeffectx.h` is a minimal private compatibility shim used only to compile selected original Airwindows VST2-era DSP sources. It does not expose or ship a VST2 plugin ABI.
- `plugins/` contains the VST3 processor, controller, factory and editor for each LVS plugin.

PearLiteEQ and ButterComp2 are adapted into host-independent DSP classes. TapeDelay2 and Reverb compile the original Airwindows processing sources behind the private compatibility layer so those larger algorithms remain close to upstream.

The plugins currently accept stereo input/output and 32-bit floating-point audio processing. They intentionally reject mono bus negotiation rather than accepting a layout the DSP wrappers do not implement.

## Build

Requirements:

- Windows
- Visual Studio 2022 with C++ desktop tools
- CMake 3.22 or newer
- Git, so CMake can fetch the Steinberg VST3 SDK

From the repository root:

```powershell
cmake -S lvs-plugins -B build/lvs-plugins -G "Visual Studio 17 2022" -A x64 -DSMTG_CREATE_PLUGIN_LINK=OFF
cmake --build build/lvs-plugins --config Release --target LVSEqualizer LVSCompressor LVSDelay LVSReverb --parallel
```

The resulting bundles are created under the CMake VST3 output directory as:

- `LVSEqualizer.vst3`
- `LVSCompressor.vst3`
- `LVSDelay.vst3`
- `LVSReverb.vst3`

The product names reported to VST3 hosts are `LVS Equalizer`, `LVS Compressor`, `LVS Delay` and `LVS Reverb`.

## CI and releases

`.github/workflows/lvs-plugins-build.yml` builds and validates the Windows x64 suite for pull requests that touch the plugin project, and again after changes land on `master`.

`.github/workflows/lvs-plugins-release.yml` builds the same four plugins for tags matching `lvs-v*`, packages them with the third-party notices into `LVS-Essentials-Windows-x64.zip`, and creates or updates the matching GitHub Release.

Example release tag:

```powershell
git tag lvs-v0.1.0
git push origin lvs-v0.1.0
```

## Validation

The Windows GitHub Actions build runs Steinberg's VST3 validator as part of each plugin target. The first complete four-plugin validation passed all 47 validator tests for each plugin.

## Licensing

See `THIRD_PARTY_NOTICES.md` for the Airwindows and Steinberg VST3 SDK notices. LVS wrapper/editor code remains separate from the third-party DSP sources.
