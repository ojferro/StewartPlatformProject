#include "motor.hpp"
#include <Arduino.h>

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return Serial.write(c) == 1? 0 : 1;
}

/* --------------------------- Environment Setup ---------------------*/

// Grab Variable from motor.hpp. We are sharing global variables btwn files
Servo servos[num_servos];

// Setup for non garbage printing to console
// https://arduino.stackexchange.com/questions/176/how-do-i-print-multiple-variables-in-a-string
FILE serial_stdout;

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
