import cv2
import time
import numpy as np
import matplotlib
matplotlib.use("TkAgg")
from matplotlib import pyplot as plt
from keras.models import load_model
from PIL import Image 
from darkflow.net.build import TFNet
import time
import psutil
import os

cap = cv2.VideoCapture(0)

print("start time:")
print(time.time())

#-------------Memory usage-----------
def memory_usage(message: str = 'debug'):
    # current process RAM usage
    p = psutil.Process()
    rss = p.memory_info().rss / 2 ** 20 # Bytes to MB
    print(f"[{message}] memory usage: {rss: 10.5f} MB")
#-------------------------------------

memory_usage('#1')


print("YOLO start")
#-------------YOLO Options-----------
options = {
    #'model': 'cfg/tiny-yolo.cfg',
    #'load': 'bin/tiny-yolo.weights',
    'model': 'cfg/yolo.cfg',
    'load': 'bin/yolov2.weights',
    'threshold': 0.4,
    #'gpu': 1.0
}

tfnet = TFNet(options)
colors = [tuple(255 * np.random.rand(3)) for _ in range(10)]

print("seg start")
#------------Segmentation options-------------
model_path = "seg_enet/enet-cityscapes/enet256_weight0501.hdf5"
model = load_model(model_path)
img_dec_wh = 256
num_classes = 7

memory_usage('#2')

#------------Segmentation-------------
def realtime_demo(img_wh):
    predict_times = []

    while True:
        # Capture frame-by-frame
        ret, orig_img = cap.read()
        bgr_img = cv2.resize(orig_img, (img_wh, img_wh))
        img = bgr_img[..., ::-1]
        img = img * (1/255.0)

        # Add batch dimension: 1 x D x D x 3
        img_tensor = np.expand_dims(img, 0)

        start = time.time()
        raw_output = model.predict(img_tensor)
        predict_times.append(time.time() - start)
        print('Average prediction time:', np.mean(predict_times))

        output = np.reshape(raw_output,(1, img_dec_wh, img_dec_wh, num_classes))

        # Get mask
        seg_labels = output[0, :, :, :]
        seg_img = np.argmax(seg_labels, axis=2)
        rcnnMask = np.zeros((256,256),dtype='uint8')    
        area = 0
        area_back=0
        sum_back = 0 
        for i in range(0,255):
            for j in range (0, 255):
                if seg_img[i][j]!=0:
                    rcnnMask[i][j] = 5
                    area += 1
                else :
                    sum_back += img[i][j]
                    area_back +=1

        mean = sum_back / area_back # mean of background rgb
        vsum = 0 

        for i in range(0,255):
            for j in range (0, 255):
                vsum = vsum + (img[i][j] - mean)**2

        variance = vsum / area_back
        

        img_dis = np.array(bgr_img)
        masked = cv2.bitwise_and(img_dis,img_dis, mask = rcnnMask)

        #------------YOLO-------------
        
        stime = time.time()
        results = tfnet.return_predict(masked)
        obj_count = 0
        for color, result in zip(colors, results):
            tl = (result['topleft']['x'], result['topleft']['y'])
            br = (result['bottomright']['x'], result['bottomright']['y'])
            label = result['label']
            confidence = result['confidence']
            text = '{}: {:.0f}%'.format(label, confidence * 100)
            masked = cv2.rectangle(masked, tl, br, color, 3)
            masked = cv2.putText(
                masked, text, tl, cv2.FONT_HERSHEY_COMPLEX, 0.25, (255, 255, 255), 1)
            obj_count += 1
        #cv2.imshow('frame', cv2.resize(masked,(512, 512)))
        cv2.imshow('frame', masked)
        print("people: ", obj_count)
        print('YOLO time:', time.time() - stime)
        print('FPS {:.1f}'.format(1 / (time.time() - stime)))
        memory_usage('#3')
        
        #print("area: ", (area/65536)*100)
        #print("mean", mean)
        #print("variance of background", variance)

        # Display
        #display_img = cv2.resize(bgr_img, (512, 512))
        #cv2.imshow("result",cv2.resize(masked,(512, 512)))
        #cv2.imshow('img', display_img)



        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()

realtime_demo(256)





