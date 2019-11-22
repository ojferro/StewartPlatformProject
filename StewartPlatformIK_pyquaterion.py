import numpy as np
import random
import math
from pyquaternion import Quaternion


# def q_mult(quaternion1, quaternion0):
#     x0, y0, z0, w0 = quaternion0
#     x1, y1, z1, w1 = quaternion1
#     return np.array([x1 * w0 + y1 * z0 - z1 * y0 + w1 * x0,
#                      -x1 * z0 + y1 * w0 + z1 * x0 + w1 * y0,
#                      x1 * y0 - y1 * x0 + z1 * w0 + w1 * z0,
#                      -x1 * x0 - y1 * y0 - z1 * z0 + w1 * w0], dtype=np.float64)

# def q_mult(quaternion0, quaternion1):
#     print(quaternion0)
#     print(quaternion1)
#     print("!!!Q_MULT!!!")
#     x1, y1, z1, w1 = quaternion0
#     print("q1 {}: {},{},{},{}".format(quaternion0, x1,y1,z1,w1))
#     x2, y2, z2, w2 = quaternion1
#     print("q2 {}: {},{},{},{}".format(quaternion1,x2,y2,z2,w2))
#     return np.array([x2 * w1 - y2 * z1 + z2 * y1 + w2 * x1,
#                      x2 * z1 + y2 * w1 - z2 * x1 + w2 * y1,
#                      -x2 * y1 + y2 * x1 + z2 * w1 + w2 * z1,
#                      -x2 * x1 - y2 * y1 - z2 * z1 + w2 * w1], dtype=np.float64)
# -x2 * y1 + y2 * x1 + z2 * w1 + w2 * z1,
# -B[0]*A[1]+B[1]*A[0]+B[2]*A[3]+B[3]*A[2]

# def norm_vec3(vec):
#     return np.sqrt(vec[0]**2 + vec[1]**2 + vec[2]**2)

# def norm_vec4(vec):
#     return np.sqrt(vec[0]**2 + vec[1]**2 + vec[2]**2+vec[3]**2)

# def conjugate(q):
#     q_inv = np.array(q[:-1])*-1
#     q_inv = np.append(q_inv, q[-1])
#     return 

# theta = 0.25 #in rad
# R=np.array([-0.71, -0.71, 0])*math.sin(theta/2)
# R = np.append(R, math.cos(theta/2))
# R = R/norm_vec4(R)
# print("R={}".format(R))
# print("THING!!! {}".format(q_mult([1,2,3,4],[5,6,7,8])))
# R_inv = conjugate(R)
# print("R_inv={}".format(R_inv))

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


def inv_kin():
    #Rod length
    d_ln = 187
    #Servo horn length
    h_ln = 24
    neutral_T=np.array([0, 0, 184.7585408])

    translation=np.array([0,0,0])

    
    # b_k_list = np.array([
    #     [-30.0, -86.75, 0],
    #     [-90.13, 17.4, 0], 
    #     [-60.13, 69.36, 0],
    #     [60.13, 69.36, 0],
    #     [90.13, 17.4, 0],
    #     [30, -86.75, 0]
    #     ])
    # #Position of joints on platform relative to Op
    # #must have z-value of 0
    # p_k_list = np.array([
    #     [-44.68, -51.78, 0],
    #     [-67.18, -12.8, 0],
    #     [-22.5, 64.58, 0],
    #     [22.5, 64.58, 0],
    #     [67.18, -12.8, 0],
    #     [44.68, -51.78, 0]
    #     ])

    # top_view_angles = np.array([
    #     -160.923523178042,
    #     -79.0732170113642,
    #     -40.9228733935925,
    #     40.9228733935925,
    #     79.0732170113642,
    #     160.923523178042
    # ])
    top_view_angles = np.array([
        -160.0,
        -80.0,
        -40.0,
        40.0,
        80.0,
        160.0
    ])
    top_view_angles = top_view_angles*(math.pi/180.0)
    b_k_list = np.empty([0,3])
    p_k_list = np.empty([0,3])
    b_k_norm = 66.66
    p_k_norm = 50.62

    for i, top_view_angle in enumerate(top_view_angles):
        b_k_list = np.append(b_k_list, [[b_k_norm*math.cos(top_view_angle), b_k_norm*math.sin(top_view_angle), 0]], axis=0)
        p_k_list = np.append(p_k_list, [[p_k_norm*math.cos(top_view_angle), p_k_norm*math.sin(top_view_angle), 0]], axis=0)

    print("b_k_list: {}".format(b_k_list))
    print("p_k_list: {}".format(p_k_list))

    # b_k_list = np.array([
    #     [-62.999, -21.786, 0],
    #     [12.636, -65.451, 0],
    #     [50.368, -43.665, 0],
    #     [50.368, 43.665, 0],
    #     [12.636, 65.451, 0],
    #     [-62.999, 21.786, 0]
    #     ])
    # #Position of joints on platform relative to Op
    # #must have z-value of 0
    # p_k_list = np.array([
    #     [-47.840, -16.544, 0],
    #     [9.595, -49.702, 0],
    #     [38.248, -33.158, 0],
    #     [38.248, 33.158, 0],
    #     [9.595, 49.702, 0],
    #     [-47.840, 16.544, 0]
    #     ])

    #Angles of servo horn w.r.t. horizontnal about z axis
    beta_k_list = np.array([
        0,
        240,
        240,
        120,
        120,
        0
        ])
    beta_k_list = beta_k_list*(math.pi/180.0)

    print("beta_k_list={}".format(beta_k_list))
    
    for i, (p_k, b_k, beta_k) in enumerate(zip(p_k_list, b_k_list, beta_k_list)):
        # beta_k = 0
        # p_k=np.array([-44.68, -51.78, 0])
        # b_k=np.array([-30.0, -86.75, 0])
        # print([p_k, b_k, beta_k])

        q_params = get_quat(math.radians(20), 0, 0)
        qt = Quaternion(q_params).normalised
        # print("qt={}, qt.rotate(p_k)={}".format(qt, qt.rotate(p_k)))

        print("qt.rotate(p_k)={}, b_k={}".format(qt.rotate(p_k), b_k))
        l_k = (neutral_T+translation)+qt.rotate(p_k) - b_k
        l_k_len = np.linalg.norm(l_k)
        print("l_k={}, length={}".format(l_k, l_k_len))

        print("h_ln={}, beta_k={}".format(h_ln, beta_k))

        e_k = 2*h_ln*l_k[2]
        f_k = 2*h_ln*((math.cos(beta_k)*l_k[0])+(math.sin(beta_k)*l_k[1]))
        g_k = (l_k_len**2 - ((d_ln)**2 - (h_ln)**2))

        print("e_k={}, f_k={}, g_k={}".format(e_k, f_k, g_k))


        alpha_k_rad = math.asin(g_k/math.sqrt((e_k)**2 + (f_k)**2)) - math.atan2(f_k, e_k)
        alpha_k_deg = math.degrees(alpha_k_rad)
        print("Alpha {} : {}\n".format(i,alpha_k_deg))

    return alpha_k_deg

