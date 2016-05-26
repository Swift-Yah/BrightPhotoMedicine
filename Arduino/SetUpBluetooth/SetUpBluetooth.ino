int ledPin = 13;
int resetPin = 7;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  Serial.begin(115200);

  blinkLed(resetPin, 10, 2000);

  Serial.println("SET BT NAME Bright_Photomedicine");
}

void blinkLed(int pin, int firstDelay, int secondDelay) {
  digitalWrite(pin, HIGH);
  delay(firstDelay);

  digitalWrite(pin, LOW);
  delay(secondDelay);
}

void loop() {
  blinkLed(ledPin, 100, 1000);
}
