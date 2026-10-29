/*
  =====================================================================
  06_Ripple_8x8   --   SMART Stencil Panel
  This copy is set up for:  8x8 panel (one 8x8 matrix)
  =====================================================================
  Rings of light spreading out like a stone dropped in water.
  SPEED sets how fast the rings spread.

  Needs the FastLED library:
    Sketch > Include Library > Manage Libraries, search "FastLED",
    click Install.
  =====================================================================
*/

#include <FastLED.h>

// ===================== PANEL SETTINGS ===============================
// Already set for your panel. You should not need to change these.
// (Teachers: the README explains each line.)
#define PANEL_LABEL        "8x8 panel"
#define WIDTH              8    // lights across
#define HEIGHT             8    // lights up
#define TILE_W             8    // one matrix board is 8 wide
#define TILE_H             8    //   ... and 8 tall
#define SERPENTINE         1    // wiring zig-zags
#define WIRING_BY_COLUMNS  0    // wiring runs along rows
#define FLIP_X             0    // 1 = mirror left/right
#define FLIP_Y             0    // 1 = mirror top/bottom
// ====================================================================

// ===================== STUDENT SETTINGS =============================
#define BRIGHTNESS  128     // 0 to 255.  128 = 50%.  Try 64 for dimmer.
#define SPEED       5       // 1 = slow ... 10 = fast
#define COLOR       150     // 0 red, 32 orange, 64 yellow, 96 green,
                            // 128 aqua, 160 blue, 192 purple, 224 pink

// Optional knobs (potentiometers). Remove the // in front of a line
// ONLY after a knob is wired to that pin.
// #define BRIGHTNESS_POT  A0
// #define SPEED_POT       A1
// ====================================================================

// ===================== DO NOT CHANGE ================================
#define DATA_PIN        13
#define MAX_MILLIAMPS   1000   // power limit - protects the power supply
#define BRIGHTNESS_MIN  15     // knob floor - panel never looks "dead"
#define NUM_LEDS        (WIDTH * HEIGHT)

CRGB leds[NUM_LEDS + 1];       // +1 = safe spare spot for off-panel drawing

// Turns an (x, y) position into the pixel's number on the wire.
// x = 0 is the LEFT edge, y = 0 is the BOTTOM edge.
uint16_t XY(uint8_t x, uint8_t y) {
  if (x >= WIDTH || y >= HEIGHT) return NUM_LEDS;   // off panel -> spare
  if (FLIP_X) x = WIDTH  - 1 - x;
  if (FLIP_Y) y = HEIGHT - 1 - y;
  uint8_t  tile = x / TILE_W;                        // which matrix board
  uint8_t  lx   = x % TILE_W;                        // x on that board
  uint16_t base = (uint16_t)tile * TILE_W * TILE_H;  // first pixel of board
  if (!WIRING_BY_COLUMNS) {
    if (SERPENTINE && (y & 1)) lx = TILE_W - 1 - lx;
    return base + (uint16_t)y * TILE_W + lx;
  } else {
    uint8_t ly = y;
    if (SERPENTINE && (lx & 1)) ly = TILE_H - 1 - ly;
    return base + (uint16_t)lx * TILE_H + ly;
  }
}

uint8_t readBrightness() {
#ifdef BRIGHTNESS_POT
  return map(analogRead(BRIGHTNESS_POT), 0, 1024, BRIGHTNESS_MIN, 256);
#else
  return BRIGHTNESS;
#endif
}

uint8_t readSpeed() {
#ifdef SPEED_POT
  return map(analogRead(SPEED_POT), 0, 1024, 1, 11);   // 1 to 10
#else
  return constrain(SPEED, 1, 10);
#endif
}

void show() {
  FastLED.setBrightness(readBrightness());
  FastLED.show();
}

void patternSetup();           // each pattern's own start-up (below)

void setup() {
  delay(500);                                  // let power settle
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_MILLIAMPS);
  FastLED.clear();
  show();
  patternSetup();
}
// ====================================================================


// ===================== THE PATTERN ==================================
// Drawing:  leds[ XY(x, y) ] = CHSV(hue, saturation, brightness);
//   x = 0 (left) ... WIDTH-1,   y = 0 (bottom) ... HEIGHT-1
// ====================================================================

uint8_t  cx, cy;                                 // where the stone landed
uint16_t r = 0;                                  // ring size, in 1/8 pixel steps
const uint16_t R_MAX = (WIDTH + HEIGHT) * 8;     // ring has left the panel

void newDrop() {
  cx = random8(WIDTH);
  cy = random8(HEIGHT);
  r  = 0;
}

void patternSetup() { newDrop(); }

// brightness of a ring at distance d (both in 1/8 pixel steps)
uint8_t ring(uint16_t d, int16_t radius) {
  int16_t diff = abs((int16_t)d - radius);
  return diff < 12 ? 255 - diff * 20 : 0;
}

void loop() {
  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      int16_t  dx = x - cx;
      int16_t  dy = y - cy;
      uint16_t d  = sqrt16((dx * dx + dy * dy) * 64);   // distance x 8
      uint8_t  b  = qadd8(ring(d, r), ring(d, (int16_t)r - 24) / 2);  // ring + fainter ring
      leds[XY(x, y)] = CHSV(COLOR, 255 - b / 2, 40 + scale8(b, 215));
    }
  }
  r += readSpeed();
  if (r > R_MAX) newDrop();
  show();
  delay(30);
}
