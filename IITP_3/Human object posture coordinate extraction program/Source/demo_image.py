import os
import cv2
import argparse
import csv
import json

# x,y 좌표 파싱 CSV
f = open('coordinate.csv','w',newline='')
wr=csv.writer(f)

# x,y 좌표 파싱 json
F_group = dict()

parser = argparse.ArgumentParser()
parser.add_argument('--number', default="1", type=str, help='Input file number.')

args = parser.parse_args()

# MPII에서 각 파트 번호, 선으로 연결될 POSE_PAIRS
# BODY_PARTS = { "Head": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
#                 "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
#                 "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "Chest": 14,
#                 "Background": 15 }
#
# POSE_PAIRS = [ ["Head", "Neck"], ["Neck", "RShoulder"], ["RShoulder", "RElbow"],
#                 ["RElbow", "RWrist"], ["Neck", "LShoulder"], ["LShoulder", "LElbow"],
#                 ["LElbow", "LWrist"], ["Neck", "Chest"], ["Chest", "RHip"], ["RHip", "RKnee"],
#                 ["RKnee", "RAnkle"], ["Chest", "LHip"], ["LHip", "LKnee"], ["LKnee", "LAnkle"] ]

BODY_PARTS = { "Nose": 0, "Neck": 1, "RShoulder": 2, "RElbow": 3, "RWrist": 4,
               "LShoulder": 5, "LElbow": 6, "LWrist": 7, "RHip": 8, "RKnee": 9,
               "RAnkle": 10, "LHip": 11, "LKnee": 12, "LAnkle": 13, "REye": 14,
               "LEye": 15, "REar": 16, "LEar": 17, "Background": 18 }

BODY_PARTS1 = [ "Nose", "Neck", "RShoulder", "RElbow", "RWrist",
               "LShoulder", "LElbow", "LWrist", "RHip", "RKnee",
               "RAnkle", "LHip", "LKnee", "LAnkle", "REye",
               "LEye", "REar", "LEar", "Background" ]

POSE_PAIRS = [ ["Neck", "RShoulder"], ["Neck", "LShoulder"], ["RShoulder", "RElbow"],
               ["RElbow", "RWrist"], ["LShoulder", "LElbow"], ["LElbow", "LWrist"],
               ["Neck", "RHip"], ["RHip", "RKnee"], ["RKnee", "RAnkle"], ["Neck", "LHip"],
               ["LHip", "LKnee"], ["LKnee", "LAnkle"], ["Neck", "Nose"], ["Nose", "REye"],
               ["REye", "REar"], ["Nose", "LEye"], ["LEye", "LEar"] ]
# 각 파일 path
protoFile = os.getcwd()+"\\model\\pose_deploy_linevec_faster_4_stages.prototxt"
weightsFile = os.getcwd()+"\\model\\pose_iter_160000.caffemodel"
 
# 위의 path에 있는 network 불러오기
# net = cv2.dnn.readNetFromCaffe(protoFile, weightsFile)
net = cv2.dnn.readNetFromTensorflow(os.getcwd()+"\\model\\graph_opt.pb")

# 이미지 읽어오기
image = cv2.imread(os.getcwd()+"\\data\\pose_image_"+args.number+".jpg")

# frame.shape = 불러온 이미지에서 height, width, color 받아옴
imageHeight = image.shape[0]
imageWidth = image.shape[1]
print("프레임 너비: %d, 프레임 높이: %d" %(imageWidth, imageHeight))

frameWidth = 368
frameHeight = 368

# network에 넣기위해 전처리
#inpBlob = cv2.dnn.blobFromImage(image, 1.0 / 255, (frameWidth, frameHeight), (0, 0, 0), swapRB=False, crop=False)
inpBlob = cv2.dnn.blobFromImage(image, 1.0, (frameWidth, frameHeight), (127.5, 127.5, 127.5), swapRB=True, crop=False)

# network에 넣어주기
net.setInput(inpBlob)

# 결과 받아오기
output = net.forward()

# output.shape[0] = 이미지 ID, [1] = 출력 맵의 높이, [2] = 너비
H = output.shape[2]
W = output.shape[3]
# print("이미지 ID : ", len(output[0]), ", H : ", output.shape[2], ", W : ",output.shape[3]) # 이미지 ID

# 키포인트 검출시 이미지에 그려줌
points = []
for i in range(len(BODY_PARTS)-1):    #(0, 15):
    # 해당 신체부위 신뢰도 얻음.
    probMap = output[0, i, :, :]

    # global 최대값 찾기
    minVal, prob, minLoc, point = cv2.minMaxLoc(probMap)

    # 원래 이미지에 맞게 점 위치 변경
    x = (imageWidth * point[0]) / W
    y = (imageHeight * point[1]) / H

    print("Frane = ","Body_parts = ", " x = ",int(x)," y= ",int(y))
    wr.writerow([BODY_PARTS1[i],i,int(x),int(y)])
    Frame=dict()
    Frame["X"]=int(x)
    Frame["Y"]=int(y)
    Frame["Z"]=0
    F_group["Frame"]=Frame

    # 키포인트 검출한 결과가 0.1보다 크면(검출한곳이 위 BODY_PARTS랑 맞는 부위면) points에 추가, 검출했는데 부위가 없으면 None으로
    if prob > 0.1 :
        cv2.circle(image, (int(x), int(y)), 3, (0, 255, 255), thickness=-1, lineType=cv2.FILLED)       # circle(그릴곳, 원의 중심, 반지름, 색)
        cv2.putText(image, "{}".format(i), (int(x), int(y)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1, lineType=cv2.LINE_AA)
        points.append((int(x), int(y)))
    else :
        points.append(None)

cv2.imshow("Output-Keypoints", image)

# 이미지 복사
imageCopy = image

# 각 POSE_PAIRS별로 선 그어줌 (머리 - 목, 목 - 왼쪽어깨, ...)
for pair in POSE_PAIRS:
    partA = pair[0]             # Head
    partA = BODY_PARTS[partA]   # 0
    partB = pair[1]             # Neck
    partB = BODY_PARTS[partB]   # 1

    #print(partA," 와 ", partB, " 연결\n")
    if points[partA] and points[partB]:
        cv2.line(imageCopy, points[partA], points[partB], (0, 255, 0), 2)


cv2.imshow("Output-Keypoints-draw", imageCopy)
cv2.waitKey(0)
# 작업 완료 후 해제
with open("test.json", 'w', encoding='utf-8') as make_file:
    json.dump(F_group, make_file, indent="\t")

f.close()
cv2.destroyAllWindows()
