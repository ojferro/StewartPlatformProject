import numpy as np
import random
import math
from pyquaternion import Quaternion

#Rod length
d_ln = 190
#Servo horn length
h_ln = 16.52
neutral_T=np.array([0, 0, 202])


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


def inv_kin(p_k, b_k, beta_k, translation, roll, pitch, yaw):

    q_params = get_quat(roll, pitch, yaw)
    qt = Quaternion(q_params).normalised

    l_k = (neutral_T+translation)+qt.rotate(p_k) - b_k
    # print("l_k={}".format(l_k))
    l_k_len = np.linalg.norm(l_k)

    e_k = 2*h_ln*l_k[2]
    f_k = 2*h_ln*((math.cos(beta_k)*l_k[0])+(math.sin(beta_k)*l_k[1]))
    g_k = (l_k_len**2 - ((d_ln)**2 - (h_ln)**2))


    alpha_k_rad = math.asin(g_k/math.sqrt((e_k)**2 + (f_k)**2)) - math.atan2(f_k, e_k)
    alpha_k_deg = math.degrees(alpha_k_rad)

    return alpha_k_deg


#Position of joints on base relative to Ob
#must have z-value of 0
b_k_list = np.array([
    [-30.0, -86.75, 0],
    [-90.13, 17.4, 0], 
    [-60.13, 69.36, 0],
    [60.13, 69.36, 0],
    [90.13, 17.4, 0],
    [30, -86.75, 0]
    ])
#Position of joints on platform relative to Op
#must have z-value of 0
p_k_list = np.array([
    [-44.68, -51.78, 0],
    [-67.18, -12.8, 0],
    [-22.5, 64.58, 0],
    [22.5, 64.58, 0],
    [67.18, -12.8, 0],
    [44.68, -51.78, 0]
    ])
beta_k_list = np.array([
    0,
    240,
    240,
    120,
    120,
    0
    ])
beta_k_list*(math.pi/180.0)

translation=np.array([0,0,0])
roll, pitch, yaw = (0.25, 0, 0)


alphas = np.array([])
for b_k, p_k, beta_k in zip(b_k_list, p_k_list, beta_k_list):
    alphas = np.append(alphas, np.array([inv_kin(p_k, b_k, beta_k, translation, roll, pitch, yaw)]))

print(alphas)
