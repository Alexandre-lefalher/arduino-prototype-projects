
#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_NeoPixel.h>

#include "lightshow.h"



//Setup for light show
Adafruit_NeoPixel pixel_belt = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_BELT);
Adafruit_NeoPixel pixel_ears = Adafruit_NeoPixel(2,PIN_LED_EARS);

uint8_t red = 0;
uint8_t blue = 0;
uint8_t green = 0;

int gameId = 0;

uint8_t trailSize = 0;

uint32_t color = 0;

uint16_t showSpeed = 10;

void setup(){

  Serio_USB.begin(115200);

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

  randomSeed(analogRead(9));

}


void loop (){


  Serio_USB.print("In Main:\n");
  //Randomisers

  delay(200);

  red = random(0, 255);
  blue = random(0, 255);
  green = random(0, 255);

  trailSize = random(1,5);

  color = pixel_belt.Color(red,blue,green);

  showSpeed = random(20, 100);

  gameId = random(0, 6);
  Serio_USB.print("Game ID: ");
  Serio_USB.print(gameId);
  Serio_USB.print("\n");

  //LaunchGames
  switch (gameId) {
    case 0:
    theaterChase(color,showSpeed);
    theaterChase(color,showSpeed);
    clearColorBelt();
    break;

    case 1:
    colorWipeLeft(color,showSpeed,trailSize);
    colorWipeRight(color,showSpeed,trailSize);
    clearColorBelt();
    break;

    case 2:
    colorWipeRight(color,showSpeed, trailSize);
    colorWipeLeft(color,showSpeed, trailSize);
    clearColorBelt();
    break;

    case 3:
    colorWipe(color,showSpeed);
    clearColorBelt();
    colorWipe(color,showSpeed);
    clearColorBelt();
    break;

    case 4:
    //TheRobot with the whole color
    rainbowSequence(10);
    break;

    case 5:
    //the Robot with raibow in bits
    rainbowCycle(5);
    break;

    case 6:
    colorWipeLeft(color,showSpeed,1);
    colorWipeRight(color,showSpeed,2);
    colorWipeLeft(color,showSpeed,3);
    colorWipeRight(color,showSpeed,4);
    colorWipeLeft(color,showSpeed,5);
    break;

  }

  //Cycle again;
  delay(50);

}

//-------------------------------------------------------------
//--- Tools ---
//-------------------------------------------------------------
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixel_belt.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixel_belt.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixel_belt.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//-------------------------------------------------------------
//--- Light patterns possible ---
//-------------------------------------------------------------
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<2; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixel_belt.numPixels(); i=i+3) {
        pixel_belt.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixel_belt.show();

      delay(wait);

      for (uint16_t i=0; i < pixel_belt.numPixels(); i=i+3) {
        pixel_belt.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}



void rainbowSequence(uint8_t wait){
  uint16_t i, j;


    for(j=0; j<256; j++) {
      for(i=0; i<pixel_belt.numPixels(); i++) {
        pixel_belt.setPixelColor(i, Wheel((i+j) & 255));
      }
      pixel_belt.show();
      delay(wait);
    }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixel_belt.numPixels(); i++) {
      pixel_belt.setPixelColor(i, Wheel(((i * 256 / pixel_belt.numPixels()) + j) & 255));
    }
    pixel_belt.show();
    delay(wait);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixel_belt.numPixels(); i++) {
    pixel_belt.setPixelColor(i, c);
    pixel_belt.show();
    delay(wait);
  }
}

void colorWipeLeft(uint32_t c, uint8_t wait, uint8_t trailSize) {
  uint32_t off = pixel_belt.Color(0,0,0);


  for(uint16_t i=0; i<pixel_belt.numPixels(); i++) {
    pixel_belt.setPixelColor(i, c);
    if((i-trailSize)>=0) pixel_belt.setPixelColor(i-trailSize,off);
    pixel_belt.show();
    delay(wait);
  }
}

void colorWipeRight(uint32_t c, uint8_t wait, uint8_t trailSize) {
  uint32_t off = pixel_belt.Color(0,0,0);
  uint16_t max_pixels = pixel_belt.numPixels();


  for(uint16_t i=0; i<=max_pixels; i++) {
    pixel_belt.setPixelColor(max_pixels-i, c);
    if(i>=trailSize)pixel_belt.setPixelColor(max_pixels-i+trailSize,off);
    pixel_belt.show();
    delay(wait);
  }
}

void clearColorBelt(){
  uint32_t off = pixel_belt.Color(0,0,0);
  for(int i = 0; i < pixel_belt.numPixels();i++){
    pixel_belt.setPixelColor(i,off);
  }
  pixel_belt.show();
}
