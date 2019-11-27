#include "print.hpp"
#include "input_pi.hpp"
#include "motor_control.hpp"

/* --------------------------- Environment Setup ---------------------*/
int x = 0;
int y = 0;

/* ------------------------ Main() Functions -----------------------*/

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    // Set PWM pint to 0 first (no random motor spurtz @ beginning)
    // Then Attach to servo
    for (int i = servo_1; i < num_servos; i++) {
      int servo_pin = i + servo_pin_offset;
      pinMode(servo_pin, OUTPUT);
      digitalWrite(servo_pin, LOW);
      servos[i].attach(servo_pin);
    }

    setup_printf();
}

void run_from_pi() {
  int angles[6] = {0};

  while(1) {
    if (get_joint_angles_from_pi(angles)) {
      // print_array(angles, 6);
      
//      unsigned long start_time = micros();
//      Serial.print("Motor Math Start Time:");
//      Serial.println(start_time);
      
      move_motors_from_IK(angles);
      
//      unsigned long end_time = micros();
//      Serial.print("Motor Math End Time:");
//      Serial.println(end_time);
//
//      unsigned long diff = end_time - start_time;
//      printf("Done Moving Motors, Total Time Delta: %lu\r\n", diff);
    }
  }
}


void loop() {
  center_motors();
  
  run_from_pi();
}
