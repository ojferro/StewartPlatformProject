#include <Servo.h>
#include <Arduino.h>

// Servo movement limits in degrees
#define mechanical_angle_max    175
#define mechanical_angle_min    5

// Center Angle
#define middle_angle            90

#define functional_angle_max    175
#define functional_angle_min    5

// Movement delay (fastest)
#define mvmt_delay_min          1
#define mvmt_delay_max          30

// Pins are offset from their array mapping to their PWM pin mapping
#define servo_pin_offset  2

typedef enum {
  servo_1 = 0,
  servo_2,
  servo_3,
  servo_4,
  servo_5,
  servo_6,
  num_servos
} servos_idx_e;

extern Servo servos[num_servos];  

/* --------------------------- Motor Control Functions ------------------------------*/

void write_servo(int target_angle, servos_idx_e target_index) {
  if (target_index != num_servos) {
    // Ensure target angle within mechanical & functional limits
    if (target_angle < mechanical_angle_max && target_angle > mechanical_angle_min &&
        target_angle < functional_angle_max && target_angle > functional_angle_min) {
          servos[target_index].write(target_angle);
        }
  }
}

void write_all_servos(int target_angle) {
  if (target_angle < mechanical_angle_max && target_angle > mechanical_angle_min &&
      target_angle < functional_angle_max && target_angle > functional_angle_min) {
        for (int servo = servo_1; servo < num_servos; servo++) {
          servos[servo].write(target_angle);
        }
  }
}

void control_and_delay_motor(servos_idx_e servo, int target_angle, int next_angle) {
  int delay_time = map((180 - abs(target_angle - next_angle)), functional_angle_min, functional_angle_max, mvmt_delay_min, mvmt_delay_max);
  servos[servo].write(next_angle);
  delay(delay_time);
}

void simple_ramp(servos_idx_e servo, int target_angle) {
  int current_angle = servos[servo].read();
  int total_error = target_angle - current_angle;
  
  for (int i = abs(total_error); i > 0; i--) {
    // Get the next angle dependent on if going left or right
    current_angle = ((total_error > 0) ? (++current_angle) : (--current_angle));

    // Write next angle
    control_and_delay_motor(servo, target_angle, current_angle);
  }
}

void calibrate() {
  write_all_servos(middle_angle);
  // Remainder of calibration process & setup
}
