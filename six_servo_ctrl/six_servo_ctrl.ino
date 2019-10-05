#include "motor.hpp"
#include "input.hpp"
#include "print.hpp"

/* --------------------------- Environment Setup ---------------------*/
int x_pos = 0;
int y_pos = 0;

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

void loop() {
  printf("Starting\r\n");
  calibrate();
  delay(2000);

  printf("Moving Now\r\n");
  int angles[6] = {90, 90, 90, 90, 165, 45};
  move_motors(angles);

  printf("Done\r\n");

  while(1) {}
}
