#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <IRrecv.h>
#include <IRsend.h>

int pinCS = 12;
Max72xxPanel m = Max72xxPanel(pinCS, 1, 1);

IRrecv irrecv(5, 1000, 1500); // 150ms timeout
decode_results results;

IRsend irsend(16);

long unsigned int code[4] = {0xFF4AB5, 0xFF4AB6, 0xFF4AB7, 0xFF4AB8};
const uint8_t R[8] = {0xFE, 0xFF, 0xE7, 0xFF, 0xFE, 0xE7, 0xE7, 0xE7};
const uint8_t S[8] = {0x7E, 0xFF, 0xE0, 0xFE, 0x7F, 0x07, 0xFF, 0x7E};
const uint8_t V[8] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x3C};
const uint8_t P[8] = {0xFE, 0xFF, 0xE7, 0xFF, 0xFE, 0xE0, 0xE0, 0xE0};
const uint8_t* bitmap[4] = {R, S, V, P};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  m.setRotation(0, 0);
  m.setIntensity(1); // the brightness of the display. (0..15)
  irrecv.enableIRIn();
  irsend.begin();
}

int last = -1;
int lostcnt = 0;

void loop() {
  int thres = analogRead(A0);
  // thres = 0; // always on
  Serial.print("dist thres=");
  Serial.println(thres);
  if (thres > 300) {
    clear();
  } else {
    int left = -1;
    if (irrecv.decode(&results)) {
      long unsigned int r = (long unsigned int)results.value;
      irrecv.resume();
      for (int i = 0; i < 3; i++) {
        if (r == code[i]) {
          left = i;
          Serial.print("left receive ");
          Serial.println(i);
        }
      }
    }
    if (left < 0) {
      Serial.println("left lost");
    }
    if (left >= 0) {
      last = left;
      lostcnt = 0;
    } else {
      if (lostcnt < 10000) {
        lostcnt++;
      }
      if (lostcnt > 10) {
        last = -1;
      }
    }
    Serial.print("last=");
    Serial.print(last);
    Serial.print(" lostcnt=");
    Serial.println(lostcnt);
    Serial.print("I AM ");
    Serial.println(last + 1);
    draw(bitmap[last + 1]);
    irsend.sendNEC(code[last + 1], 32);
  }
  delay(5);
}

void clear() {
  m.fillScreen(0);
  m.write();
}

void draw(const uint8_t* x) {
  for (int j = 0; j < 8; j++) {
    int b = 1;
    for (int i = 0; i < 8; i++) {
      m.drawPixel(7 - i, j, (x[j] & b) == b ? 1 : 0);
      b = b << 1;
    }
  }
  m.write();
}
