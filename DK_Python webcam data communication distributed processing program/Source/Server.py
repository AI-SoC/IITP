import socket 
import cv2
import numpy
import re
from queue import Queue
from _thread import *
from time import sleep

#프레임 색
white = (255,255,255)

enclosure_queue = Queue()
img_queue = Queue()
track_queue = Queue()

tr_check = 0
tr_num = 0

def display(im_queue,tr_queue):
    
    global tr_check
    global tr_num
    
    while True:
        
        #이미지 디코딩 
        stringData = im_queue.get()
        myimg = numpy.frombuffer(stringData, dtype='uint8') 
        decimg=cv2.imdecode(myimg,1)
        
        #업데이트 되었을 때, 새로운 트래킹값 얻음
        if tr_check > 0 :
            point_str = tr_queue.get()
            window = re.findall("\d+",point_str)
            tr_check = 0
            
        #트래킹 값 이미지에 그림
        for i in range(1,tr_num+1):
            print(window)
            decimg=cv2.rectangle(decimg,(int(window[0]),int(window[1])),(int(window[2]),int(window[3])),white,3)

        #이미지 출력
        cv2.imshow('Image',decimg)
        key = cv2.waitKey(1)
        if key == 27:
            break


# 처리 함수
def threaded(client_socket, addr, queue, tr_queue): 

    global tr_check
    global tr_num
    
    # 클라이언트와의 동기화를 위한 큐 누락 갯수
    omission = 0;
    
    print('Connected by :', addr[0], ':', addr[1]) 
    
    while True: 

        try:
            #클라이언트 체크용 메세지 수신
            data = client_socket.recv(1024)

            if not data: 
                print('Disconnected by ' + addr[0],':',addr[1])
                break

            #이미지 전송
            stringData = queue.get()
            client_socket.send(str(len(stringData)).ljust(16).encode())
            client_socket.send(stringData)
                   
            #추적 개수 수신
            num_string = client_socket.recv(1024).decode('utf-8')
            tr_num = int(num_string)
            tr_check = 1
            
            #트래킹 데이터를 display로 전송
            for i in range(1,tr_num+1):
                point_str = client_socket.recv(1024).decode('utf-8')
                tr_queue.put(point_str)
                
            #동기화를 위한 큐 누락
            for i in range(1,omission+1):
                stringData = queue.get()    
            
        except ConnectionResetError as e:

            print('Disconnected by ' + addr[0],':',addr[1])
            break
             
    client_socket.close() 

#웹캠 함수
def webcam(queue,im_queue):

    capture = cv2.VideoCapture(0)

    while True:
        ret, frame = capture.read()

        if ret == False:
            continue

        encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
        result, imgencode = cv2.imencode('.jpg', frame, encode_param)

        data = numpy.array(imgencode)
        stringData = data.tostring()

        queue.put(stringData)
        im_queue.put(stringData)
        
        sleep(0.2);

#서버 IP + 포트
HOST = '127.0.0.1'
PORT = 9999

#소켓 통신 설정
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((HOST, PORT)) 
server_socket.listen() 

print('server start')

#웹캠 쓰레드 시작
start_new_thread(webcam, (enclosure_queue,img_queue,))


while True: 

    print('wait')

    client_socket, addr = server_socket.accept() 
    start_new_thread(threaded, (client_socket, addr, enclosure_queue,track_queue,))
    start_new_thread(display, (img_queue,track_queue,))

server_socket.close() 