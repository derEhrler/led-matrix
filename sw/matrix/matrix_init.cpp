#include "matrix_init.h"
#include "matrix_control.h"

#include <Wire.h>
#include <avr/sleep.h>


volatile bool buttonSignal = false;
volatile int buttonValue = 0;
volatile unsigned long buttonTime = 0;

byte IRbuffer[2] = {0, 0};
byte IRValue[2] = {0, 0};
unsigned long IRTime = 0;

bool goToSleep = false;
unsigned long wakeupTime = 0;

int animationState = 0;
int brightnessOffset = 0;
long waitOffset = 0;

CRGB matrix[NUM_LEDS];


void isr_button() {
    if (goToSleep)
        sleep_disable();
    delay(BUTTON_DEBOUNCE);
    if (digitalRead(BUTTON_PIN) == LOW) {
        buttonSignal = true;
        buttonValue++;
        if (buttonValue > NUM_ANIMATIONEN)
            buttonValue = 0;
        buttonTime = millis();
    }
}


void init_pins() {
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(1, isr_button, FALLING);

    pinMode(WAKEUP_PIN, INPUT_PULLUP);
}


void init_comms() {
    
    Serial.begin(9600);
    Wire.begin();
}


void init_matrix() {
    
    FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrix, NUM_LEDS);
    FastLED.show();
    set_brightness();
    FastLED.setDither(0);
    //FastLED.setCorrection(TypicalSMD5050);
}
