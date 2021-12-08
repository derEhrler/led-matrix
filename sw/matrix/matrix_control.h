#include "Arduino.h"

void enter_sleep(void);

void animation_change_ir(void);
void animation_change_button(void);
bool wait_and_check(unsigned long wait);
bool request_from_slave(void);
void send_to_slave(void);
void set_brightness(void);
int which_state(int numStates);
long RGB_to_HEX (byte r, byte g, byte b);
int x_y (int x, int y);
void draw_horizontal_line(int x, int y, int distance, long colour);
void draw_vertical_line(int x, int y, int distance, long colour);
