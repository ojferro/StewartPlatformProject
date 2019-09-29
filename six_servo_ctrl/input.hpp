#define JOYSTICK_PIN_X    0
#define JOYSTICK_PIN_Y    1

#define X_NEUTRAL         506
#define Y_NEUTRAL         511
#define BIN_BOUNDS        2

#define JOYSTICK_LOW_BOUND    -15
#define JOYSTICK_HIGH_BOUND    15

long map_smart(long x, long in_min, long in_max, long out_min, long out_max) {
  return round(((x - in_min) * (out_max - out_min)) / float(in_max - in_min)) + out_min;
}
 
void init_input() {
  // Nothing needed for right now
}
  
void read_joystick(int & x_pos, int & y_pos) {
    x_pos = map_smart(analogRead(JOYSTICK_PIN_X), 0, 1023, JOYSTICK_LOW_BOUND, JOYSTICK_HIGH_BOUND);
    y_pos = map_smart(analogRead(JOYSTICK_PIN_Y), 0, 1023, JOYSTICK_LOW_BOUND, JOYSTICK_HIGH_BOUND);
}
