#include <IRrecv.h>

IRrecv irrecv(5);
decode_results results;

void setup() {
    Serial.begin(115200);
    irrecv.enableIRIn();
}

void loop() {
    if (irrecv.decode(&results)) {
        Serial.print("receive ");
        Serial.println((long unsigned int)results.value, HEX);
        irrecv.resume();
    } else {
        Serial.println("not received");
    }
    delay(2);
}
