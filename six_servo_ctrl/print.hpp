#include <Arduino.h>

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return Serial.write(c) == 1? 0 : 1;
}

// Setup for non garbage printing to console
// https://arduino.stackexchange.com/questions/176/how-do-i-print-multiple-variables-in-a-string
FILE serial_stdout;

void setup_printf() {
  Serial.begin(9600);
  fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial_stdout;
}
