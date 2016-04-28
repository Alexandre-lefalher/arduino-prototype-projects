#include <Arduino.h>
#include "ChibiOS_AVR.h"
//#include <string.h>
#include <Wire.h>
//#include "Moti.h"

#include <Adafruit_NeoPixel.h>

#include <ctype.h>

void mainThread(void);
//Definining Pin types for the device
#define Serio_BT Serial
#define Serio_USB Serial1
//LEDS
#define PIN_LED_BELT   46
#define PIN_LED_EARS   8
#define NUMPIXELS      20

Adafruit_NeoPixel pixel_belt = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_BELT);
Adafruit_NeoPixel pixel_ears = Adafruit_NeoPixel(2,PIN_LED_EARS);

// Motor Pins TODO:define/explain theses pins
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

//Action Flags
volatile bool FLAG_GAMES = false;
volatile bool FLAG_MOTOR = false;
volatile bool FLAG_LEDS = false;

//REVIEW:verify utility of theses verifications characters
char validChars[]={'N','L','R','B','O','H'};
int size_validChars = (int)(sizeof(validChars) / sizeof(validChars[0]));

char listStarterChar[] = {'<'};
int size_listStarterChar = (int)(sizeof(listStarterChar) / sizeof(listStarterChar[0]));

char listFinisherChar[] = {'>'};
int size_listFinisherChar = (int)(sizeof(listFinisherChar) / sizeof(listFinisherChar[0]));

//REVIEW: string orders might need different encapsulation

char command[256] = {0};
char order[256] = {0};
// String command = "";
// String order = "";

// Mutex for atomic access to data.
MUTEX_DECL(serialMutex);

//Verification functions:
bool isStarterChar(char tested_char){
  int i = 0;
  for(i=0;i<size_listStarterChar;i++){
    if(listStarterChar[i] == tested_char){
      return true;
    }
  }
  return false;
}

bool isFinisherChar(char tested_char){
  int i = 0;
  for(i=0;i<size_listFinisherChar;i++){
    if(listFinisherChar[i] == tested_char){
      return true;
    }
  }
  return false;
}

void printOrder(){
  chMtxLock(&serialMutex);
  Serio_BT.println(order);
  chMtxUnlock();
}


//------------------------------------------------------------------------------
// thread 1 - Capture Serial data, and parse commands - normal Prio
// 64 byte stack beyond task switch and interrupt needs
//static THD_WORKING_AREA(waThread1, 64);
static WORKING_AREA(waThread1, 256);

static msg_t Thread1(void* arg) {

    // String tempString = "tmpStr";
    // String tempOrder = "yo";
    char tempString[5] = {0};
    char tempOrder[50] = "";


    //delay(50);

    while (!chThdShouldTerminate()) {

      while(Serio_BT.available()){

        char caracter = (char) Serio_BT.read();

        chMtxLock(&serialMutex);
        tempString[0] = caracter;
        chMtxUnlock();
        Serio_BT.print("inThd1-inputChar:");
        Serio_BT.println(tempString);


        if(isStarterChar(caracter)){
          //tempOrder += (char)caracter;
          //tempOrder += tempString;
          strcpy(tempOrder,tempString);

          Serio_BT.print("inThd1-Order:");
          Serio_BT.println(tempOrder);

        } else if (isupper(caracter)){
          strcat(tempOrder,tempString);
          //tempOrder += tempString;
          //tempOrder += (char)caracter;
        } else if (isFinisherChar(caracter)){

          //tempOrder += tempString;
          strcat(tempOrder,tempString);
          //tempOrder += (char)caracter;
          Serio_BT.print("inThd1-Finished-TempOrder:");
          Serio_BT.println(tempOrder);

          //chMtxLock(&serialMutex);
          //order = tempOrder;
          strcpy(order,tempOrder);
          //chMtxUnlock();
          Serio_BT.print("inThd1-Finished-Order:");
          Serio_BT.println(order);


          FLAG_GAMES = true;

        }
        else {
          memset(tempOrder,0,strlen(tempOrder));
          memset(tempString,0,strlen(tempString));
        }

      }

      //REVIEW: mutexes maybe not necessary
      // // Lock access to data.
      // chMtxLock(&dataMutex);
      // order += tempChar;
      // // Unlock data access.
      // chMtxUnlock(&dataMutex);

      // Sleep for 50 milliseconds.
      chThdSleepMilliseconds(20);

  }
  return (msg_t)0;
}
//------------------------------------------------------------------------------
// thread 2 - Verify raised flags and execute apporpriate orders
// 100 byte stack beyond task switch and interrupt needs
//static THD_WORKING_AREA(waThread2, 100);
static WORKING_AREA(waThread2, 256);

