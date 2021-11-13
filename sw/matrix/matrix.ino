#include <FastLED.h>
#include <IRremote.h>
#include <avr/sleep.h>
#include <Wire.h>

/*
#include "Twinkle.cpp"
#include "Snake.cpp"
#include "Life.cpp"
#include "Plasma.cpp"
*/

//#include "letters.h"

// test, works with linux


// Pin und Matrixdefinitionen
#define WAKEUP_PIN      2
#define BUTTON_PIN      3
#define MATRIX_PIN      6
#define BRIGHTNESS_PIN  A2
#define NUM_ANIMATIONEN 9
#define HEIGHT          10
#define WIDTH           10
#define NUM_LEDS        100

#define SLEEP_COOLDOWN  1000
#define IR_COOLDOWN     700
#define BUTTON_DEBOUNCE   70

// für betterTwinkle
#define STARTING_BRIGHTNESS 64
#define FADE_IN_SPEED 32
#define FADE_OUT_SPEED 20
#define DENSITY 255

// für LSD
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

// für ISR
volatile bool buttonSignal = false;
volatile bool IRSignal = false;
volatile long IRValueNew = 0;
volatile int buttonValue = 0;
volatile unsigned long IRTime = 0;
volatile unsigned long buttonTime = 0;

byte IRValue = 0x0;
byte IRbuffer[2] = {0, 0};

// für Animationen
int animationState = 0;
int brightnessOffset = 0;
long waitOffset = 0;

// für LSD
const uint8_t kMatrixWidth  = 10;
const uint8_t kMatrixHeight = 10;
static uint16_t x = random16();
static uint16_t y = random16();
static uint16_t z = random16();
uint16_t speed = 60; // 1 = slow, 60 = like water
uint16_t scale = 30; // 1 = little noise, mostly solid colours, 100 = much noise, very zoomed out
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
CRGBPalette16 currentPalette( PartyColors_p );
uint8_t colorLoop = 1;


CRGB matrix[NUM_LEDS];


//+++++++++++++++++++++ SETUP ++++++++++++++++++++++++++++

void setup() {
    delay(500);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(1, isrButton, FALLING);

    pinMode(WAKEUP_PIN, INPUT);

    Serial.begin(9600);

    Wire.begin();

    FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrix, NUM_LEDS);
    FastLED.show();
    setBrightness();
    FastLED.setDither(0);
    //FastLED.setCorrection(TypicalSMD5050);

    IRValue = 0;
    buttonValue = 0;
}

//+++++++++++++++++++++ LOOP +++++++++++++++++++++++++++++

void loop() {
    //buttonSignal = false;
    //IRSignal = false;
    waitAndCheck(0);

    if (IRTime > buttonTime) {    //IR Singal kam zuletzt
        if (IRValue == 0x45) {    // I/O
            turn_off();
            if (millis() - IRTime > SLEEP_COOLDOWN)
                enterSleep();
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
            BalkenLilaBlau(0x1400F5, 0xDC003C, 100);
        }
        if (IRValue == 0x18) {    // 2
            pride_StripVersion();
        }
        if (IRValue == 0x5E) {    // 3
            pride_MatrixVersion();
        }
        if (IRValue == 0x08) {    // 4
            doTwinkle(45);
        }
        if (IRValue == 0x1C) {    // 5
            doSnake(65);
        }
        if (IRValue == 0x5A) {    // 6
            doLife(75);
        }
        if (IRValue == 0x42) {    // 7
            doPlasma();
        }
        if (IRValue == 0x52) {    // 8
            doBetterTwinkle(20);
        }
        if (IRValue == 0x4A) {    // 9
            doLSD(20);
        }
    }

    if (buttonTime > IRTime) {  // Knopdruck kam zuletzt
        switch (buttonValue) {
            case 0:
                turn_off();
                if (millis() - buttonTime > SLEEP_COOLDOWN)
                    enterSleep();
                break;
            case 1:
                BalkenLilaBlau(0x1400F5, 0xDC003C, 100);
                break;
            case 2:
                pride_StripVersion();
                break;
            case 3:
                pride_MatrixVersion();
                break;
            case 4:
                doTwinkle(45);
                break;
            case 5:
                doSnake(65);
                break;
            case 6:
                doLife(75);
                break;
            case 7:
                doPlasma();
                break;
            case 8:
                doBetterTwinkle(20);
                break;
            case 9:
                doLSD(20);
                break;
            default: turn_off();
        }
    }

    if (buttonTime == IRTime) {
        turn_off();
        if (millis() - IRTime > SLEEP_COOLDOWN)
                enterSleep();
    }
}


