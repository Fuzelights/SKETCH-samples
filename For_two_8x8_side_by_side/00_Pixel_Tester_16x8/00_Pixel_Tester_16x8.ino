/*
  =====================================================================
  00_Pixel_Tester_16x8   --   SMART Stencil Panel
  This copy is set up for:  16 wide x 8 tall (two 8x8 matrices, left one first)
  =====================================================================
  WHAT IT DOES (repeats forever):

    1. COLOR CHECK  - whole panel turns red, green, blue, then white.
                      A pixel that stays dark or shows the wrong
                      color has a bad red, green or blue part.
                      (White looks dimmer than the others. That is
                      the 1 amp power limit working - normal.)

    2. WIRING WALK  - lights pixels one at a time in the order they
                      are wired, leaving a dim blue trail. The FIRST
                      pixel (held for 2 seconds) shows where the
                      data wire enters the panel.

    3. MAP CHECK    - RED line along the BOTTOM edge,
                      GREEN line up the LEFT edge,
                      one BLUE dot in the TOP-RIGHT corner.
                      If that is what you see, every pattern in
                      this folder will show the right way up.

  Open Tools > Serial Monitor (set to 115200 baud) to read along.
  ---------------------------------------------------------------------
  Needs the FastLED library:
    Sketch > Include Library > Manage Libraries, search "FastLED",
    click Install.
  =====================================================================
*/

#include <FastLED.h>

// ===================== PANEL SETTINGS ===============================
// Already set for your panel. You should not need to change these.
// (Teachers: the README explains each line.)
#define PANEL_LABEL        "two 8x8 side by side (16x8)"
#define WIDTH              16   // lights across (both boards)
#define HEIGHT             8    // lights up
#define TILE_W             8    // each matrix board is 8 wide
#define TILE_H             8    //   ... and 8 tall
#define SERPENTINE         1    // wiring zig-zags
#define WIRING_BY_COLUMNS  0    // wiring runs along rows
#define FLIP_X             0    // 1 = mirror left/right
#define FLIP_Y             0    // 1 = mirror top/bottom
// ====================================================================

// ===================== STUDENT SETTINGS =============================
#define BRIGHTNESS  128     // 0 to 255.  128 = 50%.  Try 64 for dimmer.
#define SPEED       5       // 1 = slow ... 10 = fast

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

// Pause that keeps the knobs working while waiting.
void waitMs(uint16_t ms) {
  uint32_t start = millis();
  do { show(); delay(20); } while (millis() - start < ms);
}
// ====================================================================

// Wiring walk step time: speed 1 = 145 ms per pixel, speed 10 = 10 ms.
uint16_t stepMs() { return 160 - readSpeed() * 15; }

void setup() {
  delay(500);                                  // let power settle
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_MILLIAMPS);
  FastLED.clear();
  show();
  Serial.println(F("SMART panel - Pixel Tester"));
  Serial.print(F("Set for: "));
  Serial.println(F(PANEL_LABEL));
}

void loop() {
  // ---- 1. COLOR CHECK ----
  Serial.println(F("1. COLOR CHECK: red, green, blue, white"));
  const CRGB colors[4] = { CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White };
  for (uint8_t c = 0; c < 4; c++) {
    fill_solid(leds, NUM_LEDS, colors[c]);
    waitMs(1500);
  }

  // ---- 2. WIRING WALK ----
  Serial.println(F("2. WIRING WALK: first pixel = where data enters"));
  FastLED.clear();
  leds[0] = CRGB::White;
  waitMs(2000);                                // hold the first pixel
  for (uint16_t i = 1; i < NUM_LEDS; i++) {
    leds[i - 1] = CRGB(0, 0, 40);              // dim blue trail
    leds[i]     = CRGB::White;
    waitMs(stepMs());
  }
  waitMs(1000);

  // ---- 3. MAP CHECK ----
  Serial.println(F("3. MAP CHECK: red = bottom, green = left, blue dot = top-right"));
  FastLED.clear();
  for (uint8_t x = 0; x < WIDTH;  x++) leds[XY(x, 0)] = CRGB::Red;
  for (uint8_t y = 0; y < HEIGHT; y++) leds[XY(0, y)] = CRGB::Green;
  leds[XY(WIDTH - 1, HEIGHT - 1)] = CRGB::Blue;
  waitMs(5000);
}
