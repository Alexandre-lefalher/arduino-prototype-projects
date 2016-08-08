#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_NeoPixel.h>
#include <ctype.h>
#include "FreeIMU.h"

#define PIN_LED_BELT 46
#define PIN_LED_EARS  8
#define NB_LED_BELT 20
#define NB_LED_EAR 2
#define Serial_BT Serial1
#define Serial_USB Serial

#define FIMU_ACC_ADDR ADXL345_ADDR_ALT_LOW

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NB_LED_BELT, PIN_LED_BELT);
Adafruit_NeoPixel ears = Adafruit_NeoPixel(NB_LED_EAR,PIN_LED_EARS);

ADXL345 acc;

void setup()
{

  acc.init(FIMU_ACC_ADDR);

  int setRange = 4;
	acc.setRangeSetting(setRange);
	delay(1000);
	acc.getRangeSetting(&range);

  Serial1.begin(115200);
  Serial.begin(9600);

  pixels.begin();
  ears.begin();

  Wire.begin();

  main();

}

void main()
{
  while (TRUE){

    read();
    delay(80);
  }

}

void read()
{
  bool catchSerial = FALSE;

  char inputChar = '';
  String inputString = "";

  while (Serial_BT.available()) {
    inChar = (char) Serial_BT.read();

    //If start Character has been read
    if(inChar == "[" ){
      //Start creating the data string
      catchSerial == TRUE;
    }

    if(catchSerial){
      inputString += inChar;
    }
      //if Stop character has been read
    if (inChar == ']'){
      catchSerial == FALSE;
      if(inputString != "" && inputString != 0){
        //Retrieve the Command out of it
        findCommand(inputString);
      }
    }

  }

}


void findCommand(String order){
  //Retrieve first character after startbit
  int ledID = 0;
  int R=0;
  int G=0;
  int B=0;

  int motorStr = 0;

  switch (order[1]) {
    case 'L':
    //Verify String length
    if(order.length() >= 12 ){
      ledID = order.substring(1,3);
      R = order.substring(4,6);
      G = order.substring(7,9);
      B = order.substring(10,12);
    }

    //LaunchLedCommand
    return;
    case 'M':
    //LaunchMotor attribution
    return;
  }
}
