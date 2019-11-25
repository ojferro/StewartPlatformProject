import serial_arduino
import ik_pi
import time
import serial
import random
import numpy as np
import Tracker as trk

def main():
    port = serial_arduino.make_serial_connection()
    min_ang = -45
    max_ang = 90
    current_rpy = [0,0,0]
    
    print("Calling Init CV")
    trk.init_cv()
    #Simply output frames until bbox is selected
    while not trk.tracker_enabled:
        trk.get_cv_error()


    while True:
        # Get Error From CV
        error_x, error_y = trk.get_cv_error()
        # Get Joint Angles From IK
        new_roll, new_pitch, new_yaw = trk.error_to_angles(error_x, error_y)
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
        #serial_arduino.wait_and_read_from_serial(port, 10)

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
