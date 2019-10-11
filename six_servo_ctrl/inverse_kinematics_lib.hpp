#include <Servo.h>
#include "linmath.h"
#include "motor.hpp"
#include "inv_kin.hpp"

#include <Arduino.h>

// Rotation quaternion from base to platform
quat q1 = {3, 8, 4.5, 1.5};
quat q2 = {0, 1, 0, 1};
vec3 v1 = {1, 0, 2};
vec3 v2 = {1, -4, -5};


void GetQuat(quat result, float roll, float pitch, float yaw) {
  float sin_roll = sin(roll*0.5);
  float sin_pitch = sin(pitch*0.5);
  float sin_yaw = sin(yaw*0.5);
  float cos_roll = cos(roll*0.5);
  float cos_pitch = cos(pitch*0.5);
  float cos_yaw = cos(yaw*0.5);

  result[0] = cos_yaw*cos_pitch*sin_roll - sin_yaw*sin_pitch*cos_roll;
  result[1] = sin_yaw*cos_pitch*sin_roll + cos_yaw*sin_pitch*cos_roll;
  result[2] = sin_yaw*cos_pitch*cos_roll - cos_yaw*sin_pitch*sin_roll;
  result[3] = cos_yaw*cos_pitch*cos_roll + sin_yaw*sin_pitch*sin_roll;
}

void GetJointAngles(float roll, float pitch, float yaw) {
  GetQuat(bp_rot, roll, pitch, yaw);
  NormalizeQuaternion(bp_rot);
  float alpha = 0.0;

  for (int i=0; i<num_servos; i++) {
    CalcLegLength(eff_leg_lengths[i], bp_translations[i], bp_rot, servo_pcoords[i], servo_bcoords[i]);
    alpha = CalcAlpha(eff_leg_lengths[i], betas[i]);
    target_angles[i] =  (int)RAD2DEG(alpha);
  }  
}

void NormalizeQuats(quat quats[num_servos]) {
  for (int i=0; i<num_servos; i++) {
    NormalizeQuaternion(quats[i]);
  }
}
}
