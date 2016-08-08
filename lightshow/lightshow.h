#ifndef LIGHTSHOW_H
#define LIGHTSHOW_H


uint32_t Wheel(byte WheelPos);

void rainbowCycle(uint8_t wait);
void rainbowSequence(uint8_t wait);

void theaterChase(uint32_t c, uint8_t wait);

void colorWipe(uint32_t c, uint8_t wait);

void colorWipeLeft(uint32_t c, uint8_t wait, uint8_t trailSize);

void colorWipeRight(uint32_t c, uint8_t wait, uint8_t trailSize);

void clearColorBelt(void);

 #define PIN_LED_BELT   46
 #define PIN_LED_EARS   8
 #define NUMPIXELS      20

 #define Serio_USB Serial

#endif LIGHTSHOW_H
