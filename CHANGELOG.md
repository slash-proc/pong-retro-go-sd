# Changelog

## [v0.0.4] - 2026-09-16

### Changed

- Improved frame synchronization.
- Updated the changelog for v0.0.2.

## [v0.0.3] - 2026-09-13

### Changed

- Publish conservative runtime save and savestate support metadata for LFS sizing.

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

- Nothing.

### Changed

- Proper frame sync.

### Install

**Homebrew**

- Copy `Pong.bin` to `/homebrews/`.
