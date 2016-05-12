/*
 * Bright Photomedicine
 * 
 * Setting up the Bluetooth hardware.
 * Receives signals from mobile app to Bluetooth hardware.
 * Receives from the Bluetooth hardware, sends to software serial.
 * Receives from software serial, sends to led's pins.
 */
 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

private void setUpBluetooth() {
  
}

void setup() {
  setUpBluetooth();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("Bluetooth initialized!");

  // Set the data rate for the SoftwareSerial port
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

