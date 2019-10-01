#include "motor_tests.hpp"
#include <Arduino.h>

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return Serial.write(c) == 1? 0 : 1;
}

/* --------------------------- Environment Setup ---------------------*/

// Setup for non garbage printing to console
// https://arduino.stackexchange.com/questions/176/how-do-i-print-multiple-variables-in-a-string
FILE serial_stdout;

/* ------------------------ Main() Functions -----------------------*/

void setup() {
    Serial.begin(9600);
    setup_servos();
    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;
}

void loop() {
  delay_to_speed_test(servo_4);
  while(1) {}
}
