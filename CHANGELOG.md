# Changelog

This file is a template for the single project created from this repo.
At project setup time you choose exactly one kind by setting `PROJECT_KIND`
to `core` or `homebrew` (you will only build/release that chosen kind).

Update the content for your project and keep the section heading matching
the pushed release tag (CI requirement).

This file follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). Release tags must
match a section heading exactly (for example `v1.0.0`).

When you cut a release:

1. Move items from `[Unreleased]` into a new `## [vX.Y.Z] - YYYY-MM-DD` section.
2. Commit the changelog update.
3. Push the tag: `git tag vX.Y.Z && git push origin vX.Y.Z`

CI reads the matching section and uses it as the GitHub Release notes. Assets
attached to the release:

- `<binary>-<tag>.zip` — SD layout only (`cores/` or `homebrews/` + packed `.bin`)
- `<binary>-<tag>-debug.zip` — ELF + linker map (use `arm-none-eabi-addr2line` for crash PC/LR → function/line)

## [Unreleased]

### Added

- Port of Jopo12321's Game & Watch Pong as a GWHB homebrew (`PROJECT_KIND=homebrew`).
- Main menu, settings (difficulty / paddle / ball color), in-game menu, game
  over, idle demo, and the About easter egg.
- Settings persisted via `odroid_settings_app_int32_*`.
- SDK/firmware ABI: `dma2d_r2m_rgb565_start` for solid RGB565 fills (with
  line offset); Pong `FillBuffer` uses it with a CPU fallback.
- SDK/firmware ABI: `dma2d_m2m_rgb565_start_ex` for RGB565 blits with
  source/destination line offsets (pitch − width).

### Changed

- Default project kind is homebrew; packed output is `Pong.bin`.
- Graphics fills prefer firmware DMA2D R2M instead of a pure CPU loop.

## [v1.0.0] - 2026-08-12

Initial public release for your chosen kind (`core` or `homebrew`).

### Added

- Freestanding Cortex-M7 skeleton (`src/main.c`) with LCD demo, square-wave
  audio, save/load/screenshot hooks, and watchdog-friendly frame loop.
- Vendored SDK, linker scripts, and ABI bridge for `gw_firmware_abi_t`.
- Packaging for both project kinds:
  - **core** → `pack_core.py`, SD path `/cores/<name>.bin`
  - **homebrew** → `pack_homebrew.py`, SD path `/homebrews/<name>.bin`
- Docker builder integration (`make docker`) using `sylverb/retro-go-sd-builder`.
- CI build on push/PR and automated GitHub Release on `v*` tags.

### Install

Only the section corresponding to your chosen `PROJECT_KIND` is relevant for
your derived project.

**Core (`PROJECT_KIND=core`, default)**

- Copy `example.bin` to `/cores/` on the SD card.
- Place test ROMs under `/roms/example/` (dirname matches `CORE_NAME` in the
  Makefile).
- Requires firmware whose ABI matches `SDK_VERSION` in this repository.

**Homebrew (`PROJECT_KIND=homebrew`)**

- Set `PROJECT_KIND=homebrew` in the Makefile, rebuild, then copy
  `ExampleHB.bin` to `/homebrews/`.
- Optional coverflow override: `/covers/homebrew/ExampleHB.img` (JPEG ≤186×100,
  ≤10 KiB).

The release archive contains the ready-to-copy SD layout for the active project
kind only (`cores/` or `homebrews/`).
