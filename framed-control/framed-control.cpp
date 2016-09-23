#include <Arduino.h>
#include <Wire.h>
#include "framed-control.h"
#include <stdio.h>
#include <stdlib.h>

#include <Adafruit_NeoPixel.h>
// command_id order1;
// command_id order2;
// command_id order3;

volatile bool FLAG_MESSAGE = false;

char command[128] = {0};
char tempChar[5] = {0};
char character;

String str_command("_");

Adafruit_NeoPixel pixel_belt = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_BELT);
Adafruit_NeoPixel pixel_ears = Adafruit_NeoPixel(2,PIN_LED_EARS);

Msg_Led led_message;
Msg_Motor motor_message;




void setup(){

  //Initiliating Pin Modes
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  digitalWrite(LEFT_MOTOR_DIRECTION,HIGH);
  digitalWrite(RIGHT_MOTOR_DIRECTION, HIGH);
  analogWrite(LEFT_MOTOR_POWER, 0);
  analogWrite(RIGHT_MOTOR_POWER, 0);

  //Initialising Communication
  //Serio_USB.begin(115200);
  Serio_BT.begin(115200);
  Wire.begin();

  //Initialising led strips
  // pixel_belt
  // pixel_ears
  //Starting the led strips
  pixel_belt.begin();
  pixel_ears.begin();


  led_message = Msg_Led();
  motor_message = Msg_Motor();

  // wait for USB Serial
  while (!Serio_USB) {}
  // read any input
  delay(200);
  while (Serio_USB.read() >= 0) {}

  Serio_USB.println("show in setup");
  //switch all leds off
  pixel_belt.show();
  pixel_ears.show();

  character = ' ';

}

void loop(){

  //Inputs
  //Capture BT Messages
  while(Serio_BT.available()){
    character = Serio_BT.read();
    //character = Serio_BT.read();
    tempChar[0] = character;
    //Serio_USB.print(character,HEX);
    if(character == '\n'){
      strcat(command,tempChar);
      FLAG_MESSAGE = true;

    } else if(strlen(command)<100){
      strcat(command,tempChar);
    } else {
      //overcrossing the length of the char array
      memset(command,0,strlen(command));
      Serio_USB.println("Parser overload, clearing, & message lost");
    }
    memset(tempChar,0,strlen(tempChar));

  }// End of BT Serial While

  //Processes
  if(FLAG_MESSAGE != 0){

    if(strlen(command)>0){
      //Transfert char array to be able to manipulate String functionalities

      str_command = command;
      switch (command[0]) {
        case LED_BELT:
          if(strlen(command) == MSG_LED_SIZE){
            //use std string substring
             Serio_BT.println(command);
            // Serio_USB.println(str_command.substring(1,3));
            // Serio_USB.println(str_command.substring(3,6));
            // Serio_USB.println(str_command.substring(6,9));
            // Serio_USB.println(str_command.substring(9,12));
            //Apply Lighting values
            //ID,Red, Green, Blue
            //cf, frame document
            led_message.set_msg_values(str_command.substring(1,3).toInt(),
                                        str_command.substring(3,6).toInt(),
                                        str_command.substring(6,9).toInt(),
                                        str_command.substring(9,12).toInt());

            displayBelt();

          }
        break;

        case LED_EARS:
        if(strlen(command) == MSG_LED_SIZE){

          led_message.set_msg_values(str_command.substring(1,3).toInt(),
                                      str_command.substring(3,6).toInt(),
                                      str_command.substring(6,9).toInt(),
                                      str_command.substring(9,12).toInt());

          displayEars();
          Serio_USB.println(strlen(command));
          Serio_USB.println("EarsMan");
        }
        break;

        case MOTOR_LEFT:
        if(strlen(command) == MSG_MOTOR_SIZE){
          motor_message.setMsgValues(str_command.substring(1,5).toInt());
          applyLeftMotorValue();
          Serio_USB.println(strlen(command));
           Serio_USB.println("MOTOR_LEFT");
          // Serio_USB.println(str_command.substring(1,5).toInt());
        }
        break;

        case MOTOR_RIGHT:
        if(strlen(command) == MSG_MOTOR_SIZE){
          motor_message.setMsgValues(str_command.substring(1,5).toInt());
          applyRightMotorValue();
          Serio_USB.println(strlen(command));
           Serio_USB.println("MOTOR_RIGHT");
          // Serio_USB.println(str_command.substring(1,5).toInt());
        }
        break;
      }

      FLAG_MESSAGE = false;
      memset(command,0,strlen(command));

    }
  }

  //Outputs
  //Get sensor values and reprint on Serio_BT

}//End of Loop()

void displayBelt(){

    if(led_message.isMessage()){
    pixel_belt.setPixelColor(led_message.getId(),led_message.getColor());
    pixel_belt.show();
    //clear message for next instruction
    led_message.clearMsgValues();
    }
}

void displayEars(){

  if(led_message.isMessage()){
    Serio_USB.println("inDisplayBelt");
    pixel_ears.setPixelColor(led_message.getId(),led_message.getColor());
    pixel_ears.show();
    //clear message for next instruction
    led_message.clearMsgValues();
    }
}

void clearColorBelt(){
  uint32_t off = pixel_belt.Color(0,0,0);
  for(int i = 0; i < pixel_belt.numPixels();i++){
    pixel_belt.setPixelColor(i,off);
  }
  pixel_belt.show();
}

void applyLeftMotorValue(){
  int value = 0;
  if(motor_message.isMessage()){
    value=motor_message.getSpeed();

    if(value<0){
            digitalWrite(LEFT_MOTOR_DIRECTION,LOW);
            analogWrite(LEFT_MOTOR_POWER, abs(value));
    }
    else{
            digitalWrite(LEFT_MOTOR_DIRECTION,HIGH);
            analogWrite(LEFT_MOTOR_POWER, abs(value));
    }
    motor_message.clearMsgValues();
  }
}

void applyRightMotorValue(){
  int value = 0;
  if(motor_message.isMessage()){
    value=motor_message.getSpeed();

    if(value<0){
            digitalWrite(RIGHT_MOTOR_DIRECTION,LOW);
            analogWrite(RIGHT_MOTOR_POWER, abs(value));
    }
    else{
            digitalWrite(RIGHT_MOTOR_DIRECTION,HIGH);
            analogWrite(RIGHT_MOTOR_POWER, abs(value));
    }
    motor_message.clearMsgValues();
  }

}
