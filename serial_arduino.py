#!/usr/bin/python3

import serial
import struct
import time


def format_packet(angles_f):
	angles_i = [round(i) for i in angles_f]
	print("{}".format(angles_i))

	packet = struct.pack('<hhhhhh', angles_i[0], angles_i[1], angles_i[2], 
									angles_i[3], angles_i[4], angles_i[5])
	packet = b'[' + packet + b']'
	print("{}".format(packet))
	return packet

def send_angles_to_arduino(pkt):
	ser = serial.Serial('/dev/ttyACM0', 9600)
	time.sleep(2) # Time required since arduino will reset
	ser.flush()
	ser.write(pkt)
	time.sleep(2) # Buffer time to write to Serial
	while(1):
		print(ser.readline())


angles = [-11.6, -21.1, -13.44, -44.5, -52.6, -16.0]
pkt = format_packet(angles)
send_angles_to_arduino(pkt)

