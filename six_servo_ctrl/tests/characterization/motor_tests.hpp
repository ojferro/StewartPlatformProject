#include <Servo.h>
#include <Arduino.h>

#define servo_pin_offset 2

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

void setup_servos() {
  servos[servo_1].attach(servo_1 + servo_pin_offset); // pin 2
  servos[servo_2].attach(servo_2 + servo_pin_offset); // pin 3
  servos[servo_3].attach(servo_3 + servo_pin_offset); // pin 4
  servos[servo_4].attach(servo_4 + servo_pin_offset); // pin 5
  servos[servo_5].attach(servo_5 + servo_pin_offset); // pin 6
  servos[servo_6].attach(servo_6 + servo_pin_offset); // pin 7
}

void read_test(servos_idx_e servo) {
  while(1) {
    printf("%d\r\n", servos[servo].read());
  }
}

void delay_to_speed_test(servos_idx_e servo) {
  const int max_delay = 30;
  const int min_angle = 0;
  const int max_angle = 180;
  long int start_time = 0;
  long int middle_time = 0;
  long int end_time = 0;
  long int time_fwd = 0;
  long int time_bkwd = 0;

  for (int i = 0; i < max_delay; i++) {
    servos[servo].write(0);
    delay(500);

    start_time = micros();
    for (int a = min_angle; a < max_angle; a++) {
      servos[servo].write(a);
      delay(i);
    }
    
    middle_time = micros();    
    for (int a = max_angle; a >= min_angle; a--) {
      servos[servo].write(a);
      delay(i);
    }

    end_time = micros();
    time_fwd = middle_time-start_time;
    time_bkwd = end_time-middle_time;

    Serial.print(i);
    Serial.print(", ");
    Serial.print(time_fwd);
    Serial.print(", ");
    Serial.println(time_bkwd);
  }
}
