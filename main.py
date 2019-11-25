import serial_arduino
import ik_pi
import time
import serial
import random
import Tracker as trk

def main():
    port = serial_arduino.make_serial_connection()
    min_ang = -45
    max_ang = 90
    
    trk.init_cv()
    #Simply output frames until bbox is selected
    while not trk.tracker_enabled:
        trk.get_cv_error()


    while True:
        # Get Error From CV
        error_x, error_y = trk.get_cv_error()
        # Get Joint Angles From IK
        roll, pitch, yaw = trk.error_to_angles(error_x, error_y)
        angles = ik_pi.inv_kin([roll, pitch, yaw], [0,0,0])
        # Random angles for testing
        # angles = [(min_ang + (random.random() * (max_ang - min_ang))) for i in range(6)]
        print(angles)

        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)
        serial_arduino.wait_and_read_from_serial(port, 10)

        print("Done Iteration")

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