static msg_t Thread2(void* arg) {

  // print count every second
  while (!chThdShouldTerminate()) {

    if(FLAG_GAMES){
      FLAG_GAMES = false;
      Serio_BT.print("inThd2-Order:");
      Serio_BT.println(order);
      //if(strlen(order)>0) || order[1];

      if(strlen(order)>0){
        switch (order[1]) {
          case 'N':
          Serio_BT.println("inThd2-case: N");
          break;
          case 'L':
          Serio_BT.println("inThd2-case: L");
          break;
          case 'R':
          Serio_BT.println("inThd2-case: R");
          break;
        }
      }

      memset(order, 0, sizeof(order));
      //order = "";
    }

    if(FLAG_LEDS){

    }
    // Sleep for 50 milliseconds.
    chThdSleepMilliseconds(20);
  }
  return (msg_t)0;
}
//------------------------------------------------------------------------------
// TODO: thread 3 - Mouvement Thread - Applies mouvement from order to the wheels
// 64 byte stack beyond task switch and interrupt needs
// static THD_WORKING_AREA(waThread1, 64);
//
// static THD_FUNCTION(Thread1 ,arg) {
//   pinMode(LED_PIN, OUTPUT);
//
//   // Flash led every 200 ms.
//   while (1) {
//     // Turn LED on.
//     digitalWrite(LED_PIN, HIGH);
//
//     // Sleep for 50 milliseconds.
//     chThdSleepMilliseconds(50);
//
//     // Turn LED off.
//     digitalWrite(LED_PIN, LOW);
//
//     // Sleep for 150 milliseconds.
//     chThdSleepMilliseconds(150);
//   }
// }
//------------------------------------------------------------------------------
// TODO:thread 4 - SensorData record/send Thread -
// 64 byte stack beyond task switch and interrupt needs
// static THD_WORKING_AREA(waThread1, 64);
//
// static THD_FUNCTION(Thread1 ,arg) {
//   pinMode(LED_PIN, OUTPUT);
//
//   // Flash led every 200 ms.
//   while (1) {
//     // Turn LED on.
//     digitalWrite(LED_PIN, HIGH);
//
//     // Sleep for 50 milliseconds.
//     chThdSleepMilliseconds(50);
//
//     // Turn LED off.
//     digitalWrite(LED_PIN, LOW);
//
//     // Sleep for 150 milliseconds.
//     chThdSleepMilliseconds(150);
//   }
// }

//****************************************************************************//
// Main thread and Arduino setup

void setup(){

  //Initiliating Pin Modes
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  digitalWrite(M1,HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, 0);
  analogWrite(E2, 0);

  //Initialising Communication
  Serio_USB.begin(9600);
  Serio_BT.begin(115200);
  Wire.begin();

  pixel_belt.begin();
  pixel_ears.begin();

  // wait for USB Serial
  while (!Serio_USB) {}
  // read any input
  delay(200);
  while (Serio_USB.read() >= 0) {}

  //switch all leds off
  pixel_belt.show();
  pixel_ears.show();

  chBegin(mainThread);
  // chBegin never returns, main thread continues with mainThread()
  while(1) {}

}

// main thread runs at NORMALPRIO
void mainThread() {

  // start blink thread
  (void)chThdCreateStatic(waThread1, sizeof(waThread1),
                          NORMALPRIO, Thread1, NULL);
  // start print thread
  (void)chThdCreateStatic(waThread2, sizeof(waThread2),
                          NORMALPRIO, Thread2, NULL);

  // increment counter
  while (1) {
    // must insure increment is atomic in case of context switch for print
    // should use mutex for longer critical sections
    // noInterrupts();
    // count++;
    // interrupts();
  }
}
//------------------------------------------------------------------------------
void loop() {
 // not used
}
