#include "matrix_init.h"
#include "matrix_control.h"
#include "matrix_animation.h"

#include <avr/sleep.h>
#include <Wire.h>



void isrWakeup() {

    sleep_disable();
    detachInterrupt(0);
}


void enter_sleep() {

    attachInterrupt(0, isrWakeup, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    //Serial.println("Sleep Mode");
    delay(50);
    sleep_enable();
    sleep_mode();
    goToSleep = false;
    wakeupTime = millis();
    IRbuffer[0] = 0;
    IRValue[0] = IRValue[1];
}


void animation_change_ir() {

    if (IRValue[0] == 0x1C) {    // I/O
        turn_off();
        goToSleep = true;
    }
    if (IRValue[0] == 0x40) {}   // PLAY/PAUSE
    if (IRValue[0] == 0x46) {}   // VOL+
    if (IRValue[0] == 0x15) {}   // VOL-
    if (IRValue[0] == 0x43) {}   // FORWARD
    if (IRValue[0] == 0x44) {}   // BACKWARD
    if (IRValue[0] == 0x09) {}   // UP
    if (IRValue[0] == 0x07) {}   // DOWN
    if (IRValue[0] == 0x19) {}   // EQ
    if (IRValue[0] == 0x0D) {}   // ST/REPT
    if (IRValue[0] == 0x47) {}   // FUNC/STOP
    if (IRValue[0] == 0x16) {}   // 0
    if (IRValue[0] == 0x45) {    // 1
        balken_lila_blau(0x1400F5, 0xDC003C, 100);
    }
    if (IRValue[0] == 0x46) {    // 2
        pride_strip_version();
    }
    if (IRValue[0] == 0x47) {    // 3
        pride_matrix_version();
    }
    if (IRValue[0] == 0x44) {    // 4
        do_twinkle(45);
    }
    if (IRValue[0] == 0x40) {    // 5
        do_snake(65);
    }
    if (IRValue[0] == 0x43) {    // 6
        do_life(75);
    }
    if (IRValue[0] == 0x7) {    // 7
        do_plasma();
    }
    if (IRValue[0] == 0x15) {    // 8
        do_better_twinkle(20);
    }
    if (IRValue[0] == 0x9) {    // 9
        do_lsd(20);
    }
}


bool wait_and_check(unsigned long wait) {

    unsigned long currentTime;
    set_brightness();
    FastLED.show();
    currentTime = millis();

    unsigned long helpTime;
    long sleepTimeout;

    //Serial.println(IRbuffer[0]);
    //Serial.println(IRValue[0]);
    //Serial.println(" ");

    do {
        helpTime = millis() - wakeupTime;
        sleepTimeout = long(helpTime);
        if (request_from_slave()) {
            if (IRbuffer[0] == 0x0) {}           // vertippt
            else if (IRbuffer[0] == 0x18) {      // VOL+
                brightnessOffset += 5 * IRbuffer[1];
            }
            else if (IRbuffer[0] == 0x52) {      // VOL-
                brightnessOffset -= 5 * IRbuffer[1];
            }
            else if (IRbuffer[0] == 0x5A) {      // FORWARD
                waitOffset -= 2 * IRbuffer[1];
            }
            else if (IRbuffer[0] == 0x8) {      // BACKWARD
                waitOffset += 2 * IRbuffer[1];
            }
            else if (IRbuffer[0] == 0xD) {      // UP
                animationState ++;
                IRbuffer[0] = 0x0;
            }
            else if (IRbuffer[0] == 0x16) {      // DOWN
                animationState --;
                IRbuffer[0] = 0x0;
            }
            else {
                if (IRbuffer[0] == 0x1C && sleepTimeout <= SLEEP_COOLDOWN) {
                    return false;    
                }
                IRValue[1] = IRValue[0];
                IRValue[0] = IRbuffer[0];
                IRbuffer[0] = 0x0;
                waitOffset = 0;
                animationState = 0;
                return true;
            }
        }
    } while  (millis() < (currentTime + wait + waitOffset));
    return false;
}


bool request_from_slave() {

    if (millis() < (IRTime + IR_COOLDOWN))
        return false;

    Wire.requestFrom(5, 2); // adress, number of bytes

    if (Wire.available()) {
        for (int i = 0; i < 2; i++) {
            IRbuffer[i] = Wire.read();
            //Serial.println(IRbuffer[i]);
        }
    }

    IRTime = millis();

    return true;
}


void send_to_slave() {

    Wire.beginTransmission(5);
    Wire.write(1);
    Wire.endTransmission();
}


void set_brightness() {

    if (brightnessOffset > (255 - INITIAL_BRIGHT)) 
        brightnessOffset = 255 - INITIAL_BRIGHT;
    else if (brightnessOffset < (INITIAL_BRIGHT - MINIMAL_BRIGHT) * (-1))
        brightnessOffset = (INITIAL_BRIGHT - MINIMAL_BRIGHT) * (-1);

    FastLED.setBrightness(INITIAL_BRIGHT + brightnessOffset);
}


int which_state(int numStates) {

    return abs(animationState % numStates);
}


long RGB_to_HEX (byte r, byte g, byte b) {

    return ((long)r << 16L) | ((long)g << 8L) | (long)b;
}


int x_y (int x, int y) {

    return x + y * WIDTH;
}


void draw_horizontal_line(int x, int y, int distance, long colour) {

    if (distance > WIDTH)
        distance = WIDTH;
    for (int i = x; i < x + distance; i++) {
        if (i > WIDTH - 1) {
            distance = distance + x - WIDTH;
            i = 0;
            x = 0;
        }
        matrix[i + WIDTH * y] = colour;
    }
}


void draw_vertical_line(int x, int y, int distance, long colour) {

    if (distance > HEIGHT)
        distance = HEIGHT;
    for (int i = y; i < y + distance; i++) {
        if (i > HEIGHT - 1) {
            distance = distance + y - HEIGHT;
            i = 0;
            y = 0;
        }
        matrix[WIDTH * i + x] = colour;
    }
}
