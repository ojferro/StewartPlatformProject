#include <Servo.h>

Servo servos[6];  // create servo object to control a servo
 
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  for (int i = 0; i<6;i++){
    servos[i].attach(i+2);
  }
//  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      for (int serv=0; serv<6;serv+=1){
        servos[serv].write(pos);              // tell servo to go to position in variable 'pos'
        delay(1);                       // waits 15ms for the servo to reach the position
      }
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    for (int serv=0; serv<6;serv+=1){
        servos[serv].write(pos);              // tell servo to go to position in variable 'pos'
        delay(1);                       // waits 15ms for the servo to reach the position
    }                  // waits 15ms for the servo to reach the position
  }
}
