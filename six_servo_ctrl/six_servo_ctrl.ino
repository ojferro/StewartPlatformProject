#include <Servo.h>

// Dependent on this matrix multiplication library: https://playground.arduino.cc/Code/MatrixMath/
// Must be manually setup by the user (rly easy)
#include <MatrixMath.h>

// Servo movement limits in degrees
#define mechanical_angle_limit_max    175
#define mechanical_angle_limit_min    5

#define functional_angle_limit_max    175
#define functional_angle_limit_min    5

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
int pos = 0;

void setup() {
    servos[servo_1].attach(servo_1 + servo_pin_offset);
    servos[servo_2].attach(servo_2 + servo_pin_offset);
    servos[servo_3].attach(servo_3 + servo_pin_offset);
    servos[servo_4].attach(servo_4 + servo_pin_offset);
    servos[servo_5].attach(servo_5 + servo_pin_offset);
    servos[servo_6].attach(servo_6 + servo_pin_offset);    
}

void write_servo(int target_angle, servos_idx_e target_index) {
  if (target_index != num_servos) {
    // Ensure target angle within mechanical & functional liimits
    if (target_angle < mechanical_angle_limit_max && target_angle > mechanical_angle_limit_min &&
        target_angle < functional_angle_limit_max && target_angle > functional_angle_limit_min) {
          servos[target_index].write(target_angle);
        }
  }
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      for (int servo = 0; servo < 6;servo += 1){
        write_servo(pos, (servos_idx_e) servo);
        delay(1);
      }
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    for (int servo = 0; servo < 6; servo += 1){
        write_servo(pos, (servos_idx_e) servo);
        delay(1);
  }
}
