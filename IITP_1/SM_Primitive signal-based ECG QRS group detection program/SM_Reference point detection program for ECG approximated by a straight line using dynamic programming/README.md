# Reference point detection program for ECG approximated by a straight line using dynamic programming

## Program developer

Seungmin Lee, Daejin Park 

## Feature

복잡한 심전도 신호의 기준점 검출을 소수의 직선으로 근사화하여 표현함으로써, 신호를 단순화하고, 근사화 정점으로 잘 나타나는 QRS군의 기준점이 가지는 특징을 강화하여 신뢰있게 검출할 수 있도록 함

##   Function

곡률기반 직선근사화, 순차적 직선근사화, 그리고 동적계획법을 이용한 직선근사화 오차 최적화 기법이 적용

R-peak를 중심으로 좌우 영역의 정점들이 가지는 전위, R-peak와의 시간차, 이웃한 정점과 이루는 각도 등의 특징값을 획득하고 이를 분석하여 QRS군의 기준점일 확률이 가장 높은 정점을 기준점으로 검출

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

 354 KB