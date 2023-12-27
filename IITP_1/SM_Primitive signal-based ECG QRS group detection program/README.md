# Primitive signal-based ECG QRS group detection program

## Program developer

Seungmin Lee, Daejin Park 

## Feature

주기적으로 유사한 형태의 신호가 반복되는 심전도 신호의 특성 상, QRS군에 대한 Primitive 신호를 사전에 정의하고, 차후 입력되는 심박에 대해 Primitive 신호와의 유사도를 통해 QRS군의 양끝점을 검출한다.

##   Function

 GUI를 이용하여 입력된 데이터로부터 Primitive를 설정하는 기능

 설정된 Primitive를 이용하여 각 심박의 QRS군의 시작점과 끝점을 검출하는 기능 및 GUI를 통한 결과 출력 기능

신호 분석을 위해 첨도와 전위값을 출력하는 기능

## How to use

1. MATLAB 혹은 MATLAB 라이브러리가 설치된 PC환경에서 Main_Figure.m 파일을 실행하여 GUI 호출

2. GUI 상단 탭에서 신호, R-peak 정보, Primitive 정보를 불러옴
    \- Primitive가 미설정 시, GUI 상단 탭에서 Primitive Reset을 통해 좌하단의 창에서 Primitive 영역을 설정
    \- https://physionet.org/ 에서 제공하는 QT-DB 중, sel30 데이터 예제 첨부

3. GUI 상단 탭에서 Primitive QRS 버튼을 이용하여 검출 실행

##  OS

 Windows10

## Language

MATLAB

## Required Program

MATLAB

## Size

 389 KB