#define JOYSTICK_PIN_X    0
#define JOYSTICK_PIN_Y    1

#define X_NEUTRAL         506
#define Y_NEUTRAL         511
#define BIN_BOUNDS        2

int x_pos = 0;
int y_pos = 0;

void init_input() {
  // Nothing needed for right now
}

void run_input_test() {
  x_pos = analogRead(JOYSTICK_PIN_X);
  y_pos = analogRead(JOYSTICK_PIN_Y);

  printf("VRx: %d -- VRy: %d\r\n", x_pos, y_pos);
}
