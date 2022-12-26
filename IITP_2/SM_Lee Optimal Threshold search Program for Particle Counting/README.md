# Optimal Threshold search Program for Particle Counting

## Program Developer

Seungmin Lee, Daejin Park



## Feature

저성능 테스트 먼지센싱 장비에서 PC을 획득과정에서 사용되는 다중임계값을 결정하기 위해, 고성능 레퍼런스 먼지센싱 장비의 PC값과 비교하여 최적의 다중임계값을 결정하는 프로그램



## Function

테스트 장비로부터 Raw 데이터 획득

함께 측정된 레퍼런스 장비의 PC값을 획득

임계값에 따른 PC값 획득 결과를 Look-up table로 사전에 형성함

다중임계값 조합에 따른 다채널 PC값의 결과를 레퍼런스 PC와 비교하여 유사도를 측정

최적 유사도를 갖는 다중임계값에서의 PC값을 반환



## How to Use

Fun_Main.m 파일을 시행

사전 획득된 테스트 장비의 Raw 데이터와 레퍼런스 장비의 PC값을 불러옴
 \- 1016c 번 데이터 예제 첨부

임계값에 따른 Raw 데이터의 PC 변환 결과를 Look-up table로 형성

레퍼런스 PC값과 비교하여 최적 다중임계값 탐색

최적 다중임계값에 의한 테스트 PC결과와 레퍼런스 PC와 비교 출력



## OS

Windows



## Language

MATLAB



## Required Program

MATLAB



## Size

20,280KB