#include <Servo.h>
#include "linmath.h"
#include "motor.hpp"
#include "inv_kin.hpp"

#include <Arduino.h>

// Function that printf and related will use to print
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return Serial.write(c) == 1? 0 : 1;
}

FILE serial_stdout;


// Rotation quaternion from base to platform
quat q1 = {3, 8, 4.5, 1.5};
quat q2 = {0, 1, 0, 1};
vec3 v1 = {1, 0, 2};
vec3 v2 = {1, -4, -5};

void TestQuaternionMath(void) {
    quat q_ans;
    float f_ans;
    vec3 v_ans;

    PrintQuaternion(q1, "q1 = ");
    PrintQuaternion(q2, "q2 = ");
    PrintVec3(v1, "v1 = ");
    PrintVec3(v2, "v2 = ");

    // Verify quaternion addition/subtraction
    quat_add(q_ans, q1, q2);
    PrintQuaternion(q_ans, "q1 + q2 = ");

    // Verify quaternion X quaternion
    quat_mul(q_ans, q1, q2);
    PrintQuaternion(q_ans, "q1 X q2 = ");
    
    // Verify quaternion . quaternion
    f_ans = quat_inner_product(q1, q2);
    Serial.print("q1 . q2 = ");
    Serial.println(f_ans);

    // Verify vector X quaternion
    quat_mul_vec3(v_ans, q1, v1);
    PrintVec3(v_ans, "q1 X v1 = ");
}

void TestIkNeutral(void) {
  for (int i=0; i<num_servos; i++) {
      // Calculate effective leg length for each servo
      CalcLegLength(eff_leg_lengths[i], bp_translations[i], bp_rots[i], servo_pcoords[i], servo_bcoords[i]);
      // Compute corresponding servo angle
      target_angles[i] =  (int)RAD2DEG(CalcAlpha(eff_leg_lengths[i], betas[i]));

      Serial.print("alpha[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(target_angles[i]);
  }
  move_motors(target_angles);
}

// void TestIkIncremental(void) {
//   float alpha=0;
//     for (float x=-5; x<5; x++) {
//       for (float y=-5; y<5; y++) {
//         for (float z=0; z<5; z++) {
//           for (float w=0; w<180; w+=30) {
//             for (int i=0; i<num_servos; i++) {
//               bp_rots[i][0] = x;
//               bp_rots[i][1] = y;
//               bp_rots[i][2] = z;
//               bp_rots[i][3] = DEG2RAD(w);
//               NormalizeQuaternion(bp_rots[i]);
  
//               // Calculate effective leg length for each servo
//               CalcLegLength(eff_leg_lengths[i], bp_translations[i], bp_rots[i], servo_pcoords[i], servo_bcoords[i]);
//               PrintQuaternion(eff_leg_lengths[i], "eff_leg_lengths");
//               // Compute corresponding servo angle
//               alpha = CalcAlpha(eff_leg_lengths[i], betas[i]);
//               Serial.println(alpha);
//               target_angles[i] =  (int)RAD2DEG(alpha);
  
//               Serial.println(y);
//               Serial.println(w);
//   //            printf("<x,y,z,w> = <%d,%d,%d,%d>\n", x*100, y*100, z*100, w*100);
//   //            printf("alpha[%d] = %d\n", i, target_angles[i]);
              
//               Serial.print("alpha[");
//               Serial.print(i);
//               Serial.print("] = ");
//               Serial.println(target_angles[i]);
//               Serial.println();
//               delay(1000);
//             }
//           }
//         }
//       }
//   }
// }

void TestIkIncremental(void) {
  int i =0;
  float theta = 0.25;
  bp_rots[0][0] = -0.71;
  bp_rots[0][1] = -0.71;
  bp_rots[0][2] = 0;
  bp_rots[0][3] = cos(theta/2);;

  for (int j=0; j<3; j++) {
    bp_rots[0][j] = bp_rots[0][j] * sin(theta/2);
  }
  NormalizeQuaternion(bp_rots[i]);

  // Calculate effective leg length for each servo
  CalcLegLength(eff_leg_lengths[i], bp_translations[i], bp_rots[i], servo_pcoords[i], servo_bcoords[i]);
  PrintQuaternion(eff_leg_lengths[i], "eff_leg_lengths");
  // Compute corresponding servo angle
  float alpha = CalcAlpha(eff_leg_lengths[i], betas[i]);
  Serial.println(alpha);
  target_angles[i] =  (int)RAD2DEG(alpha);

//  Serial.println(y);
//  Serial.println(w);
//            printf("<x,y,z,w> = <%d,%d,%d,%d>\n", x*100, y*100, z*100, w*100);
//            printf("alpha[%d] = %d\n", i, target_angles[i]);
  
  Serial.print("alpha[");
  Serial.print(i);
  Serial.print("] = ");
  Serial.println(target_angles[i]);
  Serial.println();
  delay(1000);
}

//void TestIKRotation(void) {
//  vec3 leg_length;
//  vec3 xyz_angles = {DEG2RAD(0), DEG2RAD(15), DEG2RAD(0)};
//  CalcLegLength(leg_length, bp_translations, xyz_angles, servo_pcoords, servo_bcoords);
//  PrintVec3(leg_length);
//
//  float alpha = CalcAlpha(leg_length, betas[i]);
//  Serial.print("alpha: ");
//  Serial.println(alpha);
//}

void NormalizeQuats(quat quats[num_servos]) {
  for (int i=0; i<num_servos; i++) {
    NormalizeQuaternion(quats[i]);
  }
}

void setup() {
    Serial.begin(9600);
    fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_stdout;

    NormalizeQuats(bp_rots);
}

void loop() {
  quat result;
  quat A = {1,2,3,4};
  quat B = {5,6,7,8};
  QuatMult(result, A,B);
  PrintQuaternion(result, "Test quat mult");
  // TestIKRotation();
  TestIkIncremental();
  while(1) {
    
  }
}
