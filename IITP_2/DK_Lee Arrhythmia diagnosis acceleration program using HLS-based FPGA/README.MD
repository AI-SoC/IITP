#Arrhythmia diagnosis acceleration program using HLS-based FPGA

## Program Developer

Dongkyu Lee, Daejin Park



##Feature

심전도 신호에서 비정상 심박은 많은 정상 심박 사이에 드물게 나타난다. 사람에 따라 다르게 나타나는 많은 정상 신호 사이에서 비정상 신호를 찾는 것은 많은 연산을 필요로 한다. 본 프로그램은 개인에 최적화된 정상 심박 템플릿을 결정하고, Alveo U200 FPGA를 이용하여 입력된 심박 데이터의 정상/비정상을 가속한다. FPGA에는 입력된 개수만큼의 하드웨어가 적재되고, 심박 데이터의 정상/비정상을 병렬 연산하게 되어 데이터 처리 속도가 증가한다.



## Function

심박 간 유사도를 측정하여 정상/비정상 템플릿 형성

템플릿 갱신 회수를 측정하여 개인 별 대표 정상 템플릿군 형성

C++ 언어로 작성된 정상/비정상 판단 하드웨어를 HLS로 합성 및 적재

Alveo U200 FPGA를 기반으로 소프트웨어가 실행될 때 하드웨어가 자동 적재 및 실행

정상/비정상 판별 기준 학습, 판단 하드웨어 합



## How to Use

Linux commandline에서 “./run_me.sh hw” 명령어 입력

스크립트에서 아래 과정이 실행됨

  \- ./exe.sh : 정상 심박 학습 및 판단 프로그램 컴파일

  \- ./build.sh hw : 정상 심박 판단 가속 하드웨어 합성

  \- ./run.sh hw ../data/DB.csv ../data/R_peak.csv : 데이터베이스의 심박 정상/비정상 판별

각 심박의 정상/비정상을 Normal(N)/Ventricular arrhythmia(V)로 출력



## OS

Linux



## Language

 C++



## Required Program

 C++, Vitis compiler, Vivado HLS, OpenCL



## Sized

 2.57 MB