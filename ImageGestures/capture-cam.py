#!/usr/bin/env python
# OpenCV's Python demo
# -- adapted by Minh-Tri Pham to work with pyopencv

import sys

# import the necessary things for OpenCV
from pyopencv import *

#############################################################################
# so, here is the main part of the program

if __name__ == '__main__':

    # a small welcome
    print("OpenCV Python capture video")

    # first, create the necessary window
    namedWindow('Camera', CV_WINDOW_AUTOSIZE)

    # move the new window to a better place
    moveWindow('Camera', 10, 10)

    try:
        # try to get the device number from the command line
        device = int(sys.argv [1])

        # got it ! so remove it from the arguments
        del sys.argv [1]
    except (IndexError, ValueError):
        # no device number on the command line, assume we want the 1st device
        device = 0

    if len (sys.argv) == 1:
        # no argument on the command line, try to use the camera
        capture = VideoCapture(device)
    else:
        # we have an argument on the command line,
        # we can assume this is a file name, so open it
        capture = VideoCapture(sys.argv [1])            

    # check that capture device is OK
    if not capture.isOpened():
        print("Error opening capture device")
        sys.exit (1)

    # create a Mat instance
    frame = Mat()

    # capture the 1st frame to get some propertie on it
    capture >> frame

    # get size of the frame
    frame_size = frame.size()

    # get the frame rate of the capture device
    fps = capture.get(CV_CAP_PROP_FPS)
    if fps == 0:
        # no fps getted, so set it to 30 by default
        fps = 30

    # create the writer

    # check the writer is OK
    #if not writer.isOpened():
        #print("Error opening writer")
        #sys.exit (1)
        
    while True:
        # do forever

        # 1. capture the current image
        capture >> frame
        if frame.empty():
            # no image captured... end the processing
            break

        # write the frame to the output file
        #writer << frame
        min_colour = (110, 75, 75)
        max_colour = (125,255,255)
        
        hsv_img = Mat(frame.rows,frame.cols, CV_8UC1)#Size(frame),8,1);#CV_8UC1)
        threshold_img = Mat(frame.rows,frame.cols, CV_8UC1)#Size(frame),8,1);#CV_8UC1)

        
        cvtColor(frame, hsv_img, CV_BGR2HSV);
        
        inRange(hsv_img, Scalar(*min_colour), Scalar(*max_colour), threshold_img)
        lastX = 0.0
        lastY = 0.0
        contour_img = threshold_img
        i=0
        maxArea = 0
        biggestContour = vector_Point2i()
        contours, heirachy = findContours (contour_img, RETR_LIST, CHAIN_APPROX_NONE)
        for contour in contours:
          #print contour         
          #biggestContour = contour
          #if(isContourConvex(asMat(contour))):
          if contourArea(asMat(contour))>maxArea:
            maxArea = contourArea(asMat(contour))
            biggestContour = contour
        if(asMat(biggestContour).isContinuous()):
          bbox = boundingRect(asMat(biggestContour));
          x, y = bbox.x+bbox.width*0.5, bbox.y+bbox.height*0.5
          print x,y
          circle(frame, Point2i(int(x), int(y)), 10, Scalar(0,255,255), 5);
          lastX, lastY = x, y
        #img_hsv = Mat(
        #print Scalar(*max_colour)
        #InRangeS(frame, cvScalar
        
        # display the frames to have a visual output
        imshow('Camera', frame)
        imshow('Contour', contour_img)
        imshow('Mask', threshold_img)
        #print hsv_img[150][300]
        # handle events
        if waitKey (5) & 255 == 27:
            # user has press the ESC key, so exit
            break