//++++++++++++++++++++ ISR ++++++++++++++++++++++++++++

void isrButton() {
    
    delay(BUTTON_DEBOUNCE);
    if (digitalRead(BUTTON_PIN) == LOW) {
        buttonSignal = true;
        buttonValue++;
        if (buttonValue > NUM_ANIMATIONEN)
            buttonValue = 0;
        buttonTime = millis();
    }
}


void isrWakeup() {

    detachInterrupt(0);
}

//++++++++++++++++++ STEUERFUNKTIONEN ++++++++++++++++++++++++++++++++++++

void enterSleep() {
    
    attachInterrupt(0, isrWakeup, HIGH);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
    IRTime = millis();
    buttonTime = millis();
    buttonValue = 0;
}


bool readIR() {

    if (millis() - IRTime <= 200)
        return false; 
    
    Wire.requestFrom(5, 2); // adress, number of bytes
    
    if(Wire.available()) {
        for (int i = 0; i<2; i++) {
            IRbuffer[i] = Wire.read();    
            //Serial.println(data[i]);
        }
    }

    if (IRbuffer[0] == 0x0)
        return false;

    IRValueNew = IRbuffer[0];

    IRTime = millis();
    return true;
}


void writeIR() {

    Wire.beginTransmission(5);
    Wire.write(1);
    Wire.endTransmission();
}


