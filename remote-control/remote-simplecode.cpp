/*#include <vector_math.h>
   #include <calibration.h>
   #include <CommunicationUtils.h>
 */

#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <Wire.h>

//#include <SoftwareSerial.h>
//*************************************************
// this code below allow the arduino to get an array of values as string following the model bellow :
//        [ sign 3digitleftwheel,
//          sign 3digitrightwheel,
//          3digitrightRedlight,
//          3digitrightBluelight,
//          3digitrightGreenlight,
//          3digitleftRedlight,
//          3digitleftBluelight,
//          3digitleftGreenlight ]
// Firstly , check if data available (cf. loop() --> if Serial1.available())
// Secondly, while data available, check the message is the correct on (while available: if "[" get digits && "," && "+" && "-" until "]")
// Thirdly, convert the obtained String into int for each values to control (getValuesfromBluetooth()) and be sure that no values exceed 255 (power troubles)
// Finally, set value to requested pin (go())
// wait 10 ms
// do that undefinitly!!!
//*************************************************
// INFO: 3digits code sur 8bits
//SoftwareSerial mySerial(16, 17); // RX, TX
/*       NEOPIXEL       */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN_LED_BELT   46
#define PIN_LED_EARS   8
#define NUMPIXELS      20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_BELT);
Adafruit_NeoPixel ears = Adafruit_NeoPixel(2,PIN_LED_EARS);

String json = "";
String command = "";
String order = "";
int i=0;
int values[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
//Ear Leds, pin access
int RL1 = 8;
int GL1 = 9;
int BL1 = 10;
int RL2 = 11;
int GL2 = 12;
int BL2 = 13;

//String leds;
float angles[3]; // yaw pitch roll
FreeSixIMU sixDOF = FreeSixIMU();
int rawSixDof[6];

void setup()
{
        pinMode(RL1, OUTPUT);
        pinMode(RL2, OUTPUT);
        pinMode(GL1, OUTPUT);
        pinMode(GL2, OUTPUT);
        pinMode(BL1, OUTPUT);
        pinMode(BL2, OUTPUT);
        pinMode(M1, OUTPUT);
        pinMode(M2, OUTPUT);
        digitalWrite(M1,HIGH);
        digitalWrite(M2, HIGH);
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        //open bluetooth serial
        Serial1.begin(115200);
        Serial.begin(9600);
        Serial.println("Type AT commands!");

        /*       NEOPIXEL          */
  #if defined (__AVR_ATtiny85__)
        if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
        pixels.begin();
        ears.begin();

        Wire.begin();


        delay(5);
        sixDOF.init(); //begin the IMU
        delay(5);
}
void show_led_circle(int number, int r, int g, int b) {
        pixels.setPixelColor(number, pixels.Color(r,g,b));
        pixels.show();
        return;
}
//********************** ORDER EACH PIN to set the requested value
void go(int value1, int value2, int ctr, int ctg, int ctb, int flr, int flg, int flb, int frr, int frg, int frb, int blr, int blg, int blb, int brr, int brg, int brb){
/*
   Serial.println("**************************************");
   Serial.print("CENTER : ");
   Serial.print(ctr);
   Serial.print(",");
   Serial.print(ctg);
   Serial.print(",");
   Serial.print(ctb);
   Serial.println("");
   Serial.print("FRONT LEFT : ");
   Serial.print(flr);
   Serial.print(",");
   Serial.print(flg);
   Serial.print(",");
   Serial.print(flb);
   Serial.println("");
   Serial.print("FRONT RIGHT : ");
   Serial.print(frr);
   Serial.print(",");
   Serial.print(frg);
   Serial.print(",");
   Serial.print(frb);
   Serial.println("");
   Serial.print("BOT LEFT : ");
   Serial.print(blr);
   Serial.print(",");
   Serial.print(blg);
   Serial.print(",");
   Serial.print(blb);
   Serial.println("");
   Serial.print("BOT RIGHT : ");
   Serial.print(brr);
   Serial.print(",");
   Serial.print(brg);
   Serial.print(",");
   Serial.println(brb);
   Serial.println("---------------------------------------");
 */

        if(value1<0) {
                digitalWrite(M1,LOW);
                analogWrite(E1, abs(value1));
        }
        else{
                digitalWrite(M1,HIGH);
                analogWrite(E1, abs(value1));
        }

        if(value2<0) {
                digitalWrite(M2, LOW);
                analogWrite(E2, abs(value2));
        }
        else{
                digitalWrite(M2, HIGH);
                analogWrite(E2, abs(value2));
        }
        //Invidual leds installed in the ears
        //analogWrite(RL1, ctr);
        //analogWrite(GL1, ctg);
        //analogWrite(BL1, ctb);
        //analogWrite(RL2, ctr);
        //analogWrite(GL2, ctg);
        //analogWrite(BL2, ctb);
        //Leds in series used in the ears
        ears.setPixelColor(0, pixels.Color(ctr,ctg,ctb));
        ears.setPixelColor(1, pixels.Color(ctr,ctg,ctb));
        ears.show();


        int i;
        for (i=0; i<=3; i++)
                show_led_circle(i, frr,frg,frb);
        for (i=4; i<=9; i++)
                show_led_circle(i, brr,brg,brb);
        for (i=10; i<=15; i++)
                show_led_circle(i, blr,blg,blb);
        for (i=16; i<=20; i++)
                show_led_circle(i, flr,flg,flb);
}
// ***********************************************From the obtain String, translate each value (substring) into int
void getValuesfromBluetooth(String command){
        if (command.length() != 71)
                return;
        Serial.println(command);
        values[0]=((command.substring(1,5)).toInt());
        values[1]=(command.substring(6,10)).toInt();
        values[2]=(command.substring(11,14)).toInt();
        values[3]=(command.substring(15,18)).toInt();
        values[4]=(command.substring(19,22)).toInt();
        values[5]=(command.substring(23,26)).toInt();
        values[6]=(command.substring(27,30)).toInt();
        values[7]=(command.substring(31,34)).toInt();
        values[8]=(command.substring(35,38)).toInt();
        values[9]=(command.substring(39,42)).toInt();
        values[10]=(command.substring(43,46)).toInt();
        values[11]=(command.substring(47,50)).toInt();
        values[12]=(command.substring(51,54)).toInt();
        values[13]=(command.substring(55,58)).toInt();
        values[14]=(command.substring(59,62)).toInt();
        values[15]=(command.substring(63,66)).toInt();
        values[16]=(command.substring(67,70)).toInt();
/*
   Serial.print(values[0] + values[1] + values[2]);
 */
        if (values[0]>=255) {values[0]=255; }
        if (values[1]>=255) {values[1]=255; }
        if (values[0]<=-255) {values[0]=-255; }
        if (values[1]<=-255) {values[1]=-255; }

        go(values[0],values[1],values[2],values[3],values[4],values[5],values[6],values[7],values[8],values[9],values[10],values[11],values[12],values[13],values[14],values[15],values[16]);
}
//********************Globlal micro controller thread
int counter = 0;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

void clearColorEars(){
  ears.setPixelColor(1,0,0,0);
  ears.setPixelColor(2,0,0,0);

  ears.show();
}

 // TODO: Integrate Wheel to a game class, e.g: games.cpp/.h
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
// TODO: Integrate rainbowSequence to a game class, e.g: games.cpp/.h
void rainbowSequence(uint8_t wait){
  uint16_t i, j;


    for(j=0; j<256; j++) {
      for(i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, Wheel((i+j) & 255));
      }
      pixels.show();
      delay(wait);
    }
}

