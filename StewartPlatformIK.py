import numpy as np
import random


def q_mult(quaternion1, quaternion0):
    w0, x0, y0, z0 = quaternion0
    w1, x1, y1, z1 = quaternion1
    return np.array([-x1 * x0 - y1 * y0 - z1 * z0 + w1 * w0,
                     x1 * w0 + y1 * z0 - z1 * y0 + w1 * x0,
                     -x1 * z0 + y1 * w0 + z1 * x0 + w1 * y0,
                     x1 * y0 - y1 * x0 + z1 * w0 + w1 * z0], dtype=np.float64)

def norm_vec3(vec):
    return np.sqrt(vec[0]**2 + vec[1]**2 + vec[2]**2)

##########################################################################
######################## PLATFORM PARAMETERS (in cm) #####################
##########################################################################

#Origin of base and platform
Ob = np.array([0, 0, 0])
Op = np.array([0, 0, 10])

#Total relative translation vector from base to platform
T_current = Op-Ob
#Rotation pure quaternion (i.e. axis of rotation) from base to platform
R = np.array([1 0 0])

#Position of joints on base relative to Ob
#must have z-value of 0
b_k_list = np.array([
    [8,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0]
    ])
#Position of joints on platform relative to Op
#must have z-value of 0
p_k_list = np.array([
    [5,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0],
    [0,0,0]
    ])

#Angles of servo horn w.r.t. horizontnal about z axis
beta_k_list = np.array([
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
    ])

#Rod length
d_ln = 18
#Servo horn length
h_ln = 3

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
