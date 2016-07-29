#ifndef MESSAGES_H
#define MESSAGES_H

#include <Adafruit_NeoPixel.h>
#include <stdint.h>


class Msg_Led{
private:
  int id_number;
  int color_red;
  int color_green;
  int color_blue;

  bool contains_msg;

public:
  //Constructors
  Msg_Led(void);
  Msg_Led(int id, int red, int green, int blue);
  //Methods
  void set_msg_values(int id, int red, int green, int blue);
  void clearMsgValues(void);

  uint32_t getColor(void);
  int getId(void);
  bool isMessage(void);
};

class Msg_Motor{
private:
  int value;
  bool contains_msg;


public:
  Msg_Motor(void);
  Msg_Motor(int speed);
  bool isMessage(void);

  void setMsgValues(int speed);
  void clearMsgValues(void);

  int getSpeed(void);
};

#endif