bool waitAndCheck(unsigned long wait) {
    unsigned long currentTime;
    setBrightness();
    FastLED.show();
    currentTime = millis();
    do {
        if (buttonSignal) {
            buttonSignal = false;
            return true;
        }
        if (readIR()) {
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

void setBrightness() {
    
    if (analogRead(BRIGHTNESS_PIN) < 10 || analogRead(BRIGHTNESS_PIN) > 1013)
        brightnessOffset = 0;
    if (map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + brightnessOffset > 255)
        FastLED.setBrightness(255);
    else if (map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + brightnessOffset < 0)
        FastLED.setBrightness(0);
    else
        FastLED.setBrightness(map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0) + brightnessOffset);
}


int whichState(int numStates) {
    
    return abs(animationState % numStates);
}


long RGBtoHEX (byte r, byte g, byte b) {
    
    return ((long)r << 16L) | ((long)g << 8L) | (long)b;
}


int XY (int x, int y) {
    
    return x + y * WIDTH;
}


void drawHorizontalLine(int x, int y, int distance, long colour) {
    
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


void drawVertikalLine(int x, int y, int distance, long colour) {
    
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


//++++++++++++++++++ ANIMATIONEN +++++++++++++++++++++++++++++++++++++++++

//------------------------------- turn_off
void turn_off() {
    FastLED.clear();
    FastLED.show();
}

//------------------------------- rainbowPicture
/*
    void rainbowPicture() {
    for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
        for (int i = 0; i < NUM_LEDS; i++) {
            int pixelHue = firstPixelHue + (i * 65536L / NUM_LEDS);
            //matrix[i].setHue(pixelHue);
            matrix[i].setHSV(pixelHue, 255, 255);
        }
        FastLED.setBrightness(map(analogRead(BRIGHTNESS_PIN), 0, 1023, 255, 0));
        FastLED.show();
        if (buttonSignal || IRSignal)
            return;
    }
    }
*/
//------------------------------- BalkenLilaBlau
void BalkenLilaBlau(unsigned long lila, unsigned long blau, unsigned long wait) {
    for (int i = 0; i < HEIGHT / 2; i++) {          // oben -> unten
        drawHorizontalLine(0, i, 10, blau);
        drawHorizontalLine(0, i + 5, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // unten weg -> von links
        drawHorizontalLine(i, 5 + i, 10 - i, blau);
        drawVertikalLine(i, 0, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // links -> rechts
        drawVertikalLine(i, 0, 10, blau);
        drawVertikalLine(5 + i, 0, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // rechts weg -> von unten
        drawVertikalLine(5 + i, 0, 10 - i, blau);
        drawHorizontalLine(0, 9 - i, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // unten -> oben
        drawHorizontalLine(0, 9 - i, 10, blau);
        drawHorizontalLine(0, 4 - i, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // oben weg -> von rechts
        drawHorizontalLine(0, 4 - i, 10 - i, blau);
        drawVertikalLine(9 - i, 0, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // rechts -> links
        drawVertikalLine(9 - i, 0, 10, blau);
        drawVertikalLine(4 - i, 0, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // links weg -> von oben
        drawVertikalLine(4 - i, i, 10 - i, blau);
        drawHorizontalLine(0, i, 10, lila);
        if (waitAndCheck(wait))
            return;
    }
}

//------------------------------- pride_StripVersion
void pride_StripVersion() {
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;

    uint8_t sat8 = beatsin88( 87, 220, 250);
    uint8_t brightdepth = beatsin88( 341, 96, 224);
    uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
    uint8_t msmultiplier = beatsin88(147, 23, 60);

    uint16_t hue16 = sHue16;//gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);

    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis ;
    sLastMillis  = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88( 400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
        hue16 += hueinc16;
        uint8_t hue8 = hue16 / 256;

        brightnesstheta16  += brightnessthetainc16;
        uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

        uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
        uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);

        CRGB newcolor = CHSV( hue8, sat8, bri8);

        uint16_t pixelnumber = i;
        pixelnumber = (NUM_LEDS - 1) - pixelnumber;

        nblend( matrix[pixelnumber], newcolor, 64);
    }
    setBrightness();
    FastLED.show();
}

//------------------------------- pride_MatrixVersion
void pride_MatrixVersion() {
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;

    uint8_t sat8 = beatsin88( 87, 220, 250);
    uint8_t brightdepth = beatsin88( 341, 96, 224);
    uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
    uint8_t msmultiplier = beatsin88(147, 23, 60);

    uint16_t hue16 = sHue16;//gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);

    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis ;
    sLastMillis  = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88( 400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for ( uint16_t i = 0 ; i < HEIGHT; i++) {
        hue16 += hueinc16;
        uint8_t hue8 = hue16 / 256;

        brightnesstheta16  += brightnessthetainc16;
        uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

        uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
        uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);

        CRGB newcolor = CHSV( hue8, sat8, bri8);

        uint16_t pixelnumber = i;
        pixelnumber = (HEIGHT - 1) - pixelnumber;

        for (int i = 0; i < WIDTH; i++) {
            nblend(matrix[XY(i, pixelnumber)], newcolor, 64);
        }
    }
    setBrightness();
    FastLED.show();
}

//------------------------------- doTwinkle
void doTwinkle(unsigned long wait) {
    //Twinkle twinkle(matrix, WIDTH, HEIGHT, true, true);
    //twinkle.start(&buttonSignal, &IRSignal, &IRValue, &IRValueNew, &waitOffset, &brightnessOffset, &animationState, wait);
}

//------------------------------- doSnake
void doSnake(unsigned long wait) {
    //Snake snake(matrix, WIDTH, HEIGHT);
    //snake.start(&buttonSignal, &IRSignal, &IRValue, &IRValueNew, &waitOffset, &brightnessOffset, &animationState, wait);
}

//------------------------------- doLife
void doLife(unsigned long wait) {
    //Life life(matrix, WIDTH, HEIGHT, 60);
    //life.start(&buttonSignal, &IRSignal, &IRValue, &IRValueNew, &waitOffset, &brightnessOffset, &animationState, wait);
}

//------------------------------- doPlasma
void doPlasma() {
    //Plasma plasma(matrix, WIDTH, HEIGHT);
    //plasma.start(&buttonSignal, &IRSignal, &IRValue, &IRValueNew, &waitOffset, &brightnessOffset, &animationState);
}

//------------------------------- doBetterTwinkle
void doBetterTwinkle(unsigned long wait) {
    chooseColorPalette();
    colortwinkles();
    if (waitAndCheck(wait))
        return;
}

//------------------------------- doLSD
void doLSD(unsigned long wait) {
    ChangePaletteAndSettingsPeriodically();
    fillnoise8();
    mapNoiseToLEDsUsingPalette();
    if (waitAndCheck(wait))
    //if (waitAndCheck(waitOffset * (-1)))
        return;
}

//+++++++++++++++++++++++++++++ für betterTwinkle ++++++++++++++++++++++++++++++++++++++++

CRGBPalette16 gPalette;

void chooseColorPalette() {
    CRGB r(CRGB::Red), b(CRGB::Blue), w(85, 85, 85), g(CRGB::Green), W(CRGB::White), l(0xE1A024);

    switch (whichState(7)) {
        case 0: // Red, Green, and White
            gPalette = CRGBPalette16( r, r, r, r, r, r, r, r, g, g, g, g, w, w, w, w );
            break;
        case 1: // Blue and White
            //gPalette = CRGBPalette16( b,b,b,b, b,b,b,b, w,w,w,w, w,w,w,w );
            gPalette = CloudColors_p; // Blues and whites!
            break;
        case 2: // Rainbow of colors
            gPalette = RainbowColors_p;
            break;
        case 3:
            gPalette = PartyColors_p;
            break;
        case 4: // Incandescent "fairy lights"
            gPalette = CRGBPalette16( l, l, l, l, l, l, l, l, l, l, l, l, l, l, l, l );
            break;
        case 5:
            gPalette = ForestColors_p;
            break;
        case 6: // Snow
            gPalette = CRGBPalette16( W, W, W, W, w, w, w, w, w, w, w, w, w, w, w, w );
            break;
    }
}

enum { GETTING_DARKER = 0, GETTING_BRIGHTER = 1 };

void colortwinkles() {
    brightenOrDarkenEachPixel( FADE_IN_SPEED, FADE_OUT_SPEED);
    if ( random8() < DENSITY ) {
        int pos = random16(NUM_LEDS);
        if ( !matrix[pos]) {
            matrix[pos] = ColorFromPalette( gPalette, random8(), STARTING_BRIGHTNESS, NOBLEND);
            setPixelDirection(pos, GETTING_BRIGHTER);
        }
    }
}

void brightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount) {
    for ( uint16_t i = 0; i < NUM_LEDS; i++) {
        if ( getPixelDirection(i) == GETTING_DARKER) {
            matrix[i] = makeDarker( matrix[i], fadeDownAmount);
        } else {
            matrix[i] = makeBrighter( matrix[i], fadeUpAmount);
            if ( matrix[i].r == 255 || matrix[i].g == 255 || matrix[i].b == 255) {
                setPixelDirection(i, GETTING_DARKER);
            }
        }
    }
}

CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter) {
    CRGB incrementalColor = color;
    incrementalColor.nscale8( howMuchBrighter);
    return color + incrementalColor;
}

CRGB makeDarker( const CRGB& color, fract8 howMuchDarker) {
    CRGB newcolor = color;
    newcolor.nscale8( 255 - howMuchDarker);
    return newcolor;
}

#define BITS_PER_DIRECTION_FLAG 1

#if BITS_PER_DIRECTION_FLAG == 8

uint8_t directionFlags[NUM_LEDS];

bool getPixelDirection( uint16_t i) {
    return directionFlags[i];
}

void setPixelDirection( uint16_t i, bool dir) {
    directionFlags[i] = dir;
}
#endif


#if BITS_PER_DIRECTION_FLAG == 1

uint8_t directionFlags[ (NUM_LEDS + 7) / 8];

bool getPixelDirection( uint16_t i) {
    uint16_t index = i / 8;
    uint8_t bitNum = i & 0x07;
    return bitRead( directionFlags[index], bitNum);
}

void setPixelDirection( uint16_t i, bool dir) {
    uint16_t index = i / 8;
    uint8_t bitNum = i & 0x07;
    bitWrite( directionFlags[index], bitNum, dir);
}
#endif

//+++++++++++++++++++++++++++++ für LSD ++++++++++++++++++++++++++++++++++++++++

void fillnoise8() {  
    uint8_t dataSmoothing = 0;
    if ( speed < 50) {
        dataSmoothing = 200 - (speed * 4);
    }
    for (int i = 0; i < MAX_DIMENSION; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++) {
            int joffset = scale * j;
            uint8_t data = inoise8(x + ioffset, y + joffset, z);
            data = qsub8(data, 16);
            data = qadd8(data, scale8(data, 39));
            if ( dataSmoothing ) {
                uint8_t olddata = noise[i][j];
                uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
                data = newdata;
            }
            noise[i][j] = data;
        }
    }
    z += speed;
    x += speed / 8;
    y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette() {
    static uint8_t ihue = 0;
    for (int i = 0; i < kMatrixWidth; i++) {
        for (int j = 0; j < kMatrixHeight; j++) {
            uint8_t index = noise[j][i];
            uint8_t bri =   noise[i][j];
            if ( colorLoop) {
                index += ihue;
            }
            if ( bri > 127 ) {
                bri = 255;
            } else {
                bri = dim8_raw( bri * 2);
            }
            CRGB color = ColorFromPalette( currentPalette, index, bri);
            matrix[XY(i, j)] = color;
        }
    }
    ihue += 1;
}

void ChangePaletteAndSettingsPeriodically() {
    static int prevState = 99;

    if (prevState != animationState) {
        prevState = animationState;
        waitOffset = 0;
        switch (whichState(12)) {
            case 0:
                currentPalette = RainbowColors_p;
                speed = 20;
                scale = 30;
                colorLoop = 1;
                break;
            case 1:
                SetupPurpleAndGreenPalette();
                speed = 10;
                scale = 50;
                colorLoop = 1;
                break;
            case 2:
                SetupBlackAndWhiteStripedPalette();
                speed = 15;
                scale = 30;
                colorLoop = 1;
                break;
            case 3:
                currentPalette = ForestColors_p;
                speed =  10;
                scale = 50;
                colorLoop = 0;
                break;
            case 4:
                currentPalette = CloudColors_p;
                speed =  15;
                scale = 30;
                colorLoop = 0;
                break;
            case 5:
                currentPalette = LavaColors_p;
                speed =  10;
                scale = 50;
                colorLoop = 0;
                break;
            case 6:
                currentPalette = OceanColors_p;
                speed = 20;
                scale = 60;
                colorLoop = 0;
                break;
            case 7:
                currentPalette = PartyColors_p;
                speed = 25;
                scale = 30;
                colorLoop = 1;
                break;
            case 8:
                SetupRandomPalette();
                speed = 20;
                scale = 20;
                colorLoop = 1;
                break;
            case 9:
                SetupRandomPalette();
                speed = 20;
                scale = 40;
                colorLoop = 1;
                break;
            case 10:
                SetupRandomPalette();
                speed = 20;
                scale = 60;
                colorLoop = 1;
                break;
            case 11:
                currentPalette = RainbowStripeColors_p;
                speed = 25;
                scale = 25;
                colorLoop = 1;
                break;
        }
    }
    if (waitOffset != 0) {
        if (waitOffset > 0) {speed -= 2;}
        else {speed += 2;}
        waitOffset = 0;
    }
}

void SetupRandomPalette() {
    currentPalette = CRGBPalette16(
                         CHSV( random8(), 255, 32),
                         CHSV( random8(), 255, 255),
                         CHSV( random8(), 128, 255),
                         CHSV( random8(), 255, 255));
}

void SetupBlackAndWhiteStripedPalette() {
    fill_solid( currentPalette, 16, CRGB::Black);
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette() {
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    currentPalette = CRGBPalette16(
                         green,  green,  black,  black,
                         purple, purple, black,  black,
                         green,  green,  black,  black,
                         purple, purple, black,  black );
}

//-------------------------------

//    matrix[i] = CRGB(50, 100, 150);
//    matrix[i] = 0xFF007F;
//    matrix[i].setRGB( 50, 100, 150);
