#include <IRsend.h>

IRsend irsend(16);

void setup() {
  Serial.begin(115200);
  irsend.begin();
}

void loop() {
  irsend.sendNEC(0xFF4AB5, 32);
  Serial.println("send");
  delay(200);
}
