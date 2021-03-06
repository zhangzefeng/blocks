void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int thres = analogRead(A0);
  Serial.print("analog thres=");
  Serial.println(thres);
  if (thres > 300)
     digitalWrite(LED_BUILTIN, HIGH);
  else
     digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

