# Template group-based abnormal heart rate detection program for ECG signals

## Program developer

Seungmin Lee, Daejin Park 

## Feature

템플릿 군을 이용하여 정상심박의 템플릿 군을 획득함으로써, 정상 템플릿 군과의 유사도를 측정하여 정상심박과 비정상심박을 분류한다.

##   Function

Pearson 유사도를 이용하여 심박 간의 유사도 측정

정상 템플릿 군을 형성한 후, 템플릿 군 내부의 각 템플릿과의 유사도 측정

유사한 템플릿이 존재하지 않을 경우, 비정상심박으로 분류

사전에 저장된 심박 분류 정보와 비교하여, 검출률 측정 및 오검출 분석

## How to use

1. Fun_Main.m 파일을 시행
2. UI 창에서 DB 불러오기
       \- https://physionet.org/ 에서 제공하는 MIT-BIH ADB 중, 100m 데이터 예제 첨부
       \- 동봉된 R-peak정보 및 유사도 임계값 정보 로드
3. 심박 분류 정보와 비교하여 검출률 출력
4. 정상/비정상 분류 결과 출력
5. 분류 별 오검출 심박 출력을 통한 결과 분석

##  OS

 Windows10

## Language

MATLAB

## Required Program

MATLAB

## Size

 508 KB