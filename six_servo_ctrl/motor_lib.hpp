#include <Servo.h>
#include <Arduino.h>

/* --------------------- Data Definitions -----------------------*/

// Servo movement limits in degrees
#define mechanical_angle_max    180
#define mechanical_angle_min    0

// Servo Functional Limits Index (more readable code)
#define max_limit               1
#define min_limit               0

// Inverse Kinematic Reference Conversion Parameters
#define ik_angle_max            90
#define ik_angle_min           -45

// Center Angle
#define middle_angle            90

// Other Operating Parameters
#define MINIMUM_DELAY           17
#define ERROR_BOUNDS            2

// Ramp Movement Delay Parameters (NOT USING RIGHT NOW)
#define mvmt_delay_min            2   // ms
#define mvmt_delay_max            30  // ms
#define ramp_distance_percentage  0.4 // %

// Pins are offset from their array mapping to their PWM pin mapping
#define servo_pin_offset  2

/*----------------- Data Declarations -------------------------------*/

typedef enum {
  servo_1 = 0,
  servo_2,
  servo_3,
  servo_4,
  servo_5,
  servo_6,
  num_servos
} servos_idx_e;

// Globally accessile array for motor control
Servo servos[6];

// Metadata used for IK angle conversion to driver reference frame
int conversion_polarity[num_servos] = {-1, 1, -1, 1, -1, 1};
int functional_limits[num_servos][2] = 
{ {0, 135}, {45, 180}, {0, 135}, {45, 180}, {0, 135}, {45, 180} };

// Angles for specific calibration forms
int motor_calib_level[num_servos] = {85, 95, 92, 85, 95, 95};
int motor_calib_center[num_servos] = {87, 86, 88, 92, 91, 90};

/* ----------------- Helper Functions ---------------------------------*/ 

void print_array(int arr[], int num) {
  for (int i = 0; i < num; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.print("\r\n");
}

// Returns true if angle is good
bool angle_in_bounds(int target_angle, int servo) {
  return ((target_angle <= functional_limits[servo][max_limit]) && (target_angle >= functional_limits[servo][min_limit]) &&
          (target_angle <= mechanical_angle_max) && (target_angle >= mechanical_angle_min));
}

void print_angles() {
  for (int servo = servo_1; servo < num_servos; servo++) {
    printf("Servo: %d - Angle: %d\r\n", servo, servos[servo].read());
  }
  printf("\r\n");
}

int diff_error_bounds(int angle, int reference) {
  return (abs(angle - reference) > ERROR_BOUNDS) ? (angle-reference) : 0;
}

/* --------------------------- Motor Actuation Functions ------------------------------*/

void write_servo_position(servos_idx_e servo, int target_angle) {
  // Ensure target angle within mechanical & functional limits
  if  (angle_in_bounds(target_angle, servo)) {
        servos[servo].write(target_angle);
  } else {
    printf("INVALID ANGLE: [%d] %d\r\n", (int)servo, target_angle);
  }
}

int write_servo_delta(servos_idx_e servo, int delta) {
  int current_angle = servos[servo].read();
  write_servo_position(servo, current_angle + delta);
  return servos[servo].read();
}

/*
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
*/

void drive_motors(int target_angles[num_servos], int delay_time_ms[num_servos], int mvmt_direction[num_servos]) {
  uint8_t motor_moving = 0;
  int last_time_moved[num_servos] = {0};
  
  // unsigned long start_time_4 = micros();
  // Serial.print("drive_motors Start Time:");
  // Serial.println(start_time_4);
  
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

  // unsigned long end_time_4 = micros();
  // Serial.print("drive_motors End Time:");
  // Serial.println(end_time_4);
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
