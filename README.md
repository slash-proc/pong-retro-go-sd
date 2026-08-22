# Pong — Retro-Go SD homebrew

GWHB port of [Jopo12321/game-and-watch-pong](https://github.com/Jopo12321/game-and-watch-pong)
for [Game & Watch Retro-Go SD](https://github.com/sylverb/game-and-watch-retro-go-sd).

The original standalone firmware is rebuilt as a freestanding Cortex-M7 binary
that the launcher loads into `RAM_EMU` and talks to only through
`gw_firmware_abi_t`. Gameplay, menus, demo mode, and the About easter egg are
kept; solid fills use the firmware DMA2D R2M ABI helper (CPU fallback if the
start/poll fails). Direct GPIO reset is replaced by returning to the launcher.

## Install

```bash
make                          # PROJECT_KIND=homebrew is the default
# or: make docker PROJECT_KIND=homebrew
```

Copy `Pong.bin` to `/homebrews/Pong.bin` on the SD card (or unzip a
release archive onto the SD root). Optional cover override:
`/covers/homebrew/Pong.img` (JPEG ≤186×100, ≤10 KiB).

Needs firmware whose ABI matches `SDK_VERSION` in this repository.

## Controls

### Menu

| Button | Action |
| ------ | ------ |
| D-pad  | Move / change settings |
| A      | Confirm |
| B      | Back (settings / about; keeps changes) |
| POWER  | Return to Retro-Go (firmware) |

### In game

| Button | Action |
| ------ | ------ |
| UP / DOWN | Move paddle |
| GAME (Start on host: Enter) | Original in-game menu |
| PAUSE | Retro-Go pause overlay (volume, brightness, quit) |
| POWER | Return to Retro-Go (firmware) |

Idle on the main menu starts the CPU vs CPU demo. First to 10 points wins.

## Host preview

```bash
make host
./pong_host
```

Arrows = D-pad, `X` = A, `Z` = B, Enter = GAME. Esc quits.

## Build

Same toolchain as the template: `arm-none-eabi-gcc` (hard-float `fpv5-d16`),
Python 3 + Pillow, or Docker image `sylverb/retro-go-sd-builder`.

```bash
make                  # device .bin
make host             # SDL2 desktop binary
make docker           # no host ARM toolchain
```

## Credits

- Original game: [Jopo12321/game-and-watch-pong](https://github.com/Jopo12321/game-and-watch-pong) (BSD-3-Clause)
- Based on [game-and-watch-base](https://github.com/ghidraninja/game-and-watch-base) and [game-and-watch-retro-go](https://github.com/kbeckmann/game-and-watch-retro-go)
- STM32F429I-Discovery LCD drawing helpers (STMicroelectronics)
- Retro-Go SD SDK / launcher: this tree’s `sdk/`

See `src/pong/LICENSE` for the original game license. Font bitmaps under
`src/pong/font/` retain their STMicroelectronics copyright.
