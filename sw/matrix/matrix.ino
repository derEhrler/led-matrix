#include <FastLED.h>
#include <Wire.h>

#include "board_dev.h"
#include "matrix_init.h"
#include "matrix_control.h"
#include "matrix_animation.h"
//#include "letters.h"

/*
    #include "Twinkle.cpp"
    #include "Snake.cpp"
    #include "Life.cpp"
    #include "Plasma.cpp"
*/

//+++++++++++++++++++++ SETUP ++++++++++++++++++++++++++++
void setup() {

    delay(500);

    init_pins();

    init_comms();

    init_matrix();
}

//+++++++++++++++++++++ LOOP +++++++++++++++++++++++++++++
void loop() {

    wait_and_check(0);

    if (IRTime > buttonTime) {    //IR Singal kam zuletzt
        animation_change_ir();
    }

    if (buttonTime > IRTime) {  // Knopdruck kam zuletzt
        animation_change_button();
    }

    if (buttonTime == IRTime) {
        turn_off();
        goToSleep = true;
    }

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
