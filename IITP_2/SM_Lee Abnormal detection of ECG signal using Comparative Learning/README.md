# Abnormal detection of ECG signal using Comparative Learning

## Program Developer

Seungmin Lee, Daejin Park



## Feature

템플릿 군을 이용하여 대표정상심박의 템플릿을 획득하고, 이를 입력된 심박과 결합하여 학습을 진행한다. 결합된 두 심박간의 유사도를 비교학습하여, 유사할 경우 정상, 유사하지 않을 경우 비정상으로 분류함으로써, 다양한 개인에 대해 하나의 단일 비교학습 신경망으로 정상심박과 비정상심박의 분류가 가능하게 된다.



## Function

Pearson 유사도를 이용하여 심박 간의 유사도 측정

정상 템플릿 군을 형성한 후, 대표 정상심박 정보 획득

획득된 대표정상심박과 입력된 심박을 결합하여 영상화 데이터 형성

결합된 영상화 데이터를 학습하여 비교학습 인공신경망 형성

단일 비교학습 인공신경망을 통해 다양한 개인에 대해 정상 및 비정상심박 분류를 진행



## How to Use

Fun_Main.m 파일을 시행

사전에 학습된 비교학습 인공신경망을 적용하여 심박 분류
 \- https://physionet.org/ 에서 제공하는 MIT-BIH ADB 중, 100m, 101m, 119m 데이터 예제 첨부
 \- 동봉된 R-peak정보 및 유사도 임계값 정보 로드

사전에 학습된 인공신경망을 이용하여 분류 결과를 출력



## OS

Windows



## Language

MATLAB



## Required Program

MATLAB



## Size

3,318KB