/*
 * The Effect abstract class for the GauntletII project.
 */

#ifndef Effect_h
#define Effect_h


#include "root.h"
#include <FastLED.h> // FROM_ROOT funktioniert hier aus Gründen nicht

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
