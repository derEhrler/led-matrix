#include <FastLED.h>
#include <Wire.h>

#include "matrix_init.h"
#include "matrix_control.h"
#include "matrix_animation.h"
//#include "letters.h"


//+++++++++++++++++++++ SETUP ++++++++++++++++++++++++++++
void setup() {

    delay(500);

    init_pins();

    init_comms();

    init_matrix();

    turn_off();
}

//+++++++++++++++++++++ LOOP +++++++++++++++++++++++++++++
void loop() {

    wait_and_check(0);

    animation_change_ir();

    if (goToSleep) {
        turn_off();
        send_to_slave();
        enter_sleep();
    }
}


//-------------------------------

//    matrix[i] = CRGB(50, 100, 150);
//    matrix[i] = 0xFF007F;
//    matrix[i].setRGB( 50, 100, 150);
