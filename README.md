# Lyric Video Studio + LVS Essentials

**A video editor for musicians, lyric videos, music videos, content creation and flexible AI-assisted workflows.**

This repository is part of the [Lyric Video Studio](https://lyricvideo.studio/) ecosystem. It currently contains **LVS Essentials**, a small Windows VST3 audio-effect suite built for Lyric Video Studio using selected MIT-licensed [Airwindows](https://www.airwindows.com/) DSP algorithms behind new LVS-owned VST3 wrappers and plugin interfaces.

> **Looking for the video editor itself?**  
> Visit **[lyricvideo.studio](https://lyricvideo.studio/)** or **[download / try Lyric Video Studio](https://lyricvideo.studio/buy/)**.

[Website](https://lyricvideo.studio/) · [Features](https://lyricvideo.studio/features/) · [Download / Trial](https://lyricvideo.studio/buy/) · [Plugins](https://lyricvideo.studio/plugins/) · [Supported AI Models](https://lyricvideo.studio/supported-tested-models/) · [Help](https://lyricvideo.studio/help/)

---

## What is Lyric Video Studio?

**Lyric Video Studio (LVS)** is a desktop non-linear video editor designed especially for musicians, bands and creators who want more control than one-click video generators provide, without turning every music video into a full-time editing project.

It combines traditional timeline-based video editing with fast lyric and subtitle workflows, audio-reactive visuals, local and API-based generative AI, reusable plugins, ComfyUI workflows and automation tools.

In other words: it can be used as a **lyric video editor**, **music video editor**, **AI video editor for musicians**, or a general-purpose creator-focused NLE.

### Built for music and lyrics

- Multi-track timeline for video, images, audio, lyrics and generated media
- Fast lyric import and synchronization with LRC, SRT, VTT and TSV workflows
- Karaoke, word-by-word and letter-by-letter text effects
- 2D and 3D text, animation, transitions and audio-reactive modulation
- Local Whisper transcription and vocal/stem-separation workflows
- MilkDrop visualizers and fragment-shader effects for audio-reactive music videos
- 4K export and flexible horizontal, vertical and social-media formats

### Generative AI without locking you into one provider

Lyric Video Studio is built around **choice** rather than a single AI service.

- Local image, video, speech and music generation
- API integrations for multiple image, video and audio providers
- **ComfyUI integration**: reuse workflows with different inputs and return generated results directly to the editing workflow
- Local and online LLM support for prompting and creative assistance
- MCP support for connecting compatible AI assistants and local models to editing workflows
- Upscaling and frame interpolation for generated or traditional footage

You can use the AI features heavily, lightly, or not at all. The editor, timeline, text tools, effects and audio-reactive features remain useful for conventional video production too.

### For creators beyond lyric videos

LVS also includes tools for subtitles, automatic silence removal, recording, object detection and motion tracking, masking, chroma keying, background removal, project sharing, reusable project assets and general video editing.

See the current feature list at **[lyricvideo.studio/features](https://lyricvideo.studio/features/)**.

---

# LVS Essentials: Windows VST3 Audio Effects

The open-source work in this repository adds a compact set of bread-and-butter audio effects with their own VST3 identities and independent plugin windows.

| Plugin | Airwindows DSP | LVS controls |
| --- | --- | --- |
| **LVS Equalizer** | PearLiteEQ | High, High Mid, Low Mid, Bass |
| **LVS Compressor** | ButterComp2 | Compression, Output, Mix |
| **LVS Delay** | TapeDelay2 | Time, Feedback, Tone, Flutter, Mix |
| **LVS Reverb** | Reverb | Size, Mix |

The goal is deliberately simple: provide useful everyday EQ, compression, delay and reverb instead of exposing hundreds of experimental parameters or requiring musicians to hunt for a separate VST bundle before editing audio in a video project.

### Architecture

The plugins are **new LVS VST3 plugins**. Airwindows is used as the DSP implementation layer, not as the public plugin identity or user interface.

```text
VST3 host / Lyric Video Studio
            ↓
      LVS VST3 plugin
            ↓
    LVS parameter model
            ↓
   Airwindows DSP adapter
            ↓
    Airwindows algorithm
```

This keeps stable LVS plugin identities, automation parameters, saved state and custom interfaces while allowing the underlying DSP implementation to remain clearly separated.

The current LVS Essentials builds are **Windows x64 VST3 only**.

For build details, architecture notes and release automation, see [`lvs-plugins/README.md`](lvs-plugins/README.md).

---

## Why this repository exists

Lyric Video Studio itself is a commercial desktop application. This repository exists for the open-source audio-plugin components used around the application and for the third-party DSP code they build upon.

The LVS Essentials wrappers, interfaces and build system are kept separate from the original Airwindows tree. Third-party license notices are included with the plugin project and release packages.

If you arrived here while looking for the main application, its editor, AI integrations or plugin ecosystem, these are the useful starting points:

- **Main product:** https://lyricvideo.studio/
- **Download / free trial:** https://lyricvideo.studio/buy/
- **Full feature list:** https://lyricvideo.studio/features/
- **Plugin integrations:** https://lyricvideo.studio/plugins/
- **Supported and tested AI models:** https://lyricvideo.studio/supported-tested-models/
- **Documentation and tutorials:** https://lyricvideo.studio/help/

---

## Built for musicians, by a musician

Lyric Video Studio was created by **Matti Pulkkinen**, a Finnish software developer and musician, after years of dealing with video editors that made common music-video and lyric-video tasks unnecessarily slow or complicated.

The guiding idea is simple: **editing should give you control without making the workflow fight you**.

If you're creating a lyric video, music video, visualizer, AI-assisted video, YouTube video, Short, Reel or other music-focused content, take a look at **[Lyric Video Studio](https://lyricvideo.studio/)**.

---

## Licenses

Airwindows DSP code used by LVS Essentials is MIT licensed. The Steinberg VST3 SDK used by the build is also distributed under its applicable MIT terms.

See [`lvs-plugins/THIRD_PARTY_NOTICES.md`](lvs-plugins/THIRD_PARTY_NOTICES.md) for the notices included with LVS Essentials.

---

**Lyric Video Studio:** precise editing, fast lyric workflows, audio-reactive visuals, local generation, API integrations and repeatable AI workflows in one desktop editor.
