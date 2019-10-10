#include "input.hpp"
#include "print.hpp"

/* --------------------------- Environment Setup ---------------------*/
int x = 0;
int y = 0;

/* ------------------------ Main() Functions -----------------------*/

void setup() {
    Serial.begin(9600);
    servos[servo_1].attach(servo_1 + servo_pin_offset); // pin 2
    servos[servo_2].attach(servo_2 + servo_pin_offset); // pin 3
    servos[servo_3].attach(servo_3 + servo_pin_offset); // pin 4
    servos[servo_4].attach(servo_4 + servo_pin_offset); // pin 5
    servos[servo_5].attach(servo_5 + servo_pin_offset); // pin 6
    servos[servo_6].attach(servo_6 + servo_pin_offset); // pin 7

    setup_printf();
}

void control() {
  read_joystick(x, y);
  printf("Joystick: %03d -- %03d\r\n", x, y); 
  int angles[6] = {90, 90, 90, 90, 90, 90};

  { // Move Y
    if (diff_error_bounds(y, 0) > 0) {
      angles[servo_4] = middle_angle - y;
      angles[servo_3] = middle_angle + y;
    } else if (diff_error_bounds(y, 0) < 0) {
      angles[servo_1] = middle_angle + y;
      angles[servo_2] = middle_angle + y;
      angles[servo_5] = middle_angle - y;
      angles[servo_6] = middle_angle - y;
    }
  }
  
  { // Move X
    if (diff_error_bounds(x, 0) < 0) {
     angles[servo_1] = middle_angle - x;
     angles[servo_2] = middle_angle - x;
     angles[servo_3] = middle_angle + x;
    } else if (diff_error_bounds(x, 0) > 0) {
     angles[servo_4] = middle_angle - x;
     angles[servo_5] = middle_angle + x;
     angles[servo_6] = middle_angle - x; // Turned down
    }
  }

  move_motors(angles);
  
  for (int i = 0; i < 6; i++) {
    angles[i] = servos[i].read();
  }

}


void loop() {
  calibrate();  
  delay(2000);
  
  while(1) {
    control();
    delay(5);
  }
}
