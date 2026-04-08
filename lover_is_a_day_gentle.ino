#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ── LED heart pins ────────────────────────────────────────────
//        D3  D4
//      D2      D5
//        D7  D6
//           D8
//           D9
const int LED_COUNT = 8;
const int ledPins[LED_COUNT] = { 2, 3, 4, 5, 6, 7, 8, 9 };

// ── Lyrics in PROGMEM ─────────────────────────────────────────
const char l00[] PROGMEM = "Time changed,\nwe're different";
const char l01[] PROGMEM = "But my mind still\nsays redundant things";
const char l02[] PROGMEM = "Can I not think?";
const char l03[] PROGMEM = "Will you love\nthis part of me?";
const char l04[] PROGMEM = "My lover is a day\nI can't forget";
const char l05[] PROGMEM = "Furthering my\ndistance from you";
const char l06[] PROGMEM = "Realistically\nI can't leave now";
const char l07[] PROGMEM = "But I'm okay\nas long as you";
const char l08[] PROGMEM = "Keep me from\ngoing crazy";
const char l09[] PROGMEM = "Keep me from\ngoing crazy";

const char* const lyrics[] PROGMEM = {
  l00, l01, l02, l03, l04,
  l05, l06, l07, l08, l09
};

const unsigned int durations[] PROGMEM = {
  3200, 3400, 2700, 3300, 8400,
  3500, 3700, 3500, 3100, 4600
};

const int LYRIC_COUNT = 10;
char buf[48];

// ── LED helpers ───────────────────────────────────────────────
void ledsAll(bool on) {
  for (int i = 0; i < LED_COUNT; i++)
    digitalWrite(ledPins[i], on ? HIGH : LOW);
}

// ── Gentle animations (only 1 LED on at a time, slow steps) ───

// Slow gentle sweep forward — one LED glides around the heart
// stepDelay 250ms = very slow and calm
void ledGlide(unsigned long durationMs, int stepDelay = 250) {
  unsigned long start = millis();
  int i = 0;
  while (millis() - start < durationMs) {
    ledsAll(false);
    digitalWrite(ledPins[i], HIGH);
    delay(stepDelay);
    i = (i + 1) % LED_COUNT;
  }
  ledsAll(false);
}

// Slow bounce back and forth — very relaxing
void ledSoftBounce(unsigned long durationMs) {
  unsigned long start = millis();
  int i = 0;
  int dir = 1;
  while (millis() - start < durationMs) {
    ledsAll(false);
    digitalWrite(ledPins[i], HIGH);
    delay(220);
    i += dir;
    if (i >= LED_COUNT) { i = LED_COUNT - 2; dir = -1; }
    if (i < 0)          { i = 1;             dir =  1; }
  }
  ledsAll(false);
}

// Breathe — single LED on then off with a long hold
// Feels like a slow heartbeat
void ledBreathe(unsigned long durationMs) {
  unsigned long start = millis();
  int i = 0;
  while (millis() - start < durationMs) {
    // Light one LED for 600ms then off for 400ms
    ledsAll(false);
    digitalWrite(ledPins[i], HIGH);
    delay(600);
    digitalWrite(ledPins[i], LOW);
    delay(400);
    i = (i + 1) % LED_COUNT;
  }
  ledsAll(false);
}

// Soft fill — one by one on slowly, long pause, one by one off slowly
void ledSoftFill(unsigned long durationMs) {
  unsigned long start = millis();
  while (millis() - start < durationMs) {
    ledsAll(false);
    // Fill up slowly
    for (int i = 0; i < LED_COUNT; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(180);
      if (millis() - start >= durationMs) return;
    }
    // Hold a moment with just the last one (others off)
    ledsAll(false);
    digitalWrite(ledPins[LED_COUNT - 1], HIGH);
    delay(400);
    // Unfill slowly
    for (int i = LED_COUNT - 1; i >= 0; i--) {
      digitalWrite(ledPins[i], LOW);
      delay(180);
      if (millis() - start >= durationMs) return;
    }
    delay(300);
  }
  ledsAll(false);
}

// Spotlight — stays on one LED for a while then gently moves to next
void ledSpotlight(unsigned long durationMs) {
  unsigned long start = millis();
  int i = 0;
  while (millis() - start < durationMs) {
    ledsAll(false);
    digitalWrite(ledPins[i], HIGH);
    delay(500);  // long hold per position
    i = (i + 1) % LED_COUNT;
  }
  ledsAll(false);
}

