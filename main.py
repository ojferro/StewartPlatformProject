import serial_arduino
import StewartPlatformIK_pyquaterion
import time
import serial

def main():
    port = serial_arduino.make_serial_connection()

    while(1):
        # Get Error From CV
        # Get Joint Angles From IK

        angles = [-11.6, -21.1, -13.44, -44.5, -52.6, -16.0]

        packet = serial_arduino.format_packet(angles)
        serial_arduino.send_angles_to_arduino(port, packet)
        serial_arduino.wait_and_read_from_serial(port, 5)

        print("Done Iteration")


if __name__ == "__main__":
    main()
