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
const long baudRateSerialOutput = 115200;

// The maximum brightness for a led.
const byte maximumBrightness = 255;

// The minimum brightness for a led.
const byte minimumBrightness = 0;

// First available pin to user use.
const byte firstPin = 3;

// Last available pin to user use.
const byte lastPin = 13;

// The minimum value to intensity of a led.
const byte minimumIntensity = 0;

// The maximum value to intensity of a led.
const byte maximumIntensity = 100;

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
const String showDataReceived = "The value received was: \n";

// Show message to warns the user about some data missed.
const String weCouldNotFindAllData = "Sorry, we need all data, but some are failing.";

// The maximum size for be used on a buffer object.
const byte maximumSizeOfBuffer = 255;

// The number of values passed on protocol.
const byte amountOfValuesOnProtocol = 4;

// The protocol that we were waiting.
// A unique string that should be the following format.
// bright://swift-yah.io/?pin={ledPin}&intensity={intensityOfBright}&frequency={numberOfPulses}&time={totalTimeTilTurnOffAgain}
const char pattern[] = "^bright:\\/\\/swift-yah.io\\/\?pin=(%d+)&intensity=(%d+)&frequency=(%d+)&time=(%d+)$";

// Each index indicates the position on regex groups of a value.

const byte pinLedIndex = 0;
const byte intensityIndex = 1;
const byte frequencyIndex = 2;
const byte timeIndex = 3;

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
  Serial.begin(baudRateSerialOutput);
  printOnSerial(startingOutputMessage, false);
}

// Convert from 0 - 100 to 0 - 255.
short convertToBrightness(byte value) {
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
  byte brightness;

  // The time between each pulse.
  long milisecondsBetweenPulses;

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

bool checkRegexForProtocol(String data) {
  // A wrapper for String data.
  char dataArray[maximumSizeOfBuffer];

  // Will store the matched result.
  char *matchedResult;

  // Will store each data recovered.
  char *pinLedData;
  char *intensityData;
  char *frequencyData;
  char *timeData;

  // Convert our String to char[].
  data.toCharArray(dataArray, maximumSizeOfBuffer);
  
  // Match state object.
  MatchState matchState;

  // Buffer must be larger enough to hold expected string, or malloc it.
  char resultBuffer[maximumSizeOfBuffer];

  // Setting up the string that we are searching.
  matchState.Target(dataArray);

  // Let's search.
  char matchResult = matchState.Match(pattern, 0);

  switch (matchResult) {
    case REGEXP_MATCHED:
      // Stores the matched on.
      matchedResult = matchState.GetMatch(resultBuffer);

      if (matchState.level < amountOfValuesOnProtocol) {
        printOnSerial(weCouldNotFindAllData, true);
        
        return false;
      }
      
      pinLedData = matchState.GetCapture(resultBuffer, pinLedIndex);
      intensityData = matchState.GetCapture(resultBuffer, intensityIndex);
      frequencyData = matchState.GetCapture(resultBuffer, frequencyIndex);
      timeData = matchState.GetCapture(resultBuffer, timeIndex);

      brightPin.ledPin = (int) pinLedData;
      brightPin.intensity = (int) intensityData;
      brightPin.frequency = (short) frequencyData;
      brightPin.timeToOff = (short) timeData;

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
    for (byte b = 0; b <= brightPin.frequency; b++) {
      analogWrite(brightPin.ledPin, brightPin.brightness);
      delay(brightPin.milisecondsBetweenPulses);
      analogWrite(brightPin.ledPin, minimumIntensity);
    }
  }
}

// Start to work on data received from bluetooth serial.
void workOnDataReceived(String data) {
  turnOnLed(statusPin);

  printOnSerial(showDataReceived, false);
  printOnSerial(data, false);

  bool working = checkRegexForProtocol(data);

  if (working) {
    processProtocolData();
  } else {
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

