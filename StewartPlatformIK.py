 checimport numpy as np
import random


def q_mult(quaternion1, quaternion0):
    w0, x0, y0, z0 = quaternion0
    w1, x1, y1, z1 = quaternion1
    return np.array([-x1 * x0 - y1 * y0 - z1 * z0 + w1 * w0,
                     x1 * w0 + y1 * z0 - z1 * y0 + w1 * x0,
                     -x1 * z0 + y1 * w0 + z1 * x0 + w1 * y0,
                     x1 * y0 - y1 * x0 + z1 * w0 + w1 * z0], dtype=np.float64)

# Platform params (all dims in cm)
Ob = np.array([0, 0, 0])
Op = np.array([0, 0, 10])
T_current = Op-Ob
R_current = np.array([1 0 0])

#must have z-value of 0
b_k = np.array([5,0,0])
p_k = np.array([8,0,0])

d_ln = 18
h_ln = 3

                     
l_k = T + q_mult(q_mult(R, p),R_inv)-b_k
# h = h_ln*h*[np.cos(alpha_k)*np.cos(beta_k),
#             np.cos(alpha_k)*np.sin(beta_k),
#             np.sin(alpha_k)]

#Finding anchor         
H_k = B_k+h

#Finding e, f and g
#l_k = [x, y, z]

e_k = 2*h_ln*l_k[2]
f_k = 2*h_ln*(np.cos(beta_k)*l_k[0]+np.sin(beta_k)*l_k[1])
g_k = norm(l_k)**2 - (d_ln**2 - h_ln**2)

#Find servo angle
alpha_k = np.arcsin(g_k/np.sqrt(e_k**2+f_k**2)) - np.arctan2(f_k, e_k)