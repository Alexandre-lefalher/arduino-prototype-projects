#include <Arduino.h>
#include <Wire.h>

// #include "Sensors.h"
#include "DriveSystem.h"
#include "Moti.h"
#include <Adafruit_NeoPixel.h>
#define PIN 6
# define Serio Serial

#define FIMU_ACC_ADDR ADXL345_ADDR_ALT_LOW

ADXL345 acc;

int DTC;
byte range;
#define NB_LEDS 30

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

String inputString ="";
int lastR = 0;
int lastG = 0;
int lastB = 0;


/* Read leds data send by the Qt app */

/********* NOT DEFINITIVE ***********/

void read(){
	
	while(Serial.available()){

		char inChar = (char) Serial.read();
			
		inputString += inChar;

		if(inChar == '\n' && inputString[0] == '\t'){
			int SpaceIndex = inputString.indexOf(' ');
			//  Search for the next Space just after the first
			int secondSpaceIndex = inputString.indexOf(' ', SpaceIndex+1);
			//  Search for the next Space just after the first
			int thirdSpaceIndex = inputString.indexOf(' ', secondSpaceIndex+1);
			int endIndex = inputString.indexOf('\n', secondSpaceIndex+1);

			String firstValue = inputString.substring(1, SpaceIndex);
			String secondValue = inputString.substring(SpaceIndex+1, secondSpaceIndex);
			String thirdValue = inputString.substring(secondSpaceIndex,thirdSpaceIndex);
			String quadValue = inputString.substring(thirdSpaceIndex,endIndex);
			int r = firstValue.toInt();
			int g = secondValue.toInt();
			int b = thirdValue.toInt();
			int alpha = quadValue.toInt();
			
			if(r>=0 && r<=255 && g>=0 && g<=255 && b>=0 && b<=255){
				// String complete
				for(int i =0;i<NB_LEDS;i++){
					strip.setPixelColor(i, r, g, b);
				
				}
				lastR = r;
				lastG = g;
				lastB = b;
				
			}else{
				for(int i =0;i<NB_LEDS;i++){
					strip.setPixelColor(i, lastR, lastG, lastB);
				
				}
			}
			inputString="";
		}else{
			for(int i =0;i<NB_LEDS;i++){
				strip.setPixelColor(i, lastR, lastG, lastB);
			}
		}
		//strip.setBrightness(64);
		strip.show();
	}
}
void chSetup() {

	acc.init(FIMU_ACC_ADDR);
	int setRange = 4;
	acc.setRangeSetting(setRange);
	delay(1000);
	acc.getRangeSetting(&range);

int xyz[3];
float xyz_gyr[3];

	while (TRUE) {
		/* Send data to Qt app */
		acc.readAccel(&xyz[0], &xyz[1], &xyz[2]);
		Serial.print("A ");
		Serial.print(xyz[0]);
		Serial.print(" ");
		Serial.print(xyz[1]);
		Serial.print(" ");
		Serial.print(xyz[2]);
		Serial.print(" ");

		acc.get_Gxyz(xyz_gyr);
		Serial.print(xyz_gyr[0]);
		Serial.print(" ");
		Serial.print(xyz_gyr[1]);
		Serial.print(" ");
		Serial.print(xyz_gyr[2]);

		/* Read leds data */
		read();

		delay(100);
	 }
}


void setup() {
	Serio.begin(115200);
	while (!Serio);
  	strip.begin();
  	strip.show(); // Initialize all pixels to 'off'

	Wire.begin();
	delay(500);

	chBegin(chSetup);

	while (TRUE);
}

void loop() { }
