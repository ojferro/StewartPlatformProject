# import the necessary packages
from datetime import datetime
import numpy as np
from imagezmq import imagezmq
import argparse
import imutils
import cv2
 
# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
args = vars(ap.parse_args())

# initialize the ImageHub object
imageHub = imagezmq.ImageHub()
 
# initialize the dictionary which will contain  information regarding
# when a device was last active, then store the last time the check
# was made was now
lastActive = {}
lastActiveCheck = datetime.now()
 
# stores the estimated number of Pis, active checking period, and
# calculates the duration seconds to wait before making a check to
# see if a device was active
ESTIMATED_NUM_PIS = 1
ACTIVE_CHECK_PERIOD = 10
ACTIVE_CHECK_SECONDS = ESTIMATED_NUM_PIS * ACTIVE_CHECK_PERIOD

num_received_frames = 0

# start looping over all the frames
while True:
	# receive RPi name and frame from the RPi and acknowledge
	# the receipt
	print("Num received frames = {}".format(num_received_frames))
	(rpiName, frame) = imageHub.recv_image()
	imageHub.send_reply(b'OK')
	num_received_frames+=1
 
	# if a device is not in the last active dictionary then it means
	# that its a newly connected device
	if rpiName not in lastActive.keys():
		print("[INFO] receiving data from {}...".format(rpiName))
 
	# record the last active time for the device from which we just
	# received a frame
	lastActive[rpiName] = datetime.now()


	# resize the frame to have a maximum width of 400 pixels, then
	# grab the frame dimensions and construct a blob
	frame = imutils.resize(frame, width=400)

	cv2.imshow("Video Stream", frame)
 
	# detect any kepresses
	key = cv2.waitKey(1) & 0xFF
