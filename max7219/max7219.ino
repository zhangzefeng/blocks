#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <IRrecv.h>
#include <IRsend.h>

// GPIO14 - CLK
// GPIO12 - CS
// GPIO13 - DIN
int pinCS = 12;
Max72xxPanel m = Max72xxPanel(pinCS, 1, 1);

long unsigned int code[4] = {0xFF4AB5, 0xFF4AB5, 0xFF4AB5, 0xFF4AB5};
const uint8_t R[8] = {0xFE, 0xFF, 0xE7, 0xFF, 0xFE, 0xE7, 0xE7, 0xE7};
const uint8_t S[8] = {0x7E, 0xFF, 0xE0, 0xFE, 0x7F, 0x07, 0xFF, 0x7E};
const uint8_t V[8] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x3C};
const uint8_t P[8] = {0xFE, 0xFF, 0xE7, 0xFF, 0xFE, 0xE0, 0xE0, 0xE0};
const uint8_t* bitmap[4] = {R, S, V, P};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  m.setRotation(0, 0);
  m.setIntensity(5); // the brightness of the display. (0..15)
}

int last = 0;

void loop() {
  draw(bitmap[last]);
  last = (last + 1) % 4;
  delay(1000);
}

void clear() {
  m.fillScreen(0);
}

void draw(const uint8_t* x) {
  clear();
  for (int j = 0; j < 8; j++) {
    int b = 1;
    for (int i = 0; i < 8; i++) {
      if ((x[j] & b) == b) {
        m.drawPixel(7 - i, j, 1);
      }
      b = b << 1;
    }
  }
  m.write();
}
