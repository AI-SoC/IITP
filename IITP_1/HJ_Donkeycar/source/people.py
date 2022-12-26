import os
import tensorflow as tf
import tensornets as nets
import cv2
import numpy as np
import time
import tensorflow.compat.v1 as tf

tf.disable_v2_behavior() 

inputs = tf.placeholder(tf.float32, [None, 416, 416, 3]) 
model = nets.YOLOv3COCO(inputs, nets.Darknet19)

classes={'0':'person'}
list_of_classes=[0]
with tf.Session() as sess:
    sess.run(model.pretrained())
    
    cap = cv2.VideoCapture("C:\\Users\\samsung\\Desktop\\cut.mp4")
    while(cap.isOpened()):
        ret, frame = cap.read()
        img=cv2.resize(frame,(416,416))
        imge=np.array(img).reshape(-1,416,416,3)
        start_time=time.time()
        preds = sess.run(model.preds, {inputs: model.preprocess(imge)})

        print("--- %s seconds ---" % (time.time() - start_time)) 
        boxes = model.get_boxes(preds, imge.shape[1:3])
        cv2.namedWindow('image',cv2.WINDOW_NORMAL)

        cv2.resizeWindow('image', 700,700)
        
        boxes1=np.array(boxes)
        for j in list_of_classes: 
            count =0
            if str(j) in classes:
                lab=classes[str(j)]
            if len(boxes1) !=0:
                for i in range(len(boxes1[j])): 
                    box=boxes1[j][i] 
                    if boxes1[j][i][4]>=.40: 
                        count += 1    
                        cv2.rectangle(img,(box[0],box[1]),(box[2],box[3]),(0,255,0),3)
                        cv2.putText(img, lab, (box[0],box[1]), cv2.FONT_HERSHEY_PLAIN, 0.8, (0, 0, 255), lineType=cv2.LINE_AA)
            print(lab,": ",count)
    
        cv2.imshow("image",img)
        if count>0  print os.system("donkey calibrate --channel 1 --bus=1")

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
