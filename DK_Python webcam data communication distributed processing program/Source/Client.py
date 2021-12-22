import socket 
import numpy as np
import cv2
from time import sleep 

import random ################

def recvall(sock, count):
    buf = b''
    while count:
        newbuf = sock.recv(count)
        if not newbuf: return None
        buf += newbuf
        count -= len(newbuf)
    return buf

#서버 IP + 포트
HOST = '127.0.0.1'
PORT = 9999

client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM) 

client_socket.connect((HOST, PORT)) 


while True: 
    
    #연결 확인 메세지
    message = '1'
    client_socket.send(message.encode()) 
  
    #웹캠 이미지 수신
    length = recvall(client_socket,16)
    stringData = recvall(client_socket, int(length))
    data = np.frombuffer(stringData, dtype='uint8') 
    decimg=cv2.imdecode(data,1) #웹캠 이미지
    
    ## 추적 개수 전송
    num_track = 2 # 추적 개수
    client_socket.send(str(num_track).encode()) 
    sleep(0.01)
    
    ##추적한 위치 전송
    for i in range(1,num_track+1):
        p1 = random.randrange(1,400) #사각형 좌표 1의 X
        p2 = random.randrange(1,400) #사각형 좌표 1의 Y
        p3 = p1+50 #사각형 좌표 2의 X
        p4 = p2+50 #사각형 좌표 2의 Y

        point_str = str(p1) + ' ' + str(p2) + ' ' + str(p3) + ' ' + str(p4)

        client_socket.send(point_str.encode())  
        
        sleep(0.01)

    sleep(1)
        
client_socket.close() 