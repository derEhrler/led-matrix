/*
    The Effect implementation for the TechnocolourDreamcoat project.
*/

#include "Effect.h"

Effect::Effect(CRGB *leds, int width, int height): leds(leds), width(width), height(height) {}

bool Effect::inXRange(int x) {
    return x >= 0 && x < width;
}

bool Effect::inYRange(int y) {
    return y >= 0 && y < height;
}

struct CRGB& Effect::pixel(int x, int y) {
    if (y & 1) {
        return leds[(y * width) + width - x - 1];
    } else {
        return leds[(y * width) + x];
    }
}

void Effect::clearLeds() {
    memset8(leds, 0, width * height * 3);
}


bool Effect::_waitAndCheck(bool *_buttonSignal, bool *_IRSignal, long *_IRValue, volatile long *_IRValueNew, long *_waitOffset, int *_brightnessOffset, int *_animationState, unsigned long wait) {
    unsigned long currentTime;
    _setBrightness(_brightnessOffset);
    FastLED.show();
    currentTime = millis();
    do {
        if (*_buttonSignal) {
            *_buttonSignal = false;
            return true;
        }
        if (*_IRSignal) {
            while (millis() - currentTime <= IR_COOLDOWN) {
                if (*_IRValueNew != 0x0)
                    break;
            }
            *_IRSignal = false;
            if (*_IRValueNew == 0x0) {}           // vertippt
            else if (*_IRValueNew == 0x46) {      // VOL+
                *_brightnessOffset += 10;
            }
            else if (*_IRValueNew == 0x15) {      // VOL-
                *_brightnessOffset -= 10;
            }
            else if (*_IRValueNew == 0x43) {      // FORWARD
                *_waitOffset -= 5;
            }
            else if (*_IRValueNew == 0x44) {      // BACKWARD
                *_waitOffset += 5;
            }
            else if (*_IRValueNew == 0x09) {      // UP
                *_animationState ++;
            }
            else if (*_IRValueNew == 0x07) {      // DOWN
                *_animationState --;
            }
            else {
                *_IRValue = *_IRValueNew;
                *_IRValueNew = 0x0;
                *_waitOffset = 0;   
                *_animationState = 0;
                return true;
            }
        }
    } while  (millis() < (currentTime + wait + *_waitOffset));
    return false;
}

void Effect::_setBrightness(int *_brightnessOffset) {
    if (analogRead(BRIGHTNESS_PIN) < 10 || analogRead(BRIGHTNESS_PIN) > 1013) 
        *_brightnessOffset = 0;
    if (map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + *_brightnessOffset > 255)
        FastLED.setBrightness(255);
    else if (map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + *_brightnessOffset < 0)
        FastLED.setBrightness(0);
    else 
        FastLED.setBrightness(map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + *_brightnessOffset);
}
