#define JOYSTICK_PIN_X    0
#define JOYSTICK_PIN_Y    1

#define X_NEUTRAL         506
#define Y_NEUTRAL         511
#define BIN_BOUNDS        2

#define JOYSTICK_DELTA_LOW_BOUND     -80
#define JOYSTICK_DELTA_HIGH_BOUND     80

long map_smart(long x, long in_min, long in_max, long out_min, long out_max) {
  return round(((x - in_min) * (out_max - out_min)) / float(in_max - in_min)) + out_min;
}
 
void init_input() {
  // Nothing needed for right now
}
  
void read_joystick(int & x_pos, int & y_pos) {
    int x_raw = analogRead(JOYSTICK_PIN_X);
    int y_raw = analogRead(JOYSTICK_PIN_Y);
    x_pos = map_smart(x_raw, 0, 1023, JOYSTICK_DELTA_HIGH_BOUND, JOYSTICK_DELTA_LOW_BOUND);
    y_pos = map_smart(y_raw, 0, 1023, JOYSTICK_DELTA_LOW_BOUND,  JOYSTICK_DELTA_HIGH_BOUND);
}
