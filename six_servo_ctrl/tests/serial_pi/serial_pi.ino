int incomingByte = 0; // for incoming serial data

char buff[100] = {0};

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  while (Serial.available() == 0) { }
  delay(200);
  Serial.println(Serial.available());
  // send data only when you receive data:
  if (Serial.available() >= 14) {
    // read the incoming byte:
    Serial.flush();
    Serial.readBytes(buff, 14);

    delay(2000);

    for (int i = 0; i < 14; i++) {
      Serial.println(buff[i]);
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);  
    }


  } else {
    Serial.println(Serial.available());
  }

  delay(2000);
}
