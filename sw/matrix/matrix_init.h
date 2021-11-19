#include "Arduino.h"

#include <FastLED.h>

#include "board_dev.h"


extern volatile bool buttonSignal;
extern volatile int buttonValue;
extern volatile unsigned long buttonTime;

extern long IRValueNew;
extern byte IRValue;
extern byte IRbuffer[2];
extern unsigned long IRTime;

extern bool goToSleep;
extern unsigned long wakeupTime;

extern int animationState;
extern int brightnessOffset;
extern long waitOffset;

extern CRGB matrix[NUM_LEDS];


void init_pins(void);
void init_comms(void);
void init_matrix(void);
