#include "motor.hpp"

// Dependent on this matrix multiplication library: https://playground.arduino.cc/Code/MatrixMath/
// Must be manually setup by the user (rly easy)
#include <MatrixMath.h>

/* --------------------------- Environment Setup ---------------------*/

// Grab Variable from motor.hpp. We are sharing global variables btwn files
Servo servos[num_servos];

/* ------------------------ Main() Functions -----------------------*/

void setup() {
    Serial.begin(9600);
    servos[servo_1].attach(servo_1 + servo_pin_offset); // pin 2
    servos[servo_2].attach(servo_2 + servo_pin_offset); // pin 3
    servos[servo_3].attach(servo_3 + servo_pin_offset); // pin 4
    servos[servo_4].attach(servo_4 + servo_pin_offset); // pin 5
    servos[servo_5].attach(servo_5 + servo_pin_offset); // pin 6
    servos[servo_6].attach(servo_6 + servo_pin_offset); // pin 7
}

void loop() {
  servos[servo_1].write(0);
  delay(15);
  Serial.println("STARTING NOW"); 
  servos[servo_1].write(180);
  
  while(1) {
    Serial.println(servos[servo_1].read());
  }

//  for (pos = mechanical_angle_min; pos < mechanical_angle_max; pos++) {
//    write_all_servos(pos);
//    delay(15);
//  }
//  
//  for (pos = mechanical_angle_max; pos > mechanical_angle_min; pos--) {
//    write_all_servos(pos);
//    delay(15);
//  }
}
