# Third-party notices

LVS Audio Suite includes or adapts third-party open-source software. The LVS VST3 wrappers, parameter/state mapping, editor definitions and build integration are separate from the third-party DSP implementations listed below.

## Airwindows

Airwindows is by Chris Johnson and is distributed under the MIT License.

The LVS Audio Suite currently uses these Airwindows algorithms:

- `PearLiteEQ` for **LVS Equalizer**
- `ButterComp2` for **LVS Compressor**
- `TapeDelay2` for **LVS Delay**
- `Reverb` for **LVS Reverb**
- `Recurve` for **LVS Limiter**
- `ADClip9` for **LVS Clipper**
- `DeBess` for **LVS De-Esser**
- `SoftGate` for **LVS Gate**
- `StereoChorus` for **LVS Chorus**
- `Wider` for **LVS Stereo Width**
- `Density3` for **LVS Saturation**
- `ToTape9` for **LVS Tape**
- `StereoDoubler` for **LVS Doubler**
- `BiquadDouble` for **LVS Filter**
- `Tremolo` for **LVS Tremolo**
- `AutoPan` for **LVS Auto Pan**
- `Air3` for **LVS Exciter**
- `Drive` for **LVS Distortion**
- `Podcast` for **LVS Vocal Leveler**
- `StereoEnsemble` for **LVS Ensemble**
- `PitchDelay` for **LVS Pitch Delay**
- `RingModulator` for **LVS Ring Modulator**
- `DeRez4` for **LVS Lo-Fi**
- `FireAmp` for **LVS Guitar Amp**

Selected original Airwindows source files are compiled only as private DSP implementation behind the LVS compatibility adapters. No legacy Airwindows VST2 plugin binary or VST2 ABI is redistributed by the LVS VST3 targets.

MIT License

Copyright (c) 2018 Chris Johnson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Steinberg VST 3 SDK

The LVS plugin wrappers and editors use the Steinberg VST 3 SDK and VSTGUI support supplied with the SDK.

MIT License

Copyright (c) 2025, Steinberg Media Technologies GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
