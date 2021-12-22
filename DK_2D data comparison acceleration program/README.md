#2D data comparison acceleration program

## Program developer

Dongyku Jung, Daejin Park 

## Feature

  GPU 코어가 내장된 임베디드 시스템에서 탑재된 GPU 코어를 사용하여 2차원 행렬형태로 존재하는 두 데이터 파일 비교 프로세스를 가속하여 데이터 비교의 수행 시간 감소

## Function

  행렬 형태의 데이터를 행 기준으로 분할하고 각 행 데이터들의 개수만큼 병렬화된 프로세스를 생성하여 비교 연산을 수행한 후, 최종적으로 비교 결과를 가지는 행들을 통합하여 결과를 출력함

## How to use

 Linux 환경에서 GCC를 이용해 컴파일하여 실행파일(main)을 생성한 뒤, 셀 명령어로 프로그램 수행

## OS

 Linux

## Language

 C

## Required Program

 GCC

## Size

 14 KB