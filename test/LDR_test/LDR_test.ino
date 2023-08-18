
int pinLDR = 32;

void setup() {
  pinMode(pinLDR, INPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.print(analogRead(pinLDR));
  Serial.print("\n");
  delay(1000);
}
