#include <Arduino.h>
#include <Wire.h>

// #include "Sensors.h"
#include "DriveSystem.h"
#include "Moti.h"
#include <Adafruit_NeoPixel.h>
#include <ctype.h>

#define PIN 46
#define Serio Serial1

#define FIMU_ACC_ADDR ADXL345_ADDR_ALT_LOW

ADXL345 acc;

int DTC;
byte range;
#define NB_LEDS 30
#define	LED_ANGLE_PADDING 360/NB_LEDS
int threshold = 120;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NB_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int lastR = 0;
int lastG = 0;
int lastB = 0;

int xyz[3];
float xyz_gyr[3];

boolean playingSound = FALSE;
boolean showingLeds = FALSE;
char waitingParams = '0';

/*
 * Check if a number is valid
 */
boolean isValidNumber(String str){
	boolean res = true;
	for(unsigned int i=0;i<str.length();i++) {
		if(!(isDigit(str.charAt(i)) || str.charAt(i) == ' ')) res = false;
	}
	return res;
}


/*
 * Show the leds
 */
void showLeds(String inputString){
	int SpaceIndex = inputString.indexOf(' ');
	//  Search for the next Space just after the first
	int secondSpaceIndex = inputString.indexOf(' ', SpaceIndex+1);
	int thirdSpaceIndex = inputString.indexOf(' ', secondSpaceIndex+1);
	int endIndex = inputString.indexOf(' ', thirdSpaceIndex+1);

	String firstValue = inputString.substring(1, SpaceIndex);
	String secondValue = inputString.substring(SpaceIndex+1, secondSpaceIndex);
	String thirdValue = inputString.substring(secondSpaceIndex+1,thirdSpaceIndex);
	String quadValue = inputString.substring(thirdSpaceIndex+1,endIndex);

	// to restablish the original values
	int r = firstValue.toInt()-1;
	int g = secondValue.toInt()-1;
	int b = thirdValue.toInt()-1;
	int alpha = quadValue.toInt()-1;

	if(!(lastR == r && lastG == g && lastB == b)){
		if(r>=0 && r<=255 && g>=0 && g<=255 && b>=0 && b<=255 && alpha < 360 && alpha >= 0 && isValidNumber(firstValue) && isValidNumber(secondValue) && isValidNumber(thirdValue) && isValidNumber(quadValue)){
			// String complete
			for(int i =0;i<NB_LEDS;i++){
				int led_angle = LED_ANGLE_PADDING*i;
				// we light up leds according to the distance between them and the angle measured by the giro, the closer the led
					// the more shiny
				float coeff = 1.0; // max = 11

				//conditions had to be written to resolve issues due to angles near 0° and 360°
				if(alpha < threshold){
					if(led_angle >= alpha+360-threshold || led_angle < alpha + threshold){
						if(led_angle >= alpha+360-threshold){
							coeff += 10*abs(alpha-(led_angle-360))/threshold;
						}else{
							coeff += 10*abs(alpha-led_angle)/threshold;
						}
						strip.setPixelColor(i, (int) r/coeff, (int) g/coeff, (int) b/coeff);
					}else{
						strip.setPixelColor(i, 0, 0, 0);
					}
				} else if(alpha >= 360 - threshold){
					if(led_angle <= alpha-360+threshold || led_angle > alpha - threshold){
						if(led_angle <= alpha-360+threshold){
							coeff += 10*abs(alpha-(led_angle+360))/threshold;
						}else{
							coeff += 10*abs(alpha-led_angle)/threshold;
						}
						strip.setPixelColor(i, (int) r/coeff, (int) g/coeff, (int) b/coeff);
					}else{
						strip.setPixelColor(i, 0, 0, 0);
					}
				} else {
					if(led_angle >= alpha-threshold && led_angle <= alpha+threshold){
						coeff += 10*abs(alpha-led_angle)/threshold;
						strip.setPixelColor(i, (int) r/coeff, (int) g/coeff, (int) b/coeff);
					}else{
						strip.setPixelColor(i, 0, 0, 0);
					}
				}
			}
			lastR = r;
			lastG = g;
			lastB = b;
			strip.show();
		}
	}
}


/*
 * Hide the leds
 */
void hideLeds(){
	for(int i =0;i<NB_LEDS;i++){
		strip.setPixelColor(i, 0, 0, 0);
	}
	strip.show();
}


/* Read leds data sent by the Qt app */
void read(){
	String inputString ="";
	while(Serio.available()){

		char inChar = (char) Serio.read();

		inputString += inChar;

		//Reset command
		if(inChar == '\f'){
			playingSound = FALSE;
			showingLeds = FALSE;
			hideLeds();
			return;
		}

		//If we are expecting parameters
		if(waitingParams != '0' && inChar == '\r'){
			switch(waitingParams){
				//Parameters for the 'Turn on leds' command
				case 'L':
					int SpaceIndex = inputString.indexOf(' ');
					String firstValue = inputString.substring(0, SpaceIndex);
					int tmpThreshold = firstValue.toInt()-1;
					if(tmpThreshold > 0){
						threshold = tmpThreshold;
					}
					waitingParams = '0';
					return;
			}
		} else {
			//Command to turn on the music
			if(inChar == 'M'){
				playingSound = TRUE;
				return;
			}
			//Command to turn on the leds
			if(inChar == 'L'){
				showingLeds = TRUE;
				waitingParams = 'L';
				return;
			}
			//Command to turn off the music
			if(inChar == 'N'){
				playingSound = FALSE;
				return;
			}
			//Command to turn off the leds
			if(inChar == 'K'){
				showingLeds = FALSE;
				hideLeds();
				return;
			}
			//No special command so we show the leds (if turned on)
			if(inChar == '\n' && inputString[0] == '\t' && showingLeds){
				showLeds(inputString);
				inputString = "";
				return;
			}
		}
	}
}


/* Send the data from the gyro and accel to the Qt app */
void sendData(){
	acc.readAccel(&xyz[0], &xyz[1], &xyz[2]);
	Serio.print(xyz[0]);
	Serio.print(" ");
	Serio.print(xyz[1]);
	Serio.print(" ");
	Serio.print(xyz[2]);
	Serio.print(" ");

	acc.get_Gxyz(xyz_gyr);
	Serio.print(xyz_gyr[0]);
	Serio.print(" ");
	Serio.print(xyz_gyr[1]);
	Serio.print(" ");
	Serio.print(xyz_gyr[2]);
	Serio.print(" A");
}


void chSetup() {

	acc.init(FIMU_ACC_ADDR);
	int setRange = 4;
	acc.setRangeSetting(setRange);
	delay(1000);
	acc.getRangeSetting(&range);

	String inputString = "";

	while (TRUE) {

		/* Read data received from the Qt app */
		read();
		delay(80);

		/* Send data to the Qt app */
		if(playingSound || showingLeds)
			sendData();

		delay(20);
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
