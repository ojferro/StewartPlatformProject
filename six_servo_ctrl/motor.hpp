#include <Servo.h>
#include <Arduino.h>

// Servo movement limits in degrees
#define mechanical_angle_max    175
#define mechanical_angle_min    5

#define functional_angle_max    175
#define functional_angle_min    5

// Center Angle
#define middle_angle            90

// Other Operating Parameters
#define MINIMUM_DELAY           5
#define ERROR_BOUNDS            2

// Ramp Movement Delay Parameters (NOT USING RIGHT NOW)
#define mvmt_delay_min            2   // ms
#define mvmt_delay_max            30  // ms
#define ramp_distance_percentage  0.4 // %

// Delay & Velocity Mapping Constants -- V = a / (d ^ r)
#define a   1
#define r   1

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

// Directly included in other files
Servo servos[num_servos];  

void print_array(int arr[], int num) {
  for (int i = 0; i < num; i++) {
    printf("%d ", arr[i]);
  }
  printf("\r\n");
}

bool angle_in_bounds(int target_angle) {
  return ((target_angle < mechanical_angle_max) && (target_angle > mechanical_angle_min) &&
          (target_angle < functional_angle_max) && (target_angle > functional_angle_min));
}
void print_angles() {
  for (int servo = servo_1; servo < num_servos; servo++) {
    printf("Servo: %d - Angle: %d\r\n", servo, servos[servo].read());
  }
}

int diff_error_bounds(int angle, int reference) {
  return (abs(angle - reference) > ERROR_BOUNDS) ? (angle-reference) : 0;
}

/* --------------------------- Motor Actuation Functions ------------------------------*/

void write_servo_position(servos_idx_e servo, int target_angle) {
  // Ensure target angle within mechanical & functional limits
  if  (angle_in_bounds(target_angle)) {
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

void drive_motors(int target_angles[num_servos], int delay_time_ms[num_servos], int mvmt_direction[num_servos]) {
  uint8_t motor_moving = 0;
  int last_time_moved[num_servos] = {0};

  // Create a byte with 1 bit for each that needs to move
  for (int servo = 0; servo < num_servos; servo++) {
    motor_moving |= ((delay_time_ms[servo] != 0) << servo);
  }
 
  while (motor_moving != 0) { // This may be weird, but provides a simple check to see if all motors reached target
    
    for (int servo = 0; servo < num_servos; servo++) { // For each servo
            
      if ((motor_moving  & (1 << servo)) && (millis() - last_time_moved[servo]) > delay_time_ms[servo]) { // If servo not done & waited one delay cycle
        last_time_moved[servo] = millis();

        // Get current angle & move 1 degree (kind of an awkward implementation)
        int current_angle = write_servo_delta((servos_idx_e) servo, mvmt_direction[servo]);

        // If reached target angle for this servo, turn off the bit
        if (current_angle == target_angles[servo]) { // done
          motor_moving &= ~(1 << servo);
        }
      }
    }
  }
}

int map_velocity_to_delay(float omega) {
  if (omega == 0) {
    printf("ERROR: velocity = 0\r\n");
    return 0;
  } else {
    float temp_delay = 1.0 / omega;
    return round(temp_delay);
  }
}

float map_delay_to_velocity(int delay_ms) {
  if (delay_ms == 0) {
    return 0;
  } else {
    return 1.0 / float(delay_ms);
  }
}

/* --------------------------- Motor Control & Logic Functions ------------------------------*/


void calibrate() {
  for(int servo = servo_1; servo < num_servos; servo++) {
    servos[servo].write(90);
  }
  print_angles();
}


bool move_motors(int target_angles[num_servos]) {
  int error[num_servos] = {0};
  int mvmt_direction[num_servos] = {0};
  volatile float max_travel_time = 0; // Arduino IDE needs this to be volatile for some god forsaken reason
  float travel_time = 0;
  int slowest_servo; // Not using enum type because it reduces needed typecasts

  // Find distances & times for each motor given target
  for (int servo = 0; servo < num_servos; servo++) {
    // Return early if any angles are bad
    if (!angle_in_bounds(target_angles[servo])) {
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
      delay_time[servo] = int(error[servo] != 0); // min d = 1;
    } else if (error[servo] == 0) { 
      delay_time[servo] = 0; // No movement required for this motor
    } else { // Compute the slower w of each of the remaining motors
      desired_angular_speed = error[servo] / max_travel_time;
      delay_time[servo] = map_velocity_to_delay(abs(desired_angular_speed));
    }
  }

  drive_motors(target_angles, delay_time, mvmt_direction);  
  return true;
}
