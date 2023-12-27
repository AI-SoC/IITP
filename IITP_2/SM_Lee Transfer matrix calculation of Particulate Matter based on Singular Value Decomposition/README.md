# Transfer matrix calculation of Particulate Matter based on Singular Value Decomposition

## Program Developer

Seungmin Lee, Daejin Park



## Feature

저성능 테스트 먼지센싱 장비에서 PC값으로부터 PM값을 획득하기 위해, 고성능 레퍼런스 먼지센싱 장비의 PM 결과를 SVD를 이용하여 모방함으로써, 변환행렬을 획득하는 프로그램



## Function

테스트 장비로부터 PC값 획득

함께 측정된 레퍼런스 장비의 PM값과 테스트 장비의 PC값 간의 변환 행렬을 SVD를 이용하여 획득

SVD를 이용하여 획득된 변환 행렬을 테스트 장비의 PC값에 재적용하여 테스트 장비의 PM값을 획득

획득된 테스트 장비 PM값과 레퍼런스 장비의 PM값을 비교 출력하여 성능 분석



## How to Use

Fun_Main.m 파일을 시행

사전에 획득된 테스트 장비의 PC값과 레퍼런스 장비의 PC, PM값을 불러옴- 1016c 번 데이터 예제 첨부

SVD를 이용하여 레퍼런스 장비의 PM값을 모방할 수 있는 변환 행렬을 계산

변환 행렬을 다시 테스트 장비의 PC값에 적용하여 테스트 장비의 PM을 획득한 후 결과 비교



## OS

Windows



## Language

MATLAB



## Required Program

MATLAB



## Size

20,280KB