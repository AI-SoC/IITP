import numpy as np
import cv2
from matplotlib import pyplot as plt
from PIL import Image

net = cv2.dnn.readNet("yolov3.weights", "cfg/yolov3.cfg")
classes = []
with open("data/coco.names", "r") as f:
    classes = [line.strip() for line in f.readlines()]
layer_names = net.getLayerNames()
output_layers = [layer_names[i[0] - 1] for i in net.getUnconnectedOutLayers()]
colors = np.random.uniform(0, 255, size=(len(classes), 3))


face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
#eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')

img = cv2.imread('face.jpg')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

faces = face_cascade.detectMultiScale(gray, 1.3, 5)


for (x,y,w,h) in faces:
    cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
    #roi_gray = gray[y:y+h, x:x+w]
    #roi_color = img[y:y+h, x:x+w]
    #eyes = eye_cascade.detectMultiScale(roi_gray)
    #for (ex,ey,ew,eh) in eyes:
        #cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
    img_crop = img[y-10: y+10+h, x-10: x+10+w]

    blob = cv2.dnn.blobFromImage(img_crop, 0.00392, (416, 416), (0, 0, 0), True, crop=False)
    net.setInput(blob)
    outs = net.forward(output_layers)

    class_ids = []
    confidences = []
    boxes = []

    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]
            if confidence > 0.5:
                center_x = int(detection[0] * w)
                center_y = int(detection[1] * h)
                w1 = int(detection[2] * w)
                h1 = int(detection[3] * h)
                x1 = int(center_x - w1 / 2)
                y1 = int(center_y - h1 / 2)
                boxes.append([x1, y1, w1, h1])
                confidences.append(float(confidence))
                class_ids.append(class_id)

                indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)

                font = cv2.FONT_HERSHEY_PLAIN
    for i in range(len(boxes)):
        if i in indexes:
            x1, y1, w1, h1 = boxes[i]
            label = str(classes[class_ids[i]])
            color = colors[i]
            cv2.rectangle(img_crop, (x1, y1), (x1 + w1, y1 + h1), color, 2)
            cv2.putText(img_crop, label, (x1, y1 + 30), font, 3, color, 3)



#cv2.imshow('img',img)
cv2.imshow('img_crop',img_crop)
cv2.waitKey(0)
cv2.destroyAllWindows()
