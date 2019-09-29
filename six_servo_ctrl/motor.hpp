#include <Servo.h>
#include <Arduino.h>

// Servo movement limits in degrees
#define mechanical_angle_max    175
#define mechanical_angle_min    5

// Center Angle
#define middle_angle            90

#define functional_angle_max    175
#define functional_angle_min    5

// Movement delay parameters
#define mvmt_delay_min            2   // ms
#define mvmt_delay_max            30  // ms
#define ramp_distance_percentage  0.4 // %

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

void simple_ramp(servos_idx_e servo, int target_angle) {
  int original_angle = servos[servo].read();
  int current_angle = original_angle;
  int delay_time = mvmt_delay_max;
  int total_error = abs(target_angle - current_angle);
  int ramp_dist = ceil(float(total_error * ramp_distance_percentage));

  for (int i = total_error; i > 0; i--) {
    if ((abs(original_angle - current_angle) < ramp_dist)) { // In Ramp-Up
      delay_time = map(abs(original_angle - current_angle), 0, ramp_dist, mvmt_delay_max, mvmt_delay_min);
    } else if ((abs(current_angle - target_angle) < ramp_dist)) { // In Ramp-Down
      delay_time = map(abs(current_angle - target_angle), 0, ramp_dist, mvmt_delay_max, mvmt_delay_min);
    } // Else Maintain Speed
    
    current_angle = (target_angle > original_angle) ? (++current_angle) : (--current_angle);
    servos[servo].write(current_angle);
    delay(delay_time);
  }
}

void calibrate() {
  write_all_servos(middle_angle);
  // Remainder of calibration process & setup
}
