# Webcam image object recognition using socket communication

## Program developer

Heuijee Yun, Daejin Park 

## Feature

PC에 연결된 웹캠 이미지를 소켓통신을 이용해 경량화된 임베디드 보드로 전송후 경량화 임베디드 보드에서 객체 인식을 실행하여 지연시간을 감소함.

##   Function

Host PC에 연결된 웹캠 영상을 전처리해 소켓통신으로 임베디드 보드에 전송하면 임베디드 보드에서 YOLO를 사용해 객체 인식을 실행함. 객체 인식 결과를 Host PC로 전송해 Host PC에서 결과를 출력함.

## How to use

Windows 기반의 호스트 PC에 웹캠을 연결 후 리눅스 기반의 임베디드 보드의 주소로 소켓 연결, 호스트 PC와 임베디드 보드에서 파이썬 파일을 실행.

##  OS

 Windows10, Linux

## Language

 Python

## Required Program

  Python

## Size

 10 KB