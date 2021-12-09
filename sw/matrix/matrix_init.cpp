#include "matrix_init.h"
#include "matrix_control.h"

#include <Wire.h>
#include <avr/sleep.h>



byte IRbuffer[2] = {0, 0};
byte IRValue[2] = {0, 0};
unsigned long IRTime = 0;

bool goToSleep = false;
unsigned long wakeupTime = 0;

byte animationState = 0;
int brightnessOffset = 0;
long waitOffset = 0;

CRGB matrix[NUM_LEDS];


void init_pins() {

    pinMode(WAKEUP_PIN, INPUT_PULLUP);
}


void init_comms() {
    
    //Serial.begin(9600);
    Wire.begin();
}


void init_matrix() {
    
    FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrix, NUM_LEDS);
    FastLED.show();
    set_brightness();
    FastLED.setDither(0);
    //FastLED.setCorrection(TypicalSMD5050);
}
