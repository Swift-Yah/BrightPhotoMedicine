/*
 * Bright Photomedicine
 * 
 * Receives signals from mobile app to Bluetooth hardware.
 * Receives from the Bluetooth hardware, sends to software serial.
 * Receives from software serial, sends to led's pins.
 */
 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  Serial.println("Bluetooth initialized!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

