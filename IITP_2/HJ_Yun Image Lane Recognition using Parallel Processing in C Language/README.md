# Image Lane Recognition using Parallel Processing in C Language

## Program Developer

Heuijee Yun, Daejin Park



## Feature

임베디드 보드내에서 저장된 이미지를 여러 과정의 전처리를 거쳐 canny edge detection과 hough transform을 병렬로 C 언어로 작성된 차선 인식 프로그램을 사용해 실행.



## Function

임베디드 보드에 저장된 여러 이미지들을 입력으로 받아 흑백 처리를 해준뒤 thread를 이용한 병렬 처리로 gaussian smoothing, sobel operator, non-maxima suppression 과 hysteresis 작업을 해줌. 이 결과를 통합해 hough transform으로 차선을 검출



## How to Use

 리눅스 기반의 임베디드 보드에 이미지를 저장한 뒤 임베디드 보드에서 C언어 파일을 실행.



## OS

Linux



## Language

C



## Required Program

GCC Compiler



## Size

29KB