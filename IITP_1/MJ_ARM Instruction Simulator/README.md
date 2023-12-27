## Arm instruction set simulator

Myeongjin Kang, Daejin Park 

## Feature

 ARM 프로세서에서 머신 코드 명령어를 처리하는 방법을 C로 재현하여 자체 디버깅을 통해 레지스터와 메모리를 확인할 수 있다.

##   Function

 크로스 컴파일을 통해 생성한 ARM 명령어를 분석하여 ARM core의 동작을 재현하여 명령어 수행중 메모리와 레지스터 값을 확인할 수 있고, 사용자 명령어를 추가하여 시뮬레이션이 가능하다.

## How to use

크로스 컴파일을 통해 생성된 binary파일을 생성후, change.c파일을 실행시켜 2진수와 16진수 파일로 저장후 main을 실행한다.

##  OS

 Windows10

## Language

 C

## Required Program

   ARM Crosscompiler, Make, GCC

## Size

 122 KB