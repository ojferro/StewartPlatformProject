#include "print.hpp"
#include "input_pi.hpp"
#include "motor_control.hpp"

/* --------------------------- Environment Setup ---------------------*/
int x = 0;
int y = 0;

/* ------------------------ Main() Functions -----------------------*/

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    servos[servo_1].attach(servo_1 + servo_pin_offset); // pin 2
    servos[servo_2].attach(servo_2 + servo_pin_offset); // pin 3
    servos[servo_3].attach(servo_3 + servo_pin_offset); // pin 4
    servos[servo_4].attach(servo_4 + servo_pin_offset); // pin 5
    servos[servo_5].attach(servo_5 + servo_pin_offset); // pin 6
    servos[servo_6].attach(servo_6 + servo_pin_offset); // pin 7

    setup_printf();
}


void loop() {
  int angles[6] = {0};
  center();

  while(1) {
    
    if (get_joint_angles_from_pi(angles)) {
      print_array(angles, 6);
      move_motors_from_IK(angles);
      Serial.println("Done Moving Motors");
    }

    delay(300);
  }
}
