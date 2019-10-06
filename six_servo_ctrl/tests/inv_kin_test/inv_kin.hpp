#ifndef INV_KIN_HPP
#define INV_KIN_HPP

#include "linmath.h"

#define DEG2RAD(degree) (degree*M_PI/180.0)
#define RAD2DEG(radian) (radian*180.0/M_PI)

#define R_NEUTRAL_W (DEG2RAD(0)) // In RADIANS
#define R_NEUTRAL_X 0
#define R_NEUTRAL_Y 0
#define R_NEUTRAL_Z 0

#define T_NEUTRAL_X 0
#define T_NEUTRAL_Y 0
#define T_NEUTRAL_Z 20.2 // Arbitrarily defined for now

#define SERVO_HORN_LENGTH 16.52
#define ROD_LENGTH 190

// Servo horn rotations at build time (beta_k) in RADIANS
float betas[num_servos] = {
    DEG2RAD(0),
    DEG2RAD(240),
    DEG2RAD(240),
    DEG2RAD(120),
    DEG2RAD(120),
    DEG2RAD(0)
};

// Distance from servo shaft to servo end (where rod is connected)
float h = SERVO_HORN_LENGTH;

// Distance from servo base joint to platform joint (aka length of rod)
float d = ROD_LENGTH;

// Defines coordinate of servo joint on base wrt base origin
quat servo_bcoords[num_servos] = {
    {-30.0, -86.75, 0, 0},
    {-90.13, 17.4, 0, 0}, 
    {-60.13, 69.36, 0, 0},
    {60.13, 69.36, 0, 0},
    {90.13, 17.4, 0, 0},
    {30, -86.75, 0, 0}
};

// Defines coordinate of servo joint on platform wrt platform origin
quat servo_pcoords[num_servos] = {
    {-44.68, -51.78, 0, 0},
    {-67.18, -12.8, 0, 0},
    {-22.5, 64.58, 0, 0},
    {22.5, 64.58, 0, 0},
    {67.18, -12.8, 0, 0},
    {44.68, -51.78, 0, 0}
};

// Stores calculated target angles we want to reach
int target_angles[num_servos];

// Effective leg lengths (l_k values)
quat eff_leg_lengths[num_servos];

// Defines translation from base to platform
quat bp_translations[num_servos] = {
    {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0},
    {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0},
    {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0},
    {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0},
    {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0},
    {T_NEUTRAL_X, T_NEUTRAL_Y, T_NEUTRAL_Z, 0}
};

// Defines rotation from base to platform
quat bp_rots[num_servos] = {
    {R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z, R_NEUTRAL_W},
    {R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z, R_NEUTRAL_W},
    {R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z, R_NEUTRAL_W},
    {R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z, R_NEUTRAL_W},
    {R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z, R_NEUTRAL_W},
    {R_NEUTRAL_X, R_NEUTRAL_Y, R_NEUTRAL_Z, R_NEUTRAL_W}
};

void QuatMult(quat r, quat A, quat B){
    r[0] = B[0] * A[3] + B[1] * A[2] - B[2] * A[1] + B[3] * A[0];
    r[1] = -B[0] * A[2] + B[1] * A[3] + B[2] * A[0] + B[3] * A[1];
    r[2] =  B[0] * A[1] - B[1] * A[0] + B[2] * A[3] + B[3] * A[2];
    r[3] = -B[0] * A[0] - B[1] * A[1] - B[2] * A[2] + B[3] * A[3];
}

float DotProd(float v1[], float v2[]){
    return (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]);
}

void MatMult(float result[3], float mat[3][3], float vec[3]){
    for (int i =0; i<3;  i++){
        result[i] = DotProd(mat[i],vec);
    }
}

