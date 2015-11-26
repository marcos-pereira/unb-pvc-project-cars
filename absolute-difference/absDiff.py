# import the necessary packages
import argparse
import datetime
import imutils
import time
import cv2

# # construct the argument parser and parse the arguments
# ap = argparse.ArgumentParser()
# ap.add_argument("-v", "--video", help="path to the video file")
# ap.add_argument("-a", "--min-area", type=int, default=500, help="minimum area size")
# args = vars(ap.parse_args())
#
# # if the video argument is None, then we are reading from webcam
# if args.get("video", None) is None:
#     camera = cv2.VideoCapture("Bsb-EixoRodoviarioSul.mov")
#     time.sleep(0.25)
#
# # otherwise, we are reading from a video file
# else:
#     camera = cv2.VideoCapture()

# initialize the first frame in the video stream
firstFrame = None
firstFrame = cv2.imread("firstFrame2.png",1)
firstFrame = imutils.resize(firstFrame, width=500)
firstFrame = cv2.cvtColor(firstFrame, cv2.COLOR_BGR2GRAY)
firstFrame = cv2.GaussianBlur(firstFrame, (21, 21), 0)

string = ['cctv052x2004080516x01638.avi',
          'cctv052x2004080516x01639.avi',
          'cctv052x2004080516x01640.avi',
          'cctv052x2004080516x01641.avi',
          'cctv052x2004080516x01642.avi',
          'cctv052x2004080516x01643.avi',
          'cctv052x2004080516x01644.avi',
          'cctv052x2004080516x01645.avi',
          'cctv052x2004080516x01646.avi',
          'cctv052x2004080516x01647.avi',
          'cctv052x2004080516x01648.avi',
          'cctv052x2004080516x01649.avi',
          'cctv052x2004080516x01650.avi',
          'cctv052x2004080517x01652.avi',
          'cctv052x2004080517x01653.avi',
          'cctv052x2004080517x01654.avi',
          'cctv052x2004080517x01655.avi',
          'cctv052x2004080517x01656.avi',
          'cctv052x2004080517x01657.avi',
          'cctv052x2004080517x01658.avi', ]

for i in range(0, 20):
    camera = cv2.VideoCapture(string[i])
    time.sleep(0.25)

    # loop over the frames of the video
    while True:
        # grab the current frame and initialize the occupied/unoccupied
        # text
        (grabbed, frame) = camera.read()
        # text = "Unoccupied"

        # if the frame could not be grabbed, then we have reached the end
        # of the video
        if not grabbed:
            break

        # resize the frame, convert it to grayscale, and blur it
        frame = imutils.resize(frame, width=500)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (21, 21), 0)

        # if the first frame is None, initialize it
        if firstFrame is None:
            firstFrame = gray
            continue

        # compute the absolute difference between the current frame and
        # first frame
        frameDelta = cv2.absdiff(firstFrame, gray)
        thresh = cv2.threshold(frameDelta, 40, 255, cv2.THRESH_BINARY)[1]

        # dilate the thresholded image to fill in holes, then find contours
        # on thresholded image
        thresh = cv2.dilate(thresh, None, iterations=2)
        (cnts, _) = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
                                     cv2.CHAIN_APPROX_SIMPLE)

        # loop over the contours
        for c in cnts:
            # if the contour is too small, ignore it
            if cv2.contourArea(c) < 100:
                continue
            if cv2.contourArea(c) > 3000:
                continue
                # compute the bounding box for the contour, draw it on the frame,
                # and update the text
            (x, y, w, h) = cv2.boundingRect(c)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            # text = "Occupied"

            # draw the text and timestamp on the frame
            # cv2.putText(frame, "Room Status: {}".format(text), (10, 20),
            # cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
            # cv2.putText(frame, datetime.datetime.now().strftime("%A %d %B %Y %I:%M:%S%p"),
            # (10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)

        # show the frame and record if the user presses a key
        cv2.imshow("Security Feed", frame)
        cv2.imshow("Thresh", thresh)
        cv2.imshow("Frame Delta", frameDelta)
        key = cv2.waitKey(1) & 0xFF

        # if the `q` key is pressed, break from the lop
        if key == ord("q"):
            break

# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
