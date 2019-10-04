#include <Servo.h>
#include <Arduino.h>

// Servo movement limits in degrees
#define mechanical_angle_max    175
#define mechanical_angle_min    5

// Center Angle
#define middle_angle            90

#define functional_angle_max    175
#define functional_angle_min    5

// Ramp Movement Delay Parameters
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

Servo servos[num_servos];  

float max_angular_speed[6] = {1.0};

/* --------------------------- Motor Actuation Functions ------------------------------*/

void write_servo_position(servos_idx_e servo, int target_angle) {
  // Ensure target angle within mechanical & functional limits
  if (target_angle < mechanical_angle_max && target_angle > mechanical_angle_min &&
      target_angle < functional_angle_max && target_angle > functional_angle_min) {
        servos[servo].write(target_angle);
  }
}


int write_servo_delta(servos_idx_e servo, int delta) {
  int current_angle = servos[servo].read();
  write_servo_position(servo, current_angle + delta);
  return servos[servo].read();
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

void drive_motors(int target_angles[6], int delay_time[6], int mvmt_direction[6]) {
  uint8_t motor_moving = 0b00111111;
  int last_time_moved[6] = {0};
  int current_angle = 0;

  while (motor_moving != 0) { // This may be weird, but provides a simple check to see if all motors reached target
    for (int servo = 0; servo < num_servos; servo++) { // For each servo
      if ((millis() - last_time_moved[servo]) > delay_time[servo]) { // If we have waited one delay cycle for this servo
        last_time_moved[servo] = millis();

        // Get current angle & move 1 degree (kind of an awkward implementation)
        current_angle = write_servo_delta((servos_idx_e)servo, mvmt_direction[servo]);

        // If reached target angle for this servo, turn off the bit
        if (current_angle == target_angles[servo]) { //done
          motor_moving &= ~(1 << servo);
        }
      }
    }
  }
}

/* --------------------------- Motor Control & Logic Functions ------------------------------*/


void calibrate() {
  // Calibration process & setup
}


void move_motors(int target_angles[6]) {
  int error[6] = {0};
  int mvmt_direction[6] = {0};
  float max_travel_time = 0;
  float travel_time = 0;
  int slowest_servo; // Not using enum type because it reduces needed typecasts

  // Find distances & times for each motor given target
  for (int servo; servo < num_servos; servo++) {
    error[servo] = (servos[servo].read() - target_angles[servo]);
    mvmt_direction[servo] = (error[servo] < 0) ? -1 : 1;
    travel_time = (abs(error[servo]) / max_angular_speed[servo]); // FILL IN WITH VELOCITY CALCS

    // Get the slowest motor
    if (travel_time > max_travel_time) {
      max_travel_time = travel_time;
      slowest_servo = servo;
    }
  }

  // Find desired speed for each motor given longest travel time
  // Then find required delay in ms for each calculated speed
  int delay_time[6] = {0};
  float desired_angular_speed = 0;
  for (int servo; servo < num_servos; servo++) {
    if (servo == slowest_servo) { // Use the existing max speed of slowest servo
      delay_time[servo] = 0; // FILL IN WITH VELOCITY CALCS
    } else { // Compute the slower w of each of the remaining motors
      desired_angular_speed = error[servo] / max_travel_time;
      delay_time[servo] = 0; // FILL IN WITH VELOCITY CALCS
    }
  }

  return drive_motors(target_angles, delay_time, mvmt_direction);  
}
