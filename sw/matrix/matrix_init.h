#include "Arduino.h"

#include <FastLED.h>

#include "C:\Users\Georg\Documents\led-matrix\sw\common\board_dev.h"


extern volatile bool buttonSignal;
extern volatile int buttonValue;
extern volatile unsigned long buttonTime;

extern byte IRbuffer[2];
extern byte IRValue[2];
extern unsigned long IRTime;

extern bool goToSleep;
extern unsigned long wakeupTime;

extern byte animationState;
extern int brightnessOffset;
extern long waitOffset;

extern CRGB matrix[NUM_LEDS];


void init_pins(void);
void init_comms(void);
void init_matrix(void);
