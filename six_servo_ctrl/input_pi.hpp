#define PACKET_LENGTH (1 + 2 + 2 + 2 + 2 + 2 + 2 + 1) // 2 Delimiters, 6 * 2bytes

void setup_pi_serial() {
  Serial.begin(9600);
}

void clean_garbage_packet() {
  // read unreliable serial data until we either find the next packet or run out of data
  while(Serial.available() > 0) {
    // Unless next byte is start of a good packet (let the next call handle it), read & ignore it
    if (Serial.peek() == '[') {
      break;
    } else {
      Serial.read();
    }
  }
  
}

bool parse_good_packet(int angles[6])   {
   // parse through & format data from good packet
  if (Serial.available() >= PACKET_LENGTH) {
    
    Serial.read(); // Read through open delimiter
    for (int i = 0; i < 6; i++) {
      // Data is coming in little endian & should be saved little endian
      angles[i] = Serial.read() | (Serial.read() << 8);
    }
        
    if (Serial.read() == ']') {
      return true;    
    } else { // Invalid close delimiter
      clean_garbage_packet();
      return false;
    }
    
  } else {
    return false;
  }
  
}
 
bool get_joint_angles_from_pi(int angles[6]) {
  if (Serial.available() > 0) { // If data available
    
    if (Serial.peek() == '[') { // If open delimiter is correct
      return parse_good_packet(angles);

    } else { // If not the open delimiter, we do not know @ what point in the packet we are, incomplete data
      clean_garbage_packet();
      return false;
    }
    
  } else {
    return false;
  }
}
