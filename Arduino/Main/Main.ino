/*
   Bright Photomedicine.

   Setting up the Bluetooth hardware.
   Receives signals from mobile app to Bluetooth hardware.
   Receives from the Bluetooth hardware, sends to software serial.
   Receives from software serial, sends to led's pins.
*/

#include <Regexp.h>
#include <SoftwareSerial.h>
#include <Thread.h>

// The read pin for bluetooth serial port.
const byte rxPin = 0;

// The write pin for bluetooth serial port.
const byte txPin = 1;

// The pin that indicates the use of the bluetooth.
const byte statusPin = 2;

// The baud-rate that bluetooth users will use.
const short baudRateBluetooth = 9600;

// The baud-rate that we use for output user inputs.
const long baudRateSerialOutput = 9600;

// The maximum brightness for a led.
const float maximumBrightness = 255;

// The minimum brightness for a led.
const byte minimumBrightness = 0;

// First available pin to user use.
const byte firstPin = 3;

// Last available pin to user use.
const byte lastPin = 13;

// The minimum value to intensity of a led.
const byte minimumIntensity = 0;

// The maximum value to intensity of a led.
const float maximumIntensity = 100;

// The minimum value to pulses of a led.
const byte minimumFrequency = 1;

// The maximum value to pulses of a led.
const short maximumFrequency = 9999;

// The minimum time to turn off the led (in seconds).
const byte minimumTimeToOff = 1;

// The maximum time to turn off the led (in seconds).
const short maximumTimeToOff = 3600;

// Miliseconds in one second.
const short milisecondsInSecond = 1000;

// Message sent to user when it connected with us.
const String messageWhenConnected = "Welcome to Swift Yah's Arduino...";

// Initial message on input to us.
const String startingOutputMessage = "Waiting for user' inputs.";

// The protocol send to us not is correct.
const String dontMatchesWithProtocol = "Sorry, the data send to us is wrong.";

// When we could not process correctly the data sent to us.
const String dontWorkingWithDataOnProtocol = "We could not get all data we need.";

// We make a wrong regex.
const String problemsOnRegexP = "Sorry, we make a mistake on the pattern.";

// The pin sent from the user is out of the range.
const String wrongRangeOfPin = "Sorry, the PIN LED value is out of the range";

// The intensity sent from the user is out of the range.
const String wrongRangeOfIntensity = "Sorry, the INTENSITY value is out of the range";

// The frequency sent from the user is out of the range.
const String wrongRangeOfFrequency = "Sorry, the FREQUENCY value is out of the range";

// The time to off sent from the user is out of the range.
const String wrongRangeOfTimeToOff = "Sorry, the TIME TO TURN OFF value is out of the range";

// Show message to introduce received value.
const String showDataReceived = "The value received was: ";

// Show message to warns the user about some data missed.
const String weCouldNotFindAllData = "Sorry, we need all data, but some are failing.";

// The maximum size for be used on a buffer object.
const byte maximumSizeOfBuffer = 255;

// The number of values passed on protocol.
const byte amountOfValuesOnProtocol = 4;

// The increase step for each PMW sent to pulse.
const byte pulseStep = 5;

// The protocol that we were waiting.
// A unique string that should be the following format.
// Model: {(pinLed), (intensity), (frequency), (time)}
// Example: {4, 30, 2, 10}
const char pattern[] = "{(.*), (.*), (.*), (.*)}";

// Each index indicates the position on regex groups of a value.

const byte pinLedIndex = 0;
const byte intensityIndex = 1;
const byte frequencyIndex = 2;
const byte timeIndex = 3;

// The object for bluetooth serial operations.
SoftwareSerial myBluetooth(rxPin, txPin);

// Set up the bluetooth.
void setUpBluetooth() {
  // Set up the baud-rate.
  myBluetooth.begin(baudRateBluetooth);

  // Set up RX as INPUT.
  pinMode(rxPin, INPUT);

  // Set up TX as OUTPUT.
  pinMode(txPin, OUTPUT);

  // A message to be read for the Bluetooth users.
  printOnSerial(messageWhenConnected, true);
}

// Alias for digital write HIGH.
void turnOnLed(byte pin) {
  digitalWrite(pin, HIGH);
}

// Alias for digital write LOW.
void turnOffLed(byte pin) {
  digitalWrite(pin, LOW);
}

// Change the brightness for a current pin.
void changeBrightness(byte pin, byte brigthness) {
  analogWrite(pin, brigthness);
}

// Use the Serial service to print a message.
void printOnSerial(String message, bool isWriteForUser) {
  Serial.print("OUTPUT: ");
  Serial.println(message);

  if (isWriteForUser) {
    myBluetooth.println(message);
  }
}

// Set up all non-used for bluetooth and status pin for potencial used for the end-users.
void setUpAllPinsAsOutputs() {
  for (byte i = firstPin; i <= lastPin; i++) {
    // Set the current pin as OUTPUT pin.
    pinMode(i, OUTPUT);

    // Turn off current pin.
    turnOffLed(i);
  }
}

