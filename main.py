import serial_arduino
import StewartPlatformIK_pyquaterion
import time
import serial
import random

def main():
    port = serial_arduino.make_serial_connection()
    min_ang = -45
    max_ang = 90

    while(1):
        # Get Error From CV
        # Get Joint Angles From IK

        # Random angles for testing
        angles = [(min_ang + (random.random() * (max_ang - min_ang))) for i in range(6)]
        print(angles)

        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)
        serial_arduino.wait_and_read_from_serial(port, 10)

        print("Done Iteration")


if __name__ == "__main__":
    main()
