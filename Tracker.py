#Works with python 3.7.2
#Dependencies:
#   opencv-python
#   opencv-contrib-python

import cv2
import time
from matplotlib import pyplot as plt

def min_to_ms(min):
  return int(min*60*1000)

tracker = cv2.TrackerCSRT_create()
tracker_enabled = False
bbox = ()

# Read video
#video = cv2.VideoCapture("dashcam_video.mp4")
video = cv2.VideoCapture(0)
CV_CAP_PROP_BUFFERSIZE = 21
video.set(CV_CAP_PROP_BUFFERSIZE, 1)

# Exit if video not opened.
if not video.isOpened() or not video.read()[0]:
  print ("Could not open video")

video.set(0, min_to_ms(5.0))
video.set(3, video.read()[1].shape[0]*2)
video.set(4, video.read()[1].shape[1]*2)

# Read first frame.
ok, frame = video.read()
if not ok:
  print ('Cannot read video file')

NUM_FRAME_MODULUS = 5
num_frames_input = 0

p1 = (0, 0)
p2 = (0,0)

TARGET_FPS = 30
TARGET_TIME_PER_FRAME = 1000/TARGET_FPS

while True:
  # Read a new frame
  ok, frame = video.read()
  if not ok:
      break
  
  # Start timer
  timer = cv2.getTickCount()

  key = cv2.waitKey(2)
  
  scale_percent = 60 # percent of original size
  width = int(frame.shape[1] * scale_percent / 100)
  height = int(frame.shape[0] * scale_percent / 100)
  dim = (width, height)
 
  # resize image
  frame = cv2.resize(frame, dim, interpolation = cv2.INTER_AREA)

  # Update tracker
  if tracker_enabled and (num_frames_input%NUM_FRAME_MODULUS == 0):
    ok, bbox = tracker.update(frame)

        # Draw bounding box
    if ok:
        # Tracking success
        p1 = (int(bbox[0]), int(bbox[1]))
        p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
        cv2.rectangle(frame, p1, p2, (255,0,0), 4, 1)
    else :
        # Tracking failure
        cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)

    # Display tracker type on frame
    # cv2.putText(frame," Tracker", (100,20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50),2);

    # Display FPS on frame
    cv2.putText(frame, "FPS : " + str(int(fps)), (10,50), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (50,170,50), 2)
    
  elif not tracker_enabled and key==115: #if s key pressed, select roi
    print("HOORAY")
    # Select a bounding box
    # bbox = (200, 100, 100, 150) #hard coded for now b/c jupyter
    bbox = cv2.selectROI("frame", frame, True)
    print(frame.shape)
    print(bbox)

    # Initialize tracker with first frame and bounding box
    ok = tracker.init(frame, bbox)

    tracker_enabled = True
  else:
    cv2.rectangle(frame, p1, p2, (255,0,0), 4, 1)

  # Calculate Frames per second (FPS)
  fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer)
  if (fps > TARGET_FPS):
      sleep_time = (TARGET_TIME_PER_FRAME - (1000/fps))
      time.sleep(sleep_time)

  num_frames_input += 1

  # Display result
  cv2.imshow("frame", frame)
  cv2.waitKey(10)

  # Exit if ESC pressed
  k = cv2.waitKey(1) & 0xff
  if k == 27 : break