print(inv_kin())

# R_p_k = q_mult(R, p_k)
# print("Rxp_k: {}".format(R_p_k))
# R_p_k_R_inv = q_mult(R_p_k,R_inv)
# print("R_p_k_R_inv: {}".format(R_p_k_R_inv))
# l_k = T + R_p_k_R_inv - b_k
# print("Leg length x, y, z: {}".format(l_k))
# print(q_mult(q_mult(R, p_k), R_inv))

#Compute length of virtual arm
# l_k = T + q_mult(q_mult(R, p_k),R_inv)-b_k

# #Rod length
# d_ln = 190
# #Servo horn length
# h_ln = 16.52

# beta_k = 0

# e_k = 2*h_ln*l_k[2]
# print("e_k={}".format(e_k))
# f_k = 2*h_ln*(np.cos(beta_k)*l_k[0]+np.sin(beta_k)*l_k[1])
# print("f_k={}".format(f_k))
# g_k = norm_vec3(l_k)**2 - (d_ln**2 - h_ln**2)
# print("g_k={}".format(g_k))

#     #Find servo angle
# alphas = []
# alphas = np.append(alphas, np.arcsin(g_k/np.sqrt(e_k**2+f_k**2)) - np.arctan2(f_k, e_k))
# print("Alphas {}, eff leg length {}".format(alphas, norm_vec3(l_k)))

'''

##########################################################################
######################## PLATFORM PARAMETERS (in cm) #####################
##########################################################################

#Origin of base and platform
Ob = np.array([0, 0, 0])
Op = np.array([0, 0, 10])

#Total relative translation vector from base to platform
T = Op-Ob
#Rotation pure quaternion (i.e. axis of rotation) from base to platform
R = np.array([1 0 0])

#Position of joints on base relative to Ob
#must have z-value of 0
b_k_list = np.array([
    [-30.0, -86.75, 0, 0],
    [-90.13, 17.4, 0, 0], 
    [-60.13, 69.36, 0, 0],
    [60.13, 69.36, 0, 0],
    [90.13, 17.4, 0, 0],
    [30, -86.75, 0, 0]
    ])
#Position of joints on platform relative to Op
#must have z-value of 0
p_k_list = np.array([
    [-44.68, -51.78, 0, 0],
    [-67.18, -12.8, 0, 0],
    [-22.5, 64.58, 0, 0],
    [22.5, 64.58, 0, 0],
    [67.18, -12.8, 0, 0],
    [44.68, -51.78, 0, 0]
    ])

#Angles of servo horn w.r.t. horizontnal about z axis
beta_k_list = np.array([
    0,
    240,
    240,
    120,
    120,
    0
    ])
beta_k_list*(math.pi/180.0)

#Rod length
d_ln = 147.395
#Servo horn length
h_ln = 16.52

##########################################################################
##########################################################################
##########################################################################
alphas = np.array([])
for b_k, p_k, beta_k in zip(b_k_list, p_k_list, beta_k_list):
    #Compute length of virtual arm
    l_k = T + q_mult(q_mult(R, p_k),R_inv)-b_k

    e_k = 2*h_ln*l_k[2]
    f_k = 2*h_ln*(np.cos(beta_k)*l_k[0]+np.sin(beta_k)*l_k[1])
    g_k = norm_vec3(l_k)**2 - (d_ln**2 - h_ln**2)

    #Find servo angle
    alphas = np.append(alphas, np.arcsin(g_k/np.sqrt(e_k**2+f_k**2)) - np.arctan2(f_k, e_k))

print(alphas)
##########################################################################
##########################################################################


#Finding anchor (where rod end meets servo horn)
# h = h_ln*h*[np.cos(alpha_k)*np.cos(beta_k),
#             np.cos(alpha_k)*np.sin(beta_k),
#             np.sin(alpha_k)]
# H_k = B_k+h

#Finding e, f and g
#l_k = [x, y, z]

'''