# SKETCH-samples

Light show sketches to try out and modify for your new **SMART Stencil Panel** (Arduino Nano + WS2812B LED matrix).

---

## 1. Download

Click the green **Code** button above, then **Download ZIP**. Unzip it (right-click → *Extract All* / *Extract Here*).

## 2. Pick the folder that matches YOUR panel

| Your panel | Open this folder |
|---|---|
| One small square matrix (8 × 8 = 64 lights) | **For_8x8_panel** |
| One large square matrix (16 × 16 = 256 lights) | **For_16x16_panel** |
| Two small matrices side by side (16 wide × 8 tall) | **For_two_8x8_side_by_side** |

> ⚠️ Use **only** the sketches in your panel's folder. A sketch from the wrong folder will light only part of the panel or show scrambled.

Every sketch name ends with its panel size (for example `_16x16`), so you can always tell which one is loaded.

## 3. Load a sketch onto the panel

1. Install the free **Arduino IDE** from arduino.cc/en/software.
2. Install the **FastLED** library: *Sketch → Include Library → Manage Libraries*, search **FastLED**, click **Install**. (Only needed once.)
3. Open the sketch: double-click the `.ino` file inside a sketch folder.
4. Plug the Nano into your computer with a USB cable.
5. *Tools → Board* → **Arduino Nano**.
   *Tools → Processor* → **ATmega328P**. If uploading fails, try **ATmega328P (Old Bootloader)**.
   *Tools → Port* → pick the one that appears when the Nano is plugged in.
6. Click the **→ Upload** button. When it says *Done uploading*, the new pattern runs.

## 4. Sketches

| # | Sketch | What it does |
|---|---|---|
| 00 | Pixel_Tester | Checks every light and shows how the panel is wired. **Run this first if anything looks wrong.** |
| 01 | Rainbow | A rainbow sliding diagonally across the panel. |
| 02 | Fire | Flames rising from the bottom. |
| 03 | Plasma | Soft blobs of color swirling and melting together. |
| 04 | Twinkle_Stars | A night sky of sparkling stars. |
| 05 | Digital_Rain | Green drops falling, like a computer movie screen. |
| 06 | Ripple | Rings spreading like a stone dropped in water. |
| 07 | Spiral | A spinning rainbow spiral. |
| 08 | Confetti | Colorful dots popping up and fading. |
| 09 | Aurora | Waving curtains of green, blue and purple, like the northern lights. |
| — | Starter_Template | A blank starting point for making your own pattern. |

## 5. Things you can change

Near the top of every sketch, in **STUDENT SETTINGS**:

- `BRIGHTNESS` — 0 to 255. Starts at 128 (50%).
- `SPEED` — 1 (slow) to 10 (fast).
- `COLOR` (Ripple and Digital_Rain only) — 0 red, 32 orange, 64 yellow, 96 green, 128 aqua, 160 blue, 192 purple, 224 pink.
- Optional knobs: a potentiometer on pin **A0** (brightness) or **A1** (speed). Wire the knob first, then remove the `//` in front of its line.

Please leave the **DO NOT CHANGE** section alone. It holds the 1 amp power limit that protects your power supply.

---

## For teachers: panel settings

Each folder's sketches carry these preset lines in **PANEL SETTINGS**:

| Setting | Meaning |
|---|---|
| `WIDTH`, `HEIGHT` | Size of the whole display in lights. |
| `TILE_W`, `TILE_H` | Size of one matrix board (8×8 boards chained, or one 16×16 sheet). |
| `SERPENTINE` | 1 = wiring zig-zags back and forth (most matrices). |
| `WIRING_BY_COLUMNS` | 0 = wiring runs along rows, 1 = along columns. |
| `FLIP_X`, `FLIP_Y` | Mirror left/right or top/bottom if the board is mounted turned. |

If **Pixel_Tester**'s *map check* does not show red along the bottom, green up the left and a blue dot top-right, adjust `WIRING_BY_COLUMNS`, `FLIP_X` and `FLIP_Y` until it does. Every other sketch uses the same settings.

All sketches: data on pin **D13**, power limit **1000 mA**, brightness **50%**.

License: see LICENSE.
