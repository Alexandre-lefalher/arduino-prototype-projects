
#ifndef FRAMED_CONTROL_H
#define FRAMED_CONTROL_H

#include <Adafruit_NeoPixel.h>

#include "messages.h"

//Definining Pin types for the device
#define Serio_BT Serial
#define Serio_USB Serial
//LEDS
#define PIN_LED_BELT   46
#define PIN_LED_EARS   8
#define NUMPIXELS      20

//Motor definition, by Arduino's definition and Pins
//M1 corresponds to the Left Motor's direction
//M2 corresponds to the Right Motor's direction
//Mn value is LL_LOW for reverse & LL_HIGH for forward.
//Respectively the E values represent strength applied
#define LEFT_MOTOR_DIRECTION 4
#define LEFT_MOTOR_POWER 5
#define RIGHT_MOTOR_DIRECTION 7
#define RIGHT_MOTOR_POWER 6

#define MSG_LED_SIZE 13
#define MSG_MOTOR_SIZE 6

typedef enum {
  LED_EARS = 0x30,
  LED_BELT = 0x31,
  MOTOR_LEFT = 0x40,
  MOTOR_RIGHT = 0x41
}command_id;

void displayBelt(void);
void displayEars(void);
void clearColorBelt(void);

void applyRightMotorValue(void);
void applyLeftMotorValue(void);

// enum class Command_id{
//   LED_EARS = 0x10,
//   LED_BELT = 0x11,
//   MOTOR_LEFT = 0x20,
//   MOTOR_RIGHT = 0x21
// };


#endif
