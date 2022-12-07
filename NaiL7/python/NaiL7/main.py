import cv2 as cv
import time

capture = cv.VideoCapture('xd.mp4')
capturing=True;
while capturing:

    ret, frame=capture.read()
    start = time.time()
    if ret:
        frame_flip=cv.flip(frame,1)
        frame_gray=cv.cvtColor(frame_flip, cv.COLOR_BGR2GRAY)
        ret, thresh=cv.threshold(frame_gray, 150, 255, cv.THRESH_BINARY)
        contours, hierarchy = cv.findContours(image=thresh, mode=cv.RETR_TREE, method=cv.CHAIN_APPROX_NONE)
        v_copy = frame_flip.copy()
        cv.drawContours(image=v_copy, contours=contours, contourIdx=-1, color=(0, 255, 0), thickness=2,
                         lineType=cv.LINE_AA)

        all_areas = []
        for cnt in contours:
            area = cv.contourArea(cnt)
            all_areas.append(area)
        sorted_contours = sorted(contours, key=cv.contourArea, reverse=True)
        largest_item = sorted_contours[0]
        end = time.time()
        print(round((end - start)*1000.0)/1000.0)

        cv.imshow('Video', v_copy)


        if cv.waitKey(20) & 0xFF==ord(chr(27).encode()):
            break
    else:
        break


capture.release()
cv.destroyAllWindows()
