#!/usr/bin/python3

import numpy as np
import random
import math
from pyquaternion import Quaternion

## Parameter Initialization

# Rod length
d_ln = 193.0
# Servo horn length
h_ln = 24
# Translation offset vector in neutral state
neutral_T=np.array([0, 0, 190.6])

# Angles specifying servo location on the top-down view of base
top_view_angles_base = np.array([
    242.0119595,
    297.9880405,
    2.011959459,
    57.98804054,
    122.0119595,
    177.9880405
])
top_view_angles_base = top_view_angles_base*(math.pi/180.0) # Convert to radians

# Angles specifying servo location on the top-down view of platform
top_view_angles_platform = np.array([
    230.0694965,
    309.9305035,
    350.0694965,
    69.93050353,
    110.0694965,
    189.9305035
])
top_view_angles_platform = top_view_angles_platform*(math.pi/180.0) # Convert to radians

b_k_list = np.empty([0,3])
b_k_norm = 73.83537431 # Length of vector from base origin to a base servo

p_k_list = np.empty([0,3])
p_k_norm = 50.99666656 # Lenght of vector from platform origin to a platform servo joint

for i, (top_angle_base, top_angle_platform) in enumerate(zip(top_view_angles_base, top_view_angles_platform)):
    # Compute vector to joints from base and platform origin respectively
    b_k_list = np.append(b_k_list, [[b_k_norm*math.cos(top_angle_base), b_k_norm*math.sin(top_angle_base), 0]], axis=0)
    p_k_list = np.append(p_k_list, [[p_k_norm*math.cos(top_angle_platform), p_k_norm*math.sin(top_angle_platform), 0]], axis=0)

print("b_k_list: {}".format(b_k_list))
print("p_k_list: {}".format(p_k_list))

# Angles of servo horn w.r.t. horizontnal about z axis
beta_k_list = np.array([
    0,
    0,
    120,
    120,
    240,
    240
    ])
beta_k_list = beta_k_list*(math.pi/180.0) # Convert to radians


# Compute quaternion given roll, pitch, yaw angles
def get_quat(roll, pitch, yaw):
    sin_roll = math.sin(roll*0.5)
    sin_pitch = math.sin(pitch*0.5)
    sin_yaw = math.sin(yaw*0.5)
    cos_roll = math.cos(roll*0.5)
    cos_pitch = math.cos(pitch*0.5)
    cos_yaw = math.cos(yaw*0.5)

    w = cos_yaw*cos_pitch*cos_roll + sin_yaw*sin_pitch*sin_roll
    x = cos_yaw*cos_pitch*sin_roll - sin_yaw*sin_pitch*cos_roll
    y = sin_yaw*cos_pitch*sin_roll + cos_yaw*sin_pitch*cos_roll
    z = sin_yaw*cos_pitch*cos_roll - cos_yaw*sin_pitch*sin_roll

    return np.array([w,x,y,z])

# Compute servo angles given desired orientation (in degrees)
def inv_kin(rotation, translation):
    roll, pitch, yaw = rotation
    # Convert input parameters to radians
    roll = roll*math.pi/180
    pitch = pitch*math.pi/180
    yaw = yaw*math.pi/180

    servo_angles = []

    q_params = get_quat(roll, pitch, yaw)
    qt = Quaternion(q_params).normalised
    print("qt_ours={}".format(qt))
    
    for i, (p_k, b_k, beta_k) in enumerate(zip(p_k_list, b_k_list, beta_k_list)):
        # print("qt.rotate(p_k)={}".format(qt.rotate(p_k)))

        # print("qt.rotate(p_k)={}, b_k={}".format(qt.rotate(p_k), b_k))
        l_k = (neutral_T+translation) + qt.rotate(p_k) - b_k
        l_k_len = np.linalg.norm(l_k)
        print("l_k={}, length={}".format(l_k, l_k_len))

        print("h_ln={}, beta_k={}".format(h_ln, beta_k))

        e_k = 2*h_ln*l_k[2]
        f_k = 2*h_ln*((math.cos(beta_k)*l_k[0])+(math.sin(beta_k)*l_k[1]))
        g_k = (l_k_len**2 - ((d_ln)**2 - (h_ln)**2))

        print("e_k={}, f_k={}, g_k={}".format(e_k, f_k, g_k))

        alpha_k_rad = math.asin(g_k/math.sqrt((e_k)**2 + (f_k)**2)) - math.atan2(f_k, e_k)
        alpha_k_deg = math.degrees(alpha_k_rad)
        servo_angles.append(alpha_k_deg)

    return servo_angles

print("\n\nCalculated Servo angles: {}".format(inv_kin(10,10,10, [0,0,0])))