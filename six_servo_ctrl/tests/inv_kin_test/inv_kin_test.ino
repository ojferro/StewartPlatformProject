#include <Servo.h>
#include "linmath.h"
#include "motor.hpp"
#include "inv_kin.hpp"


// Rotation quaternion from base to platform
quat q1 = {3, 8, 4.5, 1.5};
quat q2 = {0, 1, 0, 1};
vec3 v1 = {1, 0, 2};
vec3 v2 = {1, -4, -5};


void setup() {
    Serial.begin(9600);
}

void PrintQuaternion(quat q, char* log_message) {
    int i;
    Serial.print(log_message);
    Serial.print(" [");
    for (i=0; i<4; i++) {
        Serial.print(q[i]);
        Serial.print(", ");
    }
    Serial.println("]");
}

void PrintVec3(vec3 v, char* log_message) {
    int i;
    Serial.print(log_message);
    Serial.print(" [");
    for (i=0; i<4; i++) {
        Serial.print(v[i]);
        Serial.print(", ");
    }
    Serial.println("]");
}

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

void loop() {
    
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
    
    while(1) {
      
    }
}
