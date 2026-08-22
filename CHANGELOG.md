# Changelog

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
