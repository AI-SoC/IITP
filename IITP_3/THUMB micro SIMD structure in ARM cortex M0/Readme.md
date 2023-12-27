# THUMB micro SIMD structure in ARM cortex M0

## Program Developer

윤희지, 박대진



## Feature

SIMD가 구현되지 않은 ARM cortex m0에서 데이터의 벡터화가 필요하지 않은 32bit 데이터 패킹을 통한 병렬 처리 연산이 가능함

## Function

Register에 저장된 32bit 데이터를 입력된 명령어에 따라 n bit로 분할한 뒤 병렬로 연산함.



## How to Use

ARM core에 연결하여 SIMD 명령어를 입력함.

## OS

  Linux



## Language

Verilog



## Required Program

NC-verilog



## Sized

2 KB
