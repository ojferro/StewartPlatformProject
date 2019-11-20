#include "motor_lib.hpp"

/* --------------------------- Motor Control & Logic Functions ------------------------------*/

bool move_motors(int target_angles[num_servos]) {
  int error[num_servos] = {0};
  int mvmt_direction[num_servos] = {0};
  volatile float max_travel_time = 0; // Arduino IDE needs this to be volatile for some god forsaken reason
  float travel_time = 0;
  int slowest_servo; // Not using enum type because it reduces needed typecasts

  // Find distances & times for each motor given target
  for (int servo = servo_1; servo < num_servos; servo++) {
    // Return early if any angles are bad
    if (!angle_in_bounds(target_angles[servo], servo)) {
      printf("Invalid angle %d passed for servo %d (0-indexed!!!) to move_motors", target_angles[servo], servo);
      return false;
    }
    
    error[servo] = (target_angles[servo] - servos[servo].read());
    mvmt_direction[servo] = (error[servo] < 0) ? -1 : 1;
    travel_time = (abs(error[servo]) / map_delay_to_velocity(MINIMUM_DELAY)); // V = 1 / d, min d = 1 -> Vmax = 1

    // Get the slowest motor
    if (travel_time > max_travel_time) {
      max_travel_time = travel_time;
      slowest_servo = servo;
    }
  }

  // Find desired speed for each motor given longest travel time
  // Then find required delay in ms for each calculated speed
  int delay_time[num_servos] = {0};
  float desired_angular_speed = 0;
  
  for (int servo = 0; servo < num_servos; servo++) {
    if (servo == slowest_servo) { // Use the existing max speed of slowest servo
      delay_time[servo] = (error[servo] == 0) ? 0 : MINIMUM_DELAY; // min d = 1;
    } else if (error[servo] == 0) { 
      delay_time[servo] = 0; // No movement required for this motor
    } else { // Compute the slower w of each of the remaining motors
      desired_angular_speed = error[servo] / max_travel_time;
      delay_time[servo] = map_velocity_to_delay(abs(desired_angular_speed));
    }
  }

  printf("Delays: ");
  print_array(delay_time, 6);

  drive_motors(target_angles, delay_time, mvmt_direction);  
  return true;
}


void move_motors_from_IK(int alpha[num_servos]) {
  int theta[num_servos] = {0};
  
  for (int servo = servo_1; servo < num_servos; servo++) {
    if (alpha[servo] > ik_angle_max || alpha[servo] < ik_angle_min) {
      printf("Invalid Angle %d for servo %d passed from IK (0-indexed!!!!)\r\n", alpha[servo], servo);
      return;
    }

    theta[servo] = 90 + (conversion_polarity[servo]*alpha[servo]);
  }

  Serial.print("Given alpha = ");
  print_array(alpha, 6);
  Serial.print("Converted to ");
  print_array(theta, 6);

  move_motors(theta);
}


void center() {
  printf("Centering Motors\r\n");
  for (int i = 0; i < num_servos; i++) {
    servos[i].write(motor_calib_center[i]); 
  }
  
  print_angles();
}


/* -------- Control Used In Initial Prototype Demo --------------- */ 
void simple_control(int x, int y) {
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
