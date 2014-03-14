#include <SoftwareSerial.h>
#include <Adafruit_Thermal.h>

const int printerRX         = 1;  // This is the green printer wire
const int printerTX         = 3;  // This is the yellow printer wire
const int iotpPin           = 7;  // To also work w/IoTP printer

const int ledGreen          = 13; // This is the green LED wire
const int ledYellow         = 12; // This is the yellow LED wire
const int ledRed            = 11; // This is the red LED wire

const byte statusOkay       = 0B00000100;
const byte statusPrinting   = 0B00000010;
const byte statusError      = 0B00000001;

const int basicMessageLength = 140;

bool hasPaper = 0;

Adafruit_Thermal printer(printerRX, printerTX);

void setup() {
  // configure pin for printer setup
  //pinMode(iotpPin, OUTPUT); digitalWrite(iotpPin, LOW);
  
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  Serial.begin(19200);
  
  setStatusLights(statusOkay);
  
  printer.begin();
  hasPaper = printer.hasPaper();
}

void loop() {
  Serial.println(1);
  if (Serial.available() > 0) {
    setStatusLights(statusPrinting);
    
    char incomingData[basicMessageLength + 1];
    int incomingBytes = Serial.readBytesUntil(13, incomingData, basicMessageLength);
    incomingData[incomingBytes] = 0;
    
    //Serial.println(incomingData); 
    printMessage(incomingData);
    setStatusLights(statusOkay);
  }
}

void setStatusLights(byte statusLights) {
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  
  if(statusLights & statusOkay) { 
    digitalWrite(ledGreen, HIGH);
  } 
  
  if(statusLights & statusPrinting) {
    digitalWrite(ledYellow, HIGH);
  }
  
  if(statusLights & statusError) {
    digitalWrite(ledRed, HIGH);
  }
}

void printMessage(char* message) {
  //printer.begin();
  
  printer.println(message);
  printer.feed(1);
  
  printer.sleep();      // Tell printer to sleep
  printer.wake();       // MUST call wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults
}


