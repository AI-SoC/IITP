# Dynamic programming optimization program for linear approximation of ECG signals

## Program developer

Seungmin Lee, Daejin Park 

## Feature

직선근사화의 오차를 최적화하기 위한 동적계획법은 신호의 길이 및 정점의 개수에 따라 기하급수적인 연산량의 증가와 메모리를 요구한다. 본 프로그램에서는 심전도 신호의 특징에 기반하여, 재귀연산 없이 실시간 처리가 가능한 수준으로 동적계획법을 최적화한다.

##   Function

기존 직선근사화에 사용된 동적계획법의 최적화

데이터 타입 및 비트수에 따른 제약을 통한 연산영역 최소화

연산속도 향상으로, 곡률기반 직선근사화 및 순차적 직선근사화 과정 생략

## How to use

1. MATLAB 혹은 MATLAB 라이브러리가 설치된 PC환경에서 test_QT_DB_Run.m 파일을 시행
2. UI 창에서 DB 불러오기
      \- https://physionet.org/ 에서 제공하는 QT-DB 중, sel30 데이터 예제 첨부
      \- 동봉된 R-peak정보 및 결과오차 분석용 기준점 정보 로드
3. Fun_Annotation_Read.m 함수를 이용하여 R-peak정보를 분석하여 심박 정보 자동 획득
4. 각 심박의 R-peak를 기준으로 RR구간을 분리하고 각 구간별 직선근사화 및 기준점 검출이 시행된 후, 결과창 출력
      \- 세로선은 사전에 정의된 기준점의 위치이며, 좌측 및 우측 방향 삼각형은 검출된 기준점을 의미함

##  OS

 Windows10

## Language

MATLAB

## Required Program

MATLAB

## Size

 355 KB