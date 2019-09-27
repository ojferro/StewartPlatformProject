#include <Servo.h>
#include "linmath.h"


// Rotation quaternion from base to platform
quat q1 = {3, 8, 4.5, 1.5};
quat q2 = {0, 1, 0, 1};
vec3 v1 = {1, 0, 2};
vec3 v2 = {1, -4, -5};

// temporary placeholders
#define X 0
#define Y 0
#define Z 0


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

void loop() {
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

    quat p_ks[6] = {
        {X, Y, Z, 0},
        {X, Y, Z, 0}, 
        {X, Y, Z, 0},
        {X, Y, Z, 0},
        {X, Y, Z, 0},
        {X, Y, Z, 0}
    };

    quat b_ks[6] = {
        {X, Y, Z, 0},
        {X, Y, Z, 0}, 
        {X, Y, Z, 0},
        {X, Y, Z, 0},
        {X, Y, Z, 0},
        {X, Y, Z, 0}
    };
    
    while(1) {
      
    }
}
