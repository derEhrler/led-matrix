#include "matrix_init.h"
#include "matrix_control.h"
#include "matrix_animation.h"

#include "Twinkle.cpp"
#include "Snake.cpp"
#include "Life.cpp"
#include "Plasma.cpp"

#include <FastLED.h>

//--------------------------------------------------------------------- turn_off
void turn_off() {

    FastLED.clear();
    FastLED.show();
}


//--------------------------------------------------------------------- BalkenLilaBlau
void balken_lila_blau(unsigned long lila, unsigned long blau, unsigned long wait) {

    for (int i = 0; i < HEIGHT / 2; i++) {          // oben -> unten
        draw_horizontal_line(0, i, 10, blau);
        draw_horizontal_line(0, i + 5, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // unten weg -> von links
        draw_horizontal_line(i, 5 + i, 10 - i, blau);
        draw_vertical_line(i, 0, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // links -> rechts
        draw_vertical_line(i, 0, 10, blau);
        draw_vertical_line(5 + i, 0, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // rechts weg -> von unten
        draw_vertical_line(5 + i, 0, 10 - i, blau);
        draw_horizontal_line(0, 9 - i, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // unten -> oben
        draw_horizontal_line(0, 9 - i, 10, blau);
        draw_horizontal_line(0, 4 - i, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // oben weg -> von rechts
        draw_horizontal_line(0, 4 - i, 10 - i, blau);
        draw_vertical_line(9 - i, 0, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // rechts -> links
        draw_vertical_line(9 - i, 0, 10, blau);
        draw_vertical_line(4 - i, 0, 10, lila);
        if (wait_and_check(wait))
            return;
    }
    for (int i = 0; i < HEIGHT / 2; i++) {          // links weg -> von oben
        draw_vertical_line(4 - i, i, 10 - i, blau);
        draw_horizontal_line(0, i, 10, lila);
        if (wait_and_check(wait))
            return;
    }
}


//--------------------------------------------------------------------- pride_StripVersion
void pride_strip_version() {

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
    set_brightness();
    FastLED.show();
}


//--------------------------------------------------------------------- pride_MatrixVersion
void pride_matrix_version() {

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
            nblend(matrix[x_y(i, pixelnumber)], newcolor, 64);
        }
    }
    set_brightness();
    FastLED.show();
}


//--------------------------------------------------------------------- doTwinkle
void do_twinkle(unsigned long wait) {

    Twinkle twinkle(matrix, WIDTH, HEIGHT, true, true);
    twinkle.start(wait);
}


//--------------------------------------------------------------------- doSnake
void do_snake(unsigned long wait) {

    Snake snake(matrix, WIDTH, HEIGHT);
    snake.start(wait);
}


//--------------------------------------------------------------------- doLife
void do_life(unsigned long wait) {

    Life life(matrix, WIDTH, HEIGHT, 60);
    life.start(wait);
}


//--------------------------------------------------------------------- doPlasma
void do_plasma() {

    Plasma plasma(matrix, WIDTH, HEIGHT);
    plasma.start();
}


//--------------------------------------------------------------------- doBetterTwinkle
CRGBPalette16 gPalette;

void chooseColorPalette() {
    CRGB r(CRGB::Red), b(CRGB::Blue), w(85, 85, 85), g(CRGB::Green), W(CRGB::White), l(0xE1A024);

    switch (which_state(7)) {
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


void do_better_twinkle(unsigned long wait) {

    chooseColorPalette();
    colortwinkles();
    if (wait_and_check(wait))
        return;
}


//--------------------------------------------------------------------- doLSD
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
            matrix[x_y(i, j)] = color;
        }
    }
    ihue += 1;
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

void ChangePaletteAndSettingsPeriodically() {
    static int prevState = 99;

    if (prevState != animationState) {
        prevState = animationState;
        waitOffset = 0;
        switch (which_state(12)) {
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
        if (waitOffset > 0) {
            speed -= 2;
        }
        else {
            speed += 2;
        }
        waitOffset = 0;
    }
}


void do_lsd(unsigned long wait) {

    ChangePaletteAndSettingsPeriodically();
    fillnoise8();
    mapNoiseToLEDsUsingPalette();
    if (wait_and_check(wait))
        return;
}
