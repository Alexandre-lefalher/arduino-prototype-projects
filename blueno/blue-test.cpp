#include <arduino.h>
#include <Wire.h>

// #define Serial0 Serial0
// #define Serial Serial


char command[128] = {0};
char tempChar[5] = {0};
char character;


void setup(){

	Serial.begin(115200);
	//Serial.begin(155200);

	// wait for USB Serial
  while (!Serial) {}
  // read any input
  delay(200);
  while (Serial.read() >= 0) {}

	Serial.println("Serials begun");
}

void loop(){

	// while(Serial.available()){
	//     character = Serial.read();
	//     Serial.println(character);
	// }

	while(Serial.available()){
    character = Serial.read();
    //character = Serial.read();
    tempChar[0] = character;
    //Serio_USB.print(character,HEX);
    if(character == 'e'){
      strcat(command,tempChar);
      //FLAG_MESSAGE = true;
      Serial.println(command);
      memset(command,0,strlen(command));

    } else if(strlen(command)<100){
      strcat(command,tempChar);
    } else {
      //overcrossing the length of the char array
      memset(command,0,strlen(command));
      //Serio_USB.println("Parser overload, clearing, & message lost");
    }
    memset(tempChar,0,strlen(tempChar));


  }// End of BT Serial While

}