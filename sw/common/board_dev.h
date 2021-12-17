//++++++++++++++++++++ MASTER +++++++++++++++++++++++
// Pins
#define WAKEUP_PIN      2
#define BUTTON_PIN      3
#define MATRIX_PIN      10
#define BRIGHTNESS_PIN  A2


// Matrix
#define NUM_ANIMATIONEN 9
#define HEIGHT          10
#define WIDTH           10
#define NUM_LEDS        100
#define INITIAL_BRIGHT  100
#define MINIMAL_BRIGHT  5


// Timers
#define SLEEP_COOLDOWN  1000
#define IR_COOLDOWN     500
#define BUTTON_DEBOUNCE   70


// betterTwinkle
#define STARTING_BRIGHTNESS 64
#define FADE_IN_SPEED 32
#define FADE_OUT_SPEED 20
#define DENSITY 255
#define BITS_PER_DIRECTION_FLAG 1


// LSD
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)



//++++++++++++++++++++ SLAVE +++++++++++++++++++++++
// Pins
#define IR_PIN          12
#define SLAVE_WAKEUP    6

// Timers
#define NO_TX_OCCURED   3000
