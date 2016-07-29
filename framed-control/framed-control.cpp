
#include "frame-control.h"
#include <stdio.h>

void setup(){
 command_id order1 = LED_BELT;
 command_id order2 = MOTOR_LEFT;
 command_id order3 = 0x21;
}

void main(){
  while(TRUE){

    printf("%x\n",order1);
    printf("%x\n",order2);
    printf("%x\n",order3);
    delay(20);
  }
}
