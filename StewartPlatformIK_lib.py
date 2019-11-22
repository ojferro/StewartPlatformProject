import numpy as np

def conjugate(R):
    R_inv = np.array(R[:-1])*-1
    return np.append(R_inv, R[-1])

def findLegLength(r, T, R, p_k, b_k):
    R_inv = conjugate(R)
    