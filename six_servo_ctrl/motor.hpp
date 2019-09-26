#include <Servo.h>

// Servo movement limits in degrees
#define mechanical_angle_max    175
#define mechanical_angle_min    5

// Center Angle
#define middle_angle            90

#define functional_angle_max    175
#define functional_angle_min    5

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

void calibrate() {
  write_all_servos(middle_angle);
  // Remainder of calibration process & setup
}