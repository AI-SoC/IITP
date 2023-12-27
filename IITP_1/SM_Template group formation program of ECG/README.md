# Template group formation program of ECG

## Program developer

Seungmin Lee, Daejin Park 

## Feature

심전도 신호의 형태는 개인에 따라 다양하다. 정상 심박은 주기적으로 유사한 형태로 반복되므로 이에 대한 템플릿을 결정하는 것은 차후 신호처리에 매우 유용하다. 본 프로그램에서는 정상심박의 템플릿을 결정한다. 가중평균을 이용하여 비정상심박에 의한 왜곡을 최소화하고, 메모리 사용량을 최소화하여 경량화 및 실시간 처리가 가능하다.

##   Function

Pearson 유사도를 이용하여 심박 간의 유사도 측정

템플릿 군을 형성하여 단일 템플릿이 아닌 다수의 템플릿 형성

가중평균을 이용하여 템플릿 갱신을 경량화 

갱신회수가 가장 높은 템플릿을 대표 정상심박으로 결정

대표정상심박과의 유사도를 측정하여 정상템플릿 군 형성

## How to use

1. Fun_Main.m 파일을 시행
2. UI 창에서 DB 불러오기
      \- https://physionet.org/ 에서 제공하는 MIT-BIH ADB 중, 100m 데이터 예제 첨부
      \- 동봉된 R-peak정보 및 유사도 임계값 정보 로드
3. 초기 입력 심박, 갱신된 템플릿 군, 정상 템플릿 군을 출력

##  OS

 Windows10

## Language

MATLAB

## Required Program

MATLAB

## Size

 506 KB