// Initializer the serial for output for us.
void setUpSerialStatus() {
  // Set up our serial output.
  Serial.begin(baudRateSerialOutput);

  printOnSerial(startingOutputMessage, false);

  // Set up the this led to show the status of the bluetooth.
  pinMode(statusPin, OUTPUT);
}

// Convert from 0 - 100 to 0 - 255.
short convertToBrightness(float value) {
  if (value < minimumIntensity || value > maximumIntensity) {
    return minimumIntensity;
  }

  return (maximumBrightness / maximumIntensity) * value;
}

// Convert 1 seconds to 1000 miliseconds and go on.
long convertSecondsForMiliseconds(short seconds) {
  if (seconds < minimumTimeToOff) {
    return minimumTimeToOff;
  }

  return seconds * milisecondsInSecond;
}

// Structure to be translated the data from protocol.
struct BrightProtocol {
  // The led that we must manipulate: 3 - 13.
  byte ledPin;

  // The brightness of the led: 0 - 100.
  byte intensity;

  // The number of time that the led will pulse before turn off.
  short frequency;

  // Time to stays on.
  short timeToOff;

  // A flag that informs us that this object is prepared to work in the led.
  bool readyToGo;

  // The real brightness to set in the led with analogWrite: 0 - 255.
  float brightness;

  // The time between each pulse.
  float milisecondsBetweenPulses;

  // Total time to turn off.
  long milisecondsToOff;

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
      brightness = convertToBrightness(intensity);
      milisecondsToOff = convertSecondsForMiliseconds(timeToOff);
      milisecondsBetweenPulses = milisecondsToOff / frequency;
    }
  }
};

// A structure to store the data getted from protocol.
struct BrightProtocol brightPin;

// Check the pattern of the our protocol for the data sent to us.
bool checkRegexForProtocol(String data) {
  byte dataLength = data.length();

  // A wrapper for String data.
  char dataArray[dataLength];

  // Will store each data recovered.
  char pinLedData[10];
  char intensityData[10];
  char frequencyData[10];
  char timeData[10];

  // Convert our String to char[].
  data.toCharArray(dataArray, dataLength);

  // Match state object.
  MatchState matchState;

  // Buffer must be larger enough to hold expected string, or malloc it.
  char resultBuffer[dataLength];

  // Setting up the string that we are searching.
  matchState.Target(dataArray);

  // Let's search.
  char matchResult = matchState.Match(pattern, 0);

  switch (matchResult) {
    case REGEXP_MATCHED:
      if (matchState.level < amountOfValuesOnProtocol) {
        printOnSerial(weCouldNotFindAllData, true);

        return false;
      }

      matchState.GetCapture(pinLedData, pinLedIndex);
      matchState.GetCapture(intensityData, intensityIndex);
      matchState.GetCapture(frequencyData, frequencyIndex);
      matchState.GetCapture(timeData, timeIndex);

      brightPin.ledPin = atoi(pinLedData);
      brightPin.intensity = atoi(intensityData);
      brightPin.frequency = atoi(frequencyData);
      brightPin.timeToOff = atoi(timeData);

      return true;

    case REGEXP_NOMATCH:
      printOnSerial(dontMatchesWithProtocol, true);

      return false;

    default:
      printOnSerial(problemsOnRegexP, false);

      return false;
  }
}

// Checks the data inputed as our protocol.
void processProtocolData() {
  brightPin.checkIfReadyToGo();

  if (brightPin.readyToGo) {
    int totalDelay = 0;
    
    for (byte b = 0; b < brightPin.frequency; b++) {
      float eachIncreaseDelay = (brightPin.milisecondsToOff / brightPin.milisecondsBetweenPulses) / 2.0;
      Serial.print("B = ");
      Serial.println(eachIncreaseDelay);

      for (byte i = minimumBrightness; i < brightPin.brightness; i += pulseStep) {
        Serial.print("I = ");
        Serial.println(i);
        analogWrite(brightPin.ledPin, i);
        delay(eachIncreaseDelay);
        totalDelay += eachIncreaseDelay;
      }

      for (byte j = maximumBrightness; j > 0; j -= pulseStep) {
        Serial.print("J = ");
        Serial.println(j);
        analogWrite(brightPin.ledPin, j);
        delay(eachIncreaseDelay);
        totalDelay += eachIncreaseDelay;
      }
    }

    Serial.print(totalDelay);
  }
}

// Start to work on data received from bluetooth serial.
void workOnDataReceived(String data) {
  printOnSerial(showDataReceived, false);
  printOnSerial(data, false);

  bool working = checkRegexForProtocol(data);

  if (working) {
    processProtocolData();
  } else {
    printOnSerial(dontWorkingWithDataOnProtocol, true);
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
  turnOnLed(statusPin);

  if (myBluetooth.available()) {
    turnOffLed(statusPin);
    
    String value = myBluetooth.readString();

    workOnDataReceived(value);
  }

  delay(100);
}

