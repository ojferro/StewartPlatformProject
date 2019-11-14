# import the necessary packages
from imutils.video import VideoStream
import imutils.video
from imagezmq import imagezmq
import argparse
import socket
import time
#import cv2
 
# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-s", "--server-ip", required=True,
	help="ip address of the server to which the client will connect")
args = vars(ap.parse_args())
 
# initialize the ImageSender object with the socket address of the
# server
sender = imagezmq.ImageSender(connect_to="tcp://{}:5555".format(
	args["server_ip"]))

# get the host name, initialize the video stream, and allow the
# camera sensor to warmup
rpiName = socket.gethostname()
vs = VideoStream(usePiCamera=True).start()
#vs = VideoStream(src=0).start()
time.sleep(2.0)

num_processed_frames = 0
 
while True:
	# read the frame from the camera and send it to the server
	print("reading frame {}".format(num_processed_frames))
	frame = vs.read()
	num_processed_frames+=1
	# cv2.imshow("Video Stream", frame)
	# cv2.waitKey(10)
	sender.send_image(rpiName, frame)
