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
        angles = ik_pi.inv_kin(roll, pitch, yaw)
        # Random angles for testing
        # angles = [(min_ang + (random.random() * (max_ang - min_ang))) for i in range(6)]
        print(angles)

        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)
        serial_arduino.wait_and_read_from_serial(port, 10)

        print("Done Iteration")


if __name__ == "__main__":
    main()
