#ifndef INV_KIN_HPP
#define INV_KIN_HPP

#include "linmath.h"

#define R_NEUTRAL_W 1.570796 // 90 degrees
#define R_NEUTRAL_X 0
#define R_NEUTRAL_Y 0
#define R_NEUTRAL_Z 0

#define T_NEUTRAL_X 0
#define T_NEUTRAL_Y 0
#define T_NEUTRAL_Z 10 // Arbitrarily defined for now

#define SERVO_HORN_LENGTH 5.0
#define ROD_LENGTH 15.0

#define SERVO_HORN_ROTATION_1 50.0
#define SERVO_HORN_ROTATION_2 100.0
#define SERVO_HORN_ROTATION_3 150.0
#define SERVO_HORN_ROTATION_4 200.0
#define SERVO_HORN_ROTATION_5 250.0
#define SERVO_HORN_ROTATION_6 300.0

// Rotation quaternion from base to platform
quat R_pb = {R_NEUTRAL_W, R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z};
// Translation vector from base to platform (defined as pure quaternion with w=0 for ease of use)
quat T_pb = {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0};

// Servo horn rotations at build time (beta_k)
float beta_k[6] = {
    SERVO_HORN_ROTATION_1,
    SERVO_HORN_ROTATION_2,
    SERVO_HORN_ROTATION_3,
    SERVO_HORN_ROTATION_4,
    SERVO_HORN_ROTATION_5,
    SERVO_HORN_ROTATION_6
};

// Distance from servo shaft to servo end (where rod is connected)
float h = SERVO_HORN_LENGTH;

// Distance from servo base joint to platform joint (aka length of rod)
float d = ROD_LENGTH;

/* 
    Calculates l_k (effective leg length) from base to platform
    @params
    r: return value
    T: Vector (pure quaternion) defining translation from base to platform
    R: Quaternion defining rotation from base to platform
    p_k: Vector (pure quaternion) defining location of servo base wrt base origin
    b_k: Vector (pure quaternion) defining location of servo platform wrt platform origin
*/
void CalcLegLength(quat r, quat T, quat R, quat p_k, quat b_k) {
    // Inverse of a quaternion is its conjugate
    quat R_inv; 
    quat_conj(R_inv, R);

    // R X p_k
    quat_mul(r, R, p_k);

    // (R X p_k) X R_inv
    quat_mul(r, r, R_inv);

    // T + [(R X p_k) X R_inv]
    quat_add(r, T, r);

    // T + [(R X p_k) X R_inv] - b_k
    quat_sub(r, r, b_k);
}

// Calculates L2 norm (magnitude) for a given quaternion/vector
float CalcL2Norm(quat q) {
    int i;
    float result = 0;
    for (i=0; i<4; i++) {
        result += (q[i]*q[i]);
    }
    return sqrt(result);
}

// Calculates servo rotation angle (alpha_k)
float CalcAlpha(quat l_k, float B_k) {
    // Calculate e_k = 2*|h|*l_k[z]
    float e_k = 2*h*l_k[2];

    float f_k = 2*h*(cos(B_k)*l_k[0] + sin(B_k)*l_k[1]);

    // g_k = |l_k|^2 - (|d|^2 - |h|^2)
    float g_k = CalcL2Norm(l_k);
    g_k = g_k*g_k - (d*d - h*h);

    // Calculate and return alpha_k
    return asin(g_k/(sqrt(e_k*e_k + f_k*f_k))) - atan2(f_k, e_k);
}

#endif //INV_KIN_HPP