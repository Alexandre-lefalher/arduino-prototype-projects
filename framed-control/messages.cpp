
#include "messages.h"

Msg_Led::Msg_Led(){
  id_number = 0;
  color_red = 0;
  color_green = 0;
  color_blue = 0;

  contains_msg = false;
}

Msg_Led::Msg_Led(int id, int red, int green, int blue){
  id_number = id;
  color_red = red;
  color_green = green;
  color_blue = blue;

  contains_msg = true;
}

void Msg_Led::set_msg_values(int id, int red, int green, int blue){
  id_number = id;
  color_red = red;
  color_green = green;
  color_blue = blue;

  contains_msg = true;
}

void Msg_Led::clearMsgValues(){
  id_number = 0;
  color_red = 0;
  color_green = 0;
  color_blue = 0;

  contains_msg = false;
}

uint32_t Msg_Led::getColor(){

  return Adafruit_NeoPixel::Color(color_red,color_green,color_blue);
}

int Msg_Led::getId(){
  return id_number;
}

bool Msg_Led::isMessage(){
  return contains_msg;
}

//Methodes for the Motor Messages
Msg_Motor::Msg_Motor(){
  value = 0;
  contains_msg = false;
}

Msg_Motor::Msg_Motor(int speed){
  value = speed;
  contains_msg = true;
}

void Msg_Motor::setMsgValues(int speed){
  value = speed;
  contains_msg = true;
}

void Msg_Motor::clearMsgValues(){
  value = 0;
  contains_msg = false;
}

int Msg_Motor::getSpeed(){
  return value;
}

bool Msg_Motor::isMessage(){
  return contains_msg;
}
