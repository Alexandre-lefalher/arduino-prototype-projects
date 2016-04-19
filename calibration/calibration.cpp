#include "Moti.h"
#include <Arduino.h>
#include <Wire.h>

#include "CommunicationUtils.h"
#include <ctype.h>

#include <SPI.h>

#define FIMU_ACC_ADDR ADXL345_ADDR_ALT_LOW
#define FIMU_ITG3200_ADDR ITG3200_ADDR_AD0_LOW

#define Serio Serial

ADXL345 acc;
ITG3200 gyr;

int acc_xyz[3];
int gyr_values[3];

int xyz_gyr[6];

char str[256];

/* Send the data from the gyro and accel to the Qt app */

void sendData(){
	//acc.readAccel(&xyz[0], &xyz[1], &xyz[2]);
	Serio.print(acc_xyz[0]);
	// Serio.print(" ");
	Serio.print(acc_xyz[1]);
	// Serio.print(" ");
	Serio.print(acc_xyz[2]);
	// Serio.print(" ");
	//acc.get_Gxyz(xyz_gyr);
	Serio.print(gyr_values[0]);
	// Serio.print(" ");
	Serio.print(gyr_values[1]);
	// Serio.print(" ");
	Serio.print(gyr_values[2]);
	// Serio.print(" A");
	Serio.print('\r\n');
}

char Serial_busy_wait() {
  while(!Serio.available()) {
    ; // do nothing unstil ready
  }
  return Serio.read();
}

void read(){
	
	char inChar;
	int range = 30;
	String inputString ="";

	while(true){
		if(Serio.available()){
				inChar = Serio.read();
				
				//inputString += inChar;
				if (inChar == 'b'){
				uint8_t count = Serial_busy_wait();
				Serio.print(count);
					for(uint8_t i=0; i<count; i++) {
						acc.readAccel(&xyz_gyr[0], &xyz_gyr[1], &xyz_gyr[2]);
						gyr.readGyroRaw(&xyz_gyr[3], &xyz_gyr[4], &xyz_gyr[5]);
						sendData();
						//writeArr(xyz_gyr,6,sizeof(uint8_t));
						Serio.println();
					}
					
				} else if(inChar=='v') {
		      sprintf(str, "FreeIMU calibration updated by %s, FREQ:%s, LIB_VERSION: %s, IMU: %s", FREEIMU_DEVELOPER, FREEIMU_FREQ, FREEIMU_LIB_VERSION, FREEIMU_ID);
		      Serio.print(str);
		      Serio.print('\n');
		    }
		}
	}
}




void chSetup(){
	acc.init(FIMU_ACC_ADDR);
	gyr.init(FIMU_ITG3200_ADDR);

	while (TRUE){
		read();
		delay(80);
		//sendData();
		//delay(20);
	}

}

void setup() {
	Serio.begin(115200);
	Wire.begin();
	delay(500);
	//chBegin(chSetup);

	acc.init(FIMU_ACC_ADDR);
	gyr.init(FIMU_ITG3200_ADDR);

	read();

	// while(true){
	// 	read();
	// 	//delay(20);
	// }


	while(TRUE);
}







void loop() {
}