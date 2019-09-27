#include <Servo.h>
#include "MatrixMath.h"

// THIS IS ONLY TEST CODE, NOT TO BE HOOKED UP TO MOTORS

// Servo movement limits in degrees
#define mechanical_angle_max    175
#define mechanical_angle_min    5

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

// Global Variables
Servo servos[num_servos];
int pos = 0;
float A[6][6] = {1.4234,5.3231,2.4231,3.1231,3.2231,1.4231,4.7231,6.1231,5.2231,1.1231,6.3231,5.4231,7.7231,8.1231,8.1231,2.1231,9.1231,3.9231,
                 1.4234,5.3231,2.4231,3.1231,3.2231,1.4231,4.7231,6.1231,5.2231,1.1231,6.3231,5.4231,7.7231,8.1231,8.1231,2.1231,9.1231,3.9231};
                 
float B[6][6] = {5.1231,2.5231,4.2231,3.1231,3.3231,4.1231,6.4231,5.4231,7.5231,6.2231,1.6231,7.5231,2.7231,8.7231,3.8231,9.2231,5.9231,1.2223,
                 5.1231,2.5231,4.2231,3.1231,3.3231,4.1231,6.4231,5.4231,7.5231,6.2231,1.6231,7.5231,2.7231,8.7231,3.8231,9.2231,5.9231,1.2231};

void setup() {
    Serial.begin(9600);
    servos[servo_1].attach(servo_1 + servo_pin_offset);
    servos[servo_2].attach(servo_2 + servo_pin_offset);
    servos[servo_3].attach(servo_3 + servo_pin_offset);
    servos[servo_4].attach(servo_4 + servo_pin_offset);
    servos[servo_5].attach(servo_5 + servo_pin_offset);
    servos[servo_6].attach(servo_6 + servo_pin_offset);
}

void matrix_test() {
  Matrix.Print((float*)A, 6, 6, "A");
  Matrix.Print((float*)B, 6, 6, "B");
  
  float C[6][6] = {0};
  unsigned long time_before = millis();
  Matrix.Multiply((float*)A, (float*)B, 6, 6, 6, (float*)C);
  unsigned long time_after = millis();
  Serial.print("Time Taken: ");
  Serial.println((time_after - time_before));
  Serial.println(time_before);
  Serial.println(time_after);
  Matrix.Print((float*)C, 6, 6, "C");

  // Stop from looping
  while(1) {}
}

void write_servo(int target_angle, servos_idx_e target_index) {
  if (target_index != num_servos) {
    // Ensure target angle within mechanical & functional limits
    if (target_angle < mechanical_angle_max && target_angle > mechanical_angle_min &&
        target_angle < functional_angle_max && target_angle > functional_angle_min) {
          servos[target_index].write(target_angle);
        }
  }
}

void write_all_seros(int target_angle) {
  if (target_angle < mechanical_angle_max && target_angle > mechanical_angle_min &&
      target_angle < functional_angle_max && target_angle > functional_angle_min) {
          servos[servo_1].write(target_angle);
          servos[servo_2].write(target_angle);
          servos[servo_3].write(target_angle);
          servos[servo_4].write(target_angle);
          servos[servo_5].write(target_angle);
          servos[servo_6].write(target_angle);
  }
}

void loop() {
  matrix_test();
  
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      for (int servo = 0; servo < 6;servo += 1) {
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
}
