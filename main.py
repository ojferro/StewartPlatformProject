import serial_arduino
import ik_pi
import time
import serial
import random
import numpy as np
# import Tracker as trk
import ipc_pi as ipc
import zmq

def error_to_angles(err_x, err_y):
    k_p_x = 0.06
    diff_err_x = (err_x - 240) if abs(err_x - 240) > 35 else 0
    yaw = (diff_err_x/8) * k_p_x # Proportional Gain for Yaw
    
    k_p_y = 0.05
    diff_err_y = (err_y - 320) if abs(err_y - 320) > 35 else 0
    cam_rot = (diff_err_y/22.0) * k_p_y 
    pitch = cam_rot * np.cos(np.pi / 3.) * -1
    roll = cam_rot * np.sin(np.pi / 3.)
    
    return [roll,pitch,yaw]

def main():
    port = serial_arduino.make_serial_connection()

    socket = ipc.create_socket(zmq.SUB)
    print("Initialized ZMQ socket")
    
    min_ang = -45
    max_ang = 90
    current_rpy = [0,0,0]
    
    while True:
        # Get Error From CV
        error_x, error_y = ipc.recv_error(socket)
        # Get Joint Angles From IK
        new_roll, new_pitch, new_yaw = error_to_angles(error_x, error_y)
        new_current_roll = current_rpy[0]+new_roll
        new_current_pitch = current_rpy[1]+new_pitch
        new_current_yaw = current_rpy[2]+new_yaw

        current_rpy[0] = new_current_roll if abs(new_current_roll) < 15 else (15*np.sign(new_current_roll))
        current_rpy[1] = new_current_pitch if abs(new_current_pitch) < 15 else (15*np.sign(new_current_pitch))
        current_rpy[2] = new_current_yaw if abs(new_current_yaw) < 30 else (30*np.sign(new_current_yaw))

        print("ErrX: {}, ErrY: {} ----- New Yaw: {}, Sent Yaw: {}, New Roll: {}, Sent Roll: {}, New Pitch: {}, Sent Pitch: {}".format(error_x-240, error_y-320, new_yaw, current_rpy[2], new_roll, current_rpy[0], new_pitch, current_rpy[1]))
        angles = ik_pi.inv_kin([current_rpy[0],
                                current_rpy[1],
                                current_rpy[2]], [0,0,0])
        # Random angles for testing
        # angles = [(min_ang + (random.random() * (max_ang - min_ang))) for i in range(6)]
        print(angles)

        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)

        print("!!!SENT PACKET!!!")

def test_main():
    port = serial_arduino.make_serial_connection()

    for i in range(15):
        angles = ik_pi.inv_kin([0, 0, 2*i], [0,0,0])
        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)

    for i in range(15,0,-1):
        angles = ik_pi.inv_kin([0, 0, 2*i], [0,0,0])
        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)
    
    for i in range(15):
        angles = ik_pi.inv_kin([0, 0, 2*i], [0,0,0])
        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)
    
    for i in range(15,0,-1):
        angles = ik_pi.inv_kin([0, 0, 2*i], [0,0,0])
        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)   

if __name__ == "__main__":
    main()
