# OpenCL-based ECG Signal Diagnosis GPU Accleration Program

## Program Developer

Dongkyu Lee, Daejin Park



##Feature

많은 양의 심전도 신호에서 비정상 심박을 찾는 것은 많은 연산을 필요로 한다. 본 프로그램은 개인에 최적화된 정상 심박 템플릿을 결정하고, OpenCL을 기반으로 GPU를 이용하여 입력된 심박 데이터의 정상/비정상 판단을 가속한다. 프로그램은 파라미터에 따라 GPU에서 가속될 데이터의 크기를 변경할 수 있다. 또한, Makefile을 이용하여 리눅스 뿐만 아니라 윈도우에서도 하나의 명령어로 컴파일 및 실행을 할 수 있도록 한다.



## Function

심박 간 유사도를 측정하여 정상/비정상 템플릿 형성

템플릿 갱신 회수를 측정하여 개인별 대표 정상 템플릿군 형성

OpenCL 커널을 기반으로 GPU에서 정상/비정상 판단

정상/비정상 판별 기준 학습, GPU 커널 적재 및 분류 소프트웨어 실행을 하나의 스크립트로 실행

Windows와 Linux에서 모두 컴파일 및 실행할 수 있는 스크립트



## How to Use

Window 또는 Linux commandline에서 “make all” 명령어 입력

스크립트에서 OpenCL의 경로를 찾아 운영체제에 맞는 실행 파일 생성

각 심박의 정상/비정상을 Normal(N)/Ventricular arrhythmia(V)로 출력



## OS

Linux, Windows



## Language

C++



## Required Program

 C++, OpenCL



## Sized

 2.56 MB