void verifyOrder(String order){

  //REVIEW: take away debug prints on usb serial
  Serial.print("inVerifyOrder:");
  Serial.println(order);
  switch (order.charAt(1)) {
    case 'N':
    Serial.println("inCaseR");
    rainbowSequence(20);
    break;
    case 'L':
    break;
    case 'R':
    break;
    case 'B':
    break;
    case 'O':
    break;
    case 'H':
    break;

  }
}

char Serial_busy_wait() {
  while(!Serial1.available()) {
    ; // do nothing unstil ready
  }
  return Serial1.read();
}

void loop()
{
        // SEND DATA TO SERIAL
        if (counter == 1000) {

                sixDOF.getEuler(angles);
                sixDOF.getRawValues(rawSixDof);
                json += "[";
                json += (String)millis();
                json += ",";
                json += (String)rawSixDof[3];
                json += ",";
                json += (String)rawSixDof[4];
                json += ",";
                json += (String)rawSixDof[5];
                json += ",";
                json += (String)angles[0];
                json += ",";
                json += (String)angles[1];
                json += ",";
                json += (String)angles[2];
                json += ",";
                json += (String)rawSixDof[0];
                json += ",";
                json += (String)rawSixDof[1];
                json += ",";
                json += (String)rawSixDof[2];
                json += "]";
                Serial1.println(json);
                json = "";
                //         Serial1.println("{\"firstName\":\"John\", \"lastName\":\"Doe\"}");
                counter = 0;
        }
        else
                counter++;
        //********************** Get Communication data from bluetooth thread
        while(Serial1.available()) {

                char caracter= (char)Serial1.read();

                // ******* Be sure that what we get is the right value (especially because of noise)

                if(caracter=='<') {
                  order += (char)caracter;
                  Serial.print("_order:");
                  Serial.println(order);
                }else if(caracter == 'R'){
                  order += (char)caracter;
                  Serial.print("_order:");
                  Serial.println(order);
                }
                else if(caracter=='>'){
                  order += (char)caracter;
                  Serial.print("_order:");
                  Serial.println(order);
                  verifyOrder(order);
                }else{
                  order="";
                }


                if (caracter=='[') {
                        command = "";
                        command += (char)caracter;
                }
                else if (caracter=='-') {
                        command += (char)caracter;
                }
                else if (caracter=='+') {
                        command += (char)caracter;
                }
                else if (caracter==',') {
                        command += (char)caracter;
                }
                else if (isdigit(caracter)) {
                        command += (char)caracter;
                }
                else if (caracter==']') {
                        command += (char)caracter;
                        if (command.charAt(0)=='[') {
                                //Serial.println(command);
                                getValuesfromBluetooth(command);
                                command = "";
                                break;
                        }
                        else{
                                command = "";
                                Serial1.println("Error");
                                break;
                        }
                }
                else {
                        command = "";
                        break;
                }
        }
        //********** Pause 10ms to avoid noise
        delay(10);
}
