#Works with python 3.7.2
#Dependencies:
#   opencv-python
#   opencv-contrib-python

import cv2
from matplotlib import pyplot as plt
import numpy as np

def min_to_ms(min):
  return int(min*60*1000)

def get_bbox_centre(bbox):
  return (int(bbox[0] + bbox[2]/2), int(bbox[1] + bbox[3]/2))

def get_centres_avg(bbox_buffer):
  return (int(np.average(np.array(bbox_buffer)[:,0])), int(np.average(np.array(bbox_buffer)[:,1])))

# tracker = cv2.TrackerCSRT_create()
tracker = cv2.TrackerKCF_create()
tracker_enabled = False
bbox = ()
bbox_buffer_ln = 5 # num bboxes to be 
bbox_buffer=[]

# Read video
video = cv2.VideoCapture("dashcam_video.mp4")

# Exit if video not opened.
if not video.isOpened() or not video.read()[0]:
  print ("Could not open video")

ok, frame = video.read()
# print("!!!!!!!!!!!!!! frame size {}x{}".format(frame.shape[0], frame.shape[1]))
# cv2.waitKey(500)

video.set(0, min_to_ms(5.0))
video.set(3, video.read()[1].shape[0]*1)
video.set(4, video.read()[1].shape[1]*1)

# Read first frame.
ok, frame = video.read()
if not ok:
  print ('Cannot read video file')

cnt=0
while True:
  cnt+=1
  # Read a new frame
  ok, frame = video.read()
  if not ok:
      break
  
  # Start timer
  timer = cv2.getTickCount()

  key = cv2.waitKey(1)

  # Update tracker
  if tracker_enabled:
    ok, bbox = tracker.update(frame)

    # Draw bounding box
    if ok:
        # Tracking success
        bbox_buffer[cnt%bbox_buffer_ln]=get_bbox_centre(bbox)
        centre = get_centres_avg(bbox_buffer)
        
        p1 = (int(bbox[0]), int(bbox[1]))
        p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
        cv2.rectangle(frame, p1, p2, (255,0,0), 3, 1)
        cv2.circle(frame,centre,3,(0,0,255),1) #Drawing circle is apparently super expensive, USE FOR DEBUGGING ONLY!
    else:
        # Tracking failure
        cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)

    # Display FPS on frame
    cv2.putText(frame, "FPS : " + str(int(fps)), (10,50), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (50,170,50), 2)
    
  elif key==115: #if s key pressed, select roi
    # Select a bounding box
    # bbox = (200, 100, 100, 150) #hard coded for now b/c jupyter
    bbox = cv2.selectROI("frame", frame, True)
    # print(frame.shape)
    # print(bbox)

    # Initialize tracker with first frame and bounding box
    ok = tracker.init(frame, bbox)
    bbox_buffer=[get_bbox_centre(bbox)]*bbox_buffer_ln

    tracker_enabled = True

  # Calculate Frames per second (FPS)
  fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer)

  # Display result
  cv2.imshow("frame", frame)
  cv2.waitKey(10)

  # Exit if ESC pressed
  if key == 27 : break