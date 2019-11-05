#!/usr/bin/python3

import serial
import struct
import time
import calendar

def make_serial_connection():
    port = serial.Serial('/dev/ttyACM0', 9600)
    time.sleep(2)  # Time required since arduino will reset
    return port

def format_packet(angles_f):
    angles_i = [round(i) for i in angles_f]
    print("{}".format(angles_i))

    packet = struct.pack('<hhhhhh', angles_i[0], angles_i[1], angles_i[2],
                         angles_i[3], angles_i[4], angles_i[5])
    packet = b'[' + packet + b']'
    return packet


def send_angles_to_arduino(port, pkt):
    port.flush()
    port.write(pkt)
    time.sleep(2)  # Buffer time to write to Serial

def wait_and_read_from_serial(port, seconds):
    start = calendar.timegm(time.gmtime())
    print("Incoming Serial Data: \n")
    while ((calendar.timegm(time.gmtime()) - start) < seconds):
        if (port.in_waiting):
            print(str(port.readline()))

### Test Code ###

def open_arduino_serial():
    port = serial.Serial('/dev/ttyACM0', 9600, timeout = 0)
    time.sleep(2)  # Time required since arduino will reset
    return port


def test_main():
    angles = [-11.6, -21.1, -13.44, -44.5, -52.6, -16.0]
    port = open_arduino_serial()
    pkt = format_packet(port, angles)
    send_angles_to_arduino(pkt)
    wait_and_read_from_serial(port, 10)
    port.close()