void CreateRotMat(float result [3][3], float phi, float theta, float tri){
    result[0][0] = cos(tri)*cos(theta);
    result[0][1] = -sin(tri)*cos(phi)+cos(tri)*sin(theta)*sin(phi);
    result[0][2] = sin(tri)*sin(phi)+cos(tri)*sin(theta)*cos(phi);
    result[1][0] = sin(tri)*cos(theta);
    result[1][1] = cos(tri)*cos(phi)+sin(tri)*sin(theta)*sin(phi);
    result[1][2] = -cos(tri)*sin(phi)+sin(tri)*sin(theta)*cos(phi);
    result[2][0] = -sin(theta);
    result[2][1] = cos(theta)*sin(phi);
    result[2][2] = cos(theta)*cos(phi);
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
    for (i=0; i<3; i++) {
        Serial.print(v[i]);
        Serial.print(", ");
    }
    Serial.println("]");
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

void NormalizeQuaternion(quat R) {
  float norm_R = CalcL2Norm(R);

  for (int i=0; i<4; i++) {
    R[i] = R[i]/norm_R;
  }
}

/* 
    Calculates l_k (effective leg length) from base to platform
    @params
    r: return value
    T: Vector (pure quaternion) defining translation from base to platform
    R: Quaternion defining rotation from base to platform
    p_k: Vector (pure quaternion) defining location of servo base wrt base origin
    b_k: Vector (pure quaternion) defining location of servo platform wrt platform origin
*/

void CalcLegLength(vec3 result, vec3 T, vec3 xyz_angles, vec3 p_k, vec3 b_k) {
    float rot_mat[3][3];
    CreateRotMat(rot_mat, xyz_angles[0], xyz_angles[1], xyz_angles[2]);

    float rot_p_k_vec[3];
    MatMult(rot_p_k_vec, rot_mat, p_k);

    for (int i=0; i<3; i++) {
        result[i] = rot_p_k_vec[i] + T[i] - b_k[i];
    }
}

// void CalcLegLength(quat r, quat T, quat R, quat p_k, quat b_k) {
//     quat temp;
//     PrintQuaternion(r, "result");
//     PrintQuaternion(T, "T");
//     PrintQuaternion(R, "R");
//     PrintQuaternion(p_k, "p_k");
//     PrintQuaternion(b_k, "b_k");
//     // Inverse of a quaternion is its conjugate
//     quat R_inv; 
//     quat_conj(R_inv, R);
//     PrintQuaternion(R_inv, "R_inv");

//     // R X p_k
//     QuatMult(temp, R, p_k);
//     PrintQuaternion(r, "R X p_k");

//     quat temp2;

//     // (R X p_k) X R_inv
//     QuatMult(temp2, temp, R_inv);
//     PrintQuaternion(r, "(R X p_k) X R_inv");

//     // T + [(R X p_k) X R_inv]
//     quat_add(r, T, temp2);
//     PrintQuaternion(r, "T + [(R X p_k) X R_inv]");
//     quat ans;
//     // T + [(R X p_k) X R_inv] - b_k
//     quat_sub(ans, r, b_k);
//     PrintQuaternion(ans, "T + [(R X p_k) X R_inv] - b_k");

//     for (int i=0; i<4; i++) {
//       r[i] = ans[i];
//     }
// }

// Calculates servo rotation angle (alpha_k) in RADIANS
float CalcAlpha(quat l_k, float B_k) {
    // Calculate e_k = 2*|h|*l_k[z]
    float e_k = 2*h*l_k[2];
    Serial.print("e_k: ");
    Serial.println(e_k);

    float f_k = 2*h*(cos(B_k)*l_k[0] + sin(B_k)*l_k[1]);
    Serial.print("f_k: ");
    Serial.println(f_k);

    // g_k = |l_k|^2 - (|d|^2 - |h|^2)
    float l_norm = CalcL2Norm(l_k);
    Serial.print("Norm l_k: ");
    Serial.println(l_norm);
    float g_k = l_norm*l_norm - (d*d - h*h);
    Serial.print("g_k: ");
    Serial.println(g_k);

    // Calculate and return alpha_k
    Serial.print("Sqrt term: ");
    Serial.println(sqrt(e_k*e_k + f_k*f_k));
    Serial.print("atan2 term: ");
    Serial.println(atan2(f_k, e_k));
    return asin(g_k/(sqrt(e_k*e_k + f_k*f_k))) - atan2(f_k, e_k);
}

#endif //INV_KIN_HPP