// ── Pick animation per lyric line ─────────────────────────────
void runLedForLine(int lineIndex, unsigned long ms) {
  switch (lineIndex) {
    case 0: ledGlide(ms, 250);    break;  // slow glide
    case 1: ledSoftBounce(ms);    break;  // soft bounce
    case 2: ledSpotlight(ms);     break;  // calm spotlight
    case 3: ledGlide(ms, 300);    break;  // even slower glide
    case 4: ledSoftFill(ms);      break;  // title line — gentle fill
    case 5: ledSoftBounce(ms);    break;
    case 6: ledSpotlight(ms);     break;
    case 7: ledBreathe(ms);       break;  // heartbeat breathe
    case 8: ledGlide(ms, 250);    break;
    case 9: ledSoftFill(ms);      break;  // last line — fill again
  }
}

// ── Draw centred text splitting on \n ─────────────────────────
void drawCentred(const char* text) {
  strncpy(buf, text, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  char* lines[4];
  int count = 0;
  char* tok = strtok(buf, "\n");
  while (tok && count < 4) {
    lines[count++] = tok;
    tok = strtok(NULL, "\n");
  }

  u8g2.setFont(u8g2_font_6x10_tr);
  int lineH = 12;
  int totalH = count * lineH;
  int startY = (64 - totalH) / 2 + 10;

  u8g2.clearBuffer();
  for (int i = 0; i < count; i++) {
    int w = u8g2.getStrWidth(lines[i]);
    u8g2.drawStr((128 - w) / 2, startY + i * lineH, lines[i]);
  }
  u8g2.sendBuffer();
}

// ── Heart end animation — slow and dreamy ─────────────────────
void heartAnimation() {
  // Slow glide around heart 3 times while OLED shows heart
  // Each cycle draws OLED heart and moves one LED step

  int sizes[]  = { 10, 14, 10, 14, 10, 14 };
  int waits[]  = { 600, 800, 600, 800, 600, 800 };
  int steps = 6;
  int ledIdx = 0;

  for (int cycle = 0; cycle < 2; cycle++) {
    for (int s = 0; s < steps; s++) {
      // Draw OLED heart
      u8g2.clearBuffer();
      int cx = 64, cy = 30, sz = sizes[s], r = sz / 2;
      u8g2.drawDisc(cx - r, cy - r / 2, r);
      u8g2.drawDisc(cx + r, cy - r / 2, r);
      for (int i = 0; i <= sz; i++) {
        int hw = sz - i;
        u8g2.drawHLine(cx - hw, cy - r / 2 + i, hw * 2 + 1);
      }
      u8g2.setFont(u8g2_font_6x10_tr);
      int w = u8g2.getStrWidth("- Cuco -");
      u8g2.drawStr((128 - w) / 2, 58, "- Cuco -");
      u8g2.sendBuffer();

      // One LED glides slowly
      ledsAll(false);
      digitalWrite(ledPins[ledIdx % LED_COUNT], HIGH);
      ledIdx++;

      delay(waits[s]);
    }
  }

  // Final hold — spotlight slowly moves while big heart stays
  u8g2.clearBuffer();
  int cx = 64, cy = 30, sz = 14, r = sz / 2;
  u8g2.drawDisc(cx - r, cy - r / 2, r);
  u8g2.drawDisc(cx + r, cy - r / 2, r);
  for (int i = 0; i <= sz; i++) {
    int hw = sz - i;
    u8g2.drawHLine(cx - hw, cy - r / 2 + i, hw * 2 + 1);
  }
  u8g2.setFont(u8g2_font_6x10_tr);
  int w = u8g2.getStrWidth("- Cuco -");
  u8g2.drawStr((128 - w) / 2, 58, "- Cuco -");
  u8g2.sendBuffer();
  ledSpotlight(3000);
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  u8g2.begin();

  // Splash — slow glide during title
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  int w1 = u8g2.getStrWidth("Lover Is a Day");
  int w2 = u8g2.getStrWidth("- Cuco -");
  u8g2.drawStr((128 - w1) / 2, 28, "Lover Is a Day");
  u8g2.drawStr((128 - w2) / 2, 42, "- Cuco -");
  u8g2.sendBuffer();
  ledGlide(2500, 300);
}

// ── Main loop ─────────────────────────────────────────────────
void loop() {
  for (int i = 0; i < LYRIC_COUNT; i++) {
    const char* ptr = (const char*)pgm_read_word(&lyrics[i]);
    strncpy_P(buf, ptr, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    // Gentle blank between lines
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    ledsAll(false);
    delay(120);  // softer than 60ms

    drawCentred(buf);

    unsigned int ms = pgm_read_word(&durations[i]);
    runLedForLine(i, ms);
  }

  heartAnimation();

  u8g2.clearBuffer();
  u8g2.sendBuffer();
  ledsAll(false);
  delay(2000);
}
