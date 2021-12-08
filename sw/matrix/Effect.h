/*
 * The Effect abstract class for the GauntletII project.
 */

#ifndef Effect_h
#define Effect_h

#define IR_PIN          2
#define BUTTON_PIN      3
#define MATRIX_PIN      6
#define BRIGHTNESS_PIN  A5
#define NUM_ANIMATIONEN 9
#define HEIGHT          10
#define WIDTH           10
#define NUM_LEDS        100

#define SLEEP_COOLDOWN  1000
#define IR_COOLDOWN     700
#define BUTTON_BOUNCE   50

#import <FastLED.h>

class Effect {

    protected:
        CRGB *leds;
        int width, height;
        
        struct CRGB& pixel(int x, int y);
        bool inXRange(int x);
        bool inYRange(int y);
    
        void clearLeds();

    public:
        Effect(CRGB *leds, int width, int height);

        virtual void start() {};

};

#endif
