#!/usr/bin/python3

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
  #print(bbox_buffer)
  return (int(np.average(np.array(bbox_buffer)[:,0])), int(np.average(np.array(bbox_buffer)[:,1])))

# tracker = cv2.TrackerCSRT_create()
tracker = cv2.TrackerKCF_create()
tracker_enabled = False
bbox = ()
bbox_buffer_ln = 5 # num bboxes to be averaged
bbox_buffer=[]
cnt=0

# Read video
video = cv2.VideoCapture(0)#("dashcam_video.mp4")

def init_cv():
  # Exit if video not opened.
  if not video.isOpened() or not video.read()[0]:
    print ("Could not open video")

  ok, frame = video.read()

  video.set(0, min_to_ms(5.0))
  video.set(3, video.read()[1].shape[0]*1)
  video.set(4, video.read()[1].shape[1]*1)

  # Read first frame.
  ok, frame = video.read()
  if not ok:
    print ('Cannot read video file')

def get_cv_error():
  global cnt
  global tracker_enabled
  global bbox_buffer
  global fps


  cnt +=1

  # Read a new frame
  ok, frame = video.read()
  frame = cv2.flip(frame,0)  

  if not ok:
      return False
  
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
        cv2.circle(frame,centre,3,(0,0,255),1)
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
  
  return get_centres_avg(bbox_buffer) if tracker_enabled else -1

  # Exit if ESC pressed
  # if key == 27 : break

# 8 = 240 pix diff / 30 deg yaws
def error_to_angles(err_x, err_y):
    k_p_x = 0.06
    diff_err_x = (err_x - 240) if abs(err_x - 240) > 35 else 0
    yaw = (diff_err_x/8) * k_p_x # Proportional Gain for Yaw
    
    k_p_y = 0.05
    diff_err_y = (err_y - 320) if abs(err_y - 320) > 35 else 0
    cam_rot = (diff_err_y/22.0) * k_p_y 
    pitch = cam_rot * np.cos(np.pi / 3.) * -1
    roll = cam_rot * np.sin(np.pi / 3.)
    
    return [roll,pitch,yaw]
