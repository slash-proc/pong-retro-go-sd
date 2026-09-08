# Changelog

## [v0.0.2]

Joins the GWRG distribution spec: the release now publishes a `manifest.json`
and an offline bundle beside the binary.

### Added

- `gwrg.json`, the hand-written half of the manifest. Pong is self-contained
  and is not a port of a specific console release, so it declares neither a
  converter nor an `originalSystem`.
- `manifest.json`, the offline bundle and a GitHub Pages `dist/` tree, built by
  the shared `make_manifest.py` / `make_bundle.py` / `build_dist.py`.
- `print-SIDECARS` and `print-RO_BIN` Makefile targets. This homebrew installs
  no sidecar, but `stage_release.py` reads the Makefile positionally and a
  missing target fails the release outright.

### Changed

- `scripts/stage_release.py` replaced with the shared copy, which understands
  `SIDECARS` and stages the spec artefacts.

### Fixed

- Nothing.

### Install

**Homebrew**

- Copy `Pong.bin` to `/homebrews/`.

## [v0.0.1]

### Added

- Port of Jopo12321's Game & Watch Pong as a GWHB homebrew.
- Main menu, settings (difficulty / paddle / ball color), in-game menu, game
  over, idle demo, and the About easter egg.
- Settings persisted via `odroid_settings_app_int32_*`.
- SDK/firmware ABI: `dma2d_r2m_rgb565_start` for solid RGB565 fills (with
  line offset); Pong `FillBuffer` uses it with a CPU fallback.

### Changed

- Default project kind is homebrew; packed output is `Pong.bin`.

### Install

**Homebrew**

- Copy `Pong.bin` to `/homebrews/`.
