#!/usr/bin/env python3
"""Compact Pong cover tile for Retro-Go SD coverflow (≤186×100, ≤10 KiB)."""

from __future__ import annotations

import argparse
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


COVER_MAX_WIDTH = 186
COVER_MAX_HEIGHT = 100
COVER_SIZE_MAX = 10 * 1024


def load_font(size: int) -> ImageFont.ImageFont:
    for name in (
        "DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
    ):
        try:
            return ImageFont.truetype(name, size=size)
        except OSError:
            continue
    return ImageFont.load_default()


def generate(path: Path, width: int, height: int) -> None:
    if width > COVER_MAX_WIDTH or height > COVER_MAX_HEIGHT:
        raise SystemExit(
            f"cover size {width}x{height} exceeds max "
            f"{COVER_MAX_WIDTH}x{COVER_MAX_HEIGHT}"
        )

    img = Image.new("RGB", (width, height), (0, 0, 0))
    draw = ImageDraw.Draw(img)
    draw.rectangle((1, 1, width - 2, height - 2), outline=(255, 255, 255))

    paddle_w, paddle_h = 4, max(16, height // 4)
    draw.rectangle((6, (height - paddle_h) // 2,
                    6 + paddle_w, (height + paddle_h) // 2), fill=(255, 255, 255))
    draw.rectangle((width - 10, height // 3,
                    width - 6, height // 3 + paddle_h), fill=(255, 255, 255))

    cx, cy, r = width // 2 + 10, height // 2 + 6, 4
    draw.ellipse((cx - r, cy - r, cx + r, cy + r), fill=(255, 220, 40))

    font = load_font(18)
    text = "PONG"
    left, top, right, bottom = draw.textbbox((0, 0), text, font=font)
    tw, th = right - left, bottom - top
    draw.text(((width - tw) // 2 - left, 8 - top), text, fill=(255, 255, 255), font=font)

    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path, "JPEG", quality=85, optimize=True)
    size = path.stat().st_size
    if size > COVER_SIZE_MAX:
        raise SystemExit(f"cover {path} is {size} bytes (max {COVER_SIZE_MAX})")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", required=True)
    parser.add_argument("--width", type=int, default=128)
    parser.add_argument("--height", type=int, default=96)
    args = parser.parse_args()
    generate(Path(args.out), args.width, args.height)


if __name__ == "__main__":
    main()
