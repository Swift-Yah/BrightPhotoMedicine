/*
   Bright Photomedicine.

   Setting up the Bluetooth hardware.
   Receives signals from mobile app to Bluetooth hardware.
   Receives from the Bluetooth hardware, sends to software serial.
   Receives from software serial, sends to led's pins.
*/

#include <Regexp.h>
#include <SoftwareSerial.h>

// The read pin for bluetooth serial port.
const int rxPin = 0;

// The write pin for bluetooth serial port.
const int txPin = 1;

// The pin that indicates the use of the bluetooth.
const int statusPin = 2;

// The baud-rate that bluetooth users will use.
const int baudRateBluetooth = 9600;

// The baud-rate that we use for output user inputs.
const long baudRateSerialOutput = 115200;

// The maximum brightness for a led.
const int maximumBrightness = 255;

// The minimum brightness for a led.
const int minimumBrightness = 0;

// First available pin to user use.
const int firstPin = 3;

// Last available pin to user use.
const int lastPin = 13;

// The minimum value to intensity of a led.
const int minimumIntensity = 0;

// The maximum value to intensity of a led.
const int maximumIntensity = 100;

// The minimum value to pulses of a led.
const int minimumFrequency = 1;

// The maximum value to pulses of a led.
const int maximumFrequency = 9999;

// The minimum time to turn off the led (in seconds).
const int minimumTimeToOff = 1;

// The maximum time to turn off the led (in seconds).
const int maximumTimeToOff = 3600;

// Miliseconds in one second.
const int milisecondsInSecond = 1000;

// Message sent to user when it connected with us.
const String messageWhenConnected = "Welcome to Swift Yah's Arduino...";

// Initial message on input to us.
const String startingOutputMessage = "Waiting for user' inputs.";

// The protocol send to us not is correct.
const String dontMatchesWithProtocol = "Sorry, the data send to us is wrong.";

// The pin sent from the user is out of the range.
const String wrongRangeOfPin = "Sorry, the PIN LED value is out of the range";

// The intensity sent from the user is out of the range.
const String wrongRangeOfIntensity = "Sorry, the INTENSITY value is out of the range";

// The frequency sent from the user is out of the range.
const String wrongRangeOfFrequency = "Sorry, the FREQUENCY value is out of the range";

// The time to off sent from the user is out of the range.
const String wrongRangeOfTimeToOff = "Sorry, the TIME TO TURN OFF value is out of the range";

// Show message to introduce received value.
const String showDataReceived = "The value received was: \n";

// The protocol that we were waiting.
// A unique string that should be the following format.
// bright://swift-yah.io/?pin={ledPin}&intensity={intensityOfBright}&frequency={numberOfPulses}&time={totalTimeTilTurnOffAgain}
const String pattern = "^bright:\\/\\/swift-yah.io\\/\?pin=(.*)&intensity=(.*)&frequency=(.*)&time=(.*)$";

// The object for bluetooth serial operations.
SoftwareSerial myBluetooth(rxPin, txPin);

// Set up the bluetooth.
void setUpBluetooth() {
  // Set up the baud-rate for 9600.
  myBluetooth.begin(baudRateBluetooth);

  // A message to be read for the Bluetooth users.
  myBluetooth.print(messageWhenConnected);

  // Set up the this led to show the status of the bluetooth.
  pinMode(statusPin, OUTPUT);
}

// Alias for digital write HIGH.
void turnOnLed(int pin) {
  digitalWrite(pin, HIGH);
}

// Alias for digital write LOW.
void turnOffLed(int pin) {
  digitalWrite(pin, LOW);
}

// Change the brightness for a current pin.
void changeBrightness(int pin, int brigthness) {
  analogWrite(pin, brigthness);
}

void printOnSerial(String message, bool isWriteForUser) {
  Serial.print(message);

  if (isWriteForUser) {
    myBluetooth.print(message);
  }
}

// Set up all non-used for bluetooth and status pin for potencial used for the end-users.
void setUpAllPinsAsOutputs() {
  for (int i = firstPin; i <= lastPin; i++) {
    // Set the current pin as OUTPUT pin.
    pinMode(i, OUTPUT);

    // Turn off current pin.
    digitalWrite(i, LOW);
  }
}

void setUpSerialStatus() {
  Serial.begin(baudRateSerialOutput);
  printOnSerial(startingOutputMessage, false);
}

int convertBrightnessInputedValue(int value) {
  if (value < minimumIntensity || value > maximumIntensity) {
    return minimumIntensity;
  }

  return (maximumBrightness / maximumIntensity) * value;
}

int convertSecondsForMiliseconds(int seconds) {
  if (seconds < minimumTimeToOff) {
    return minimumTimeToOff;
  }

  return seconds * milisecondsInSecond;
}

// Structure to be translated the data from protocol.
struct BrightProtocol {

  // The led that we must manipulate: 3 - 13.
  int ledPin;

  // The brightness of the led: 0 - 100.
  int intensity;

  // The number of time that the led will pulse before turn off.
  int frequency;

  // Time to stays on.
  int timeToOff;

  // A flag that informs us that this object is prepared to work in the led.
  bool readyToGo;

  // Processed input values.

  // The real brightness to set in the led with analogWrite: 0 - 255.
  int brightness;

  // The time between each pulse.
  int milisecondsBetweenPulses;

  // Total time to turn off.
  int milisecondsToOff;

  // Do a series of checks to be sure that we can use it.
  void checkIfReadyToGo() {
    readyToGo = true;

    if (ledPin < firstPin || ledPin > lastPin) {
      readyToGo = false;
      printOnSerial(wrongRangeOfPin, true);
    }

    if (intensity < minimumIntensity || intensity > maximumIntensity) {
      readyToGo = false;
      printOnSerial(wrongRangeOfIntensity, true);
    }

    if (frequency < minimumFrequency || frequency > maximumFrequency) {
      readyToGo = false;
      printOnSerial(wrongRangeOfFrequency, true);
    }

    if (timeToOff < minimumTimeToOff || timeToOff > maximumTimeToOff) {
      readyToGo = false;
      printOnSerial(wrongRangeOfTimeToOff, true);
    }

    if (readyToGo) {
      brightness = convertBrightnessInputedValue(intensity);
      milisecondsToOff = convertSecondsForMiliseconds(timeToOff);
      milisecondsBetweenPulses = milisecondsToOff / frequency;
    }
  }
};

struct BrightProtocol brightPin;

bool checkRegexForProtocol(String data) {
  // Match state object.
  MatchState matchState;


}

void processProtocolData() {
  brightPin.checkIfReadyToGo();

  if (brightPin.readyToGo) {

  }
}

void workOnDataReceived(String data) {
  turnOnLed(statusPin);

  printOnSerial(showDataReceived, false);
  printOnSerial(data, false);

  bool working = checkRegexForProtocol(data);

  if (working) {
    processProtocolData();
  } else {
    brightPin.readyToGo = false;
    printOnSerial(dontMatchesWithProtocol, true);
  }
}

// Arduino piece.

void setup() {
  setUpBluetooth();
  setUpAllPinsAsOutputs();
  setUpSerialStatus();
}

void loop() {
  // Turn off the status led for indicate that we not read anything.
  turnOffLed(statusPin);

  if (myBluetooth.available()) {
    String value = myBluetooth.readString();

    workOnDataReceived(value);
  }

  // Wait for a time to read again the serial.
  delay(3000);
}

