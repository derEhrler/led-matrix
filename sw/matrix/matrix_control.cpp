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
    delay(50);
    sleep_enable();
    sleep_mode();
    goToSleep = false;
    if (buttonSignal) {
        detachInterrupt(0);
        buttonValue = 1;
    }
    wakeupTime = millis();
    IRbuffer[0] = 0;
}


void animation_change_ir() {

    if (IRValue == 0x45) {    // I/O
        turn_off();
        goToSleep = true;
    }
    if (IRValue == 0x40) {}   // PLAY/PAUSE
    if (IRValue == 0x46) {}   // VOL+
    if (IRValue == 0x15) {}   // VOL-
    if (IRValue == 0x43) {}   // FORWARD
    if (IRValue == 0x44) {}   // BACKWARD
    if (IRValue == 0x09) {}   // UP
    if (IRValue == 0x07) {}   // DOWN
    if (IRValue == 0x19) {}   // EQ
    if (IRValue == 0x0D) {}   // ST/REPT
    if (IRValue == 0x47) {}   // FUNC/STOP
    if (IRValue == 0x16) {}   // 0
    if (IRValue == 0x0C) {    // 1
        balken_lila_blau(0x1400F5, 0xDC003C, 100);
    }
    if (IRValue == 0x18) {    // 2
        pride_strip_version();
    }
    if (IRValue == 0x5E) {    // 3
        pride_matrix_version();
    }
    if (IRValue == 0x08) {    // 4
        do_twinkle(45);
    }
    if (IRValue == 0x1C) {    // 5
        do_snake(65);
    }
    if (IRValue == 0x5A) {    // 6
        do_life(75);
    }
    if (IRValue == 0x42) {    // 7
        do_plasma();
    }
    if (IRValue == 0x52) {    // 8
        do_better_twinkle(20);
    }
    if (IRValue == 0x4A) {    // 9
        do_lsd(20);
    }
}


void animation_change_button() {

    switch (buttonValue) {
        case 0:
            turn_off();
            goToSleep = true;
            break;
        case 1:
            balken_lila_blau(0x1400F5, 0xDC003C, 100);
            break;
        case 2:
            pride_strip_version();
            break;
        case 3:
            pride_matrix_version();
            break;
        case 4:
            do_twinkle(45);
            break;
        case 5:
            do_snake(65);
            break;
        case 6:
            do_life(75);
            break;
        case 7:
            do_plasma();
            break;
        case 8:
            do_better_twinkle(20);
            break;
        case 9:
            do_lsd(20);
            break;
        default: turn_off();
    }
}


bool wait_and_check(unsigned long wait) {

    unsigned long currentTime;
    set_brightness();
    FastLED.show();
    currentTime = millis();
    do {
        if (buttonSignal) {
            buttonSignal = false;
            return true;
        }
        if (request_from_slave()) {
            if (IRValueNew == 0x0) {}           // vertippt
            else if (IRValueNew == 0x46) {      // VOL+
                brightnessOffset += 5;
            }
            else if (IRValueNew == 0x15) {      // VOL-
                brightnessOffset -= 5;
            }
            else if (IRValueNew == 0x43) {      // FORWARD
                waitOffset -= 2;
            }
            else if (IRValueNew == 0x44) {      // BACKWARD
                waitOffset += 2;
            }
            else if (IRValueNew == 0x09) {      // UP
                animationState ++;
            }
            else if (IRValueNew == 0x07) {      // DOWN
                animationState --;
            }
            else {
                IRValue = IRbuffer[0];
                IRValueNew = 0x0;
                waitOffset = 0;
                animationState = 0;
                return true;
            }
        }
    } while  (millis() < (currentTime + wait + waitOffset));
    return false;
}


bool request_from_slave() {

    if (millis() - IRTime <= 200)
        return false;

    Wire.requestFrom(5, 2); // adress, number of bytes

    if (Wire.available()) {
        for (int i = 0; i < 2; i++) {
            IRbuffer[i] = Wire.read();
            //Serial.println(IRbuffer[i]);
        }
    }

    if (IRbuffer[0] == 0x0)
        return false;

    IRValueNew = IRbuffer[0];

    IRTime = millis();
    return true;
}


void send_to_slave() {

    Wire.beginTransmission(5); 
    Wire.write(1);       
    Wire.endTransmission();
}


void set_brightness() {

    if (analogRead(BRIGHTNESS_PIN) < 10 || analogRead(BRIGHTNESS_PIN) > 1013)
        brightnessOffset = 0;
    if (map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + brightnessOffset > 255)
        FastLED.setBrightness(255);
    else if (map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + brightnessOffset < 0)
        FastLED.setBrightness(0);
    else
        FastLED.setBrightness(map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + brightnessOffset);
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
