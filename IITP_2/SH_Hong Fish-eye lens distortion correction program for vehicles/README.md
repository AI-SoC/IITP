# Fish-eye lens distortion correction program for vehicles

## Program Developer

Seonghoon Hong, Daejin Park



## Feature

어안렌즈 카메라는 180도 이상인 초광각 렌즈로 넓은 화각을 가지고 있는데, 넓은 시야를 볼 수 있어서 최소의 카메라로 최대 시야각을 확보할 수 있다.

어안렌즈는 렌즈 굴절에의해 왜곡이 심한데, 이러한 왜곡을 보정하는 프로그램이다.



## Function

어안렌즈 카메라의 왜곡 보정된 이미지를 출력한다.

계산된 왜곡 보정좌표를 메모리에 저장하여 CPU의 부하를 줄인다.



## How to Use

images 폴더에서 테스트를 위한 왜곡 이미지 첨부

src 폴더에서 make all 사용하여 빌드

./image 실행

원본 이미지와 왜곡 보정된 이미지 출력



## OS

Linux



## Language

C



## Required Program

OpenCV



## Size

10.5KB