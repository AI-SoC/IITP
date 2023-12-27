# 사전 클럭트리합성 가능여부 판단을 위한 경량화된 클럭트리 재구성 방법

## Program Developer

권나영, 박대진



## Feature

core와 같은 일반적인 예제를 이용하여 특정 코드가 아닌 범용적인 코드에서 경량화된 클럭트리를 구성하는 CTS 알고리즘이다. 기존 licensed tool을 이용하여 중복되는 기대 비용을 감소할 수 있다. 또한, 경량화된 클럭트리 알고리즘을 이용하여, MCU의 클럭 트리를 최적화하여 칩 면적, 전력 소모를 낮출 수 있다. MCU와 같이 power, performance, area (PPA) 최적화를 중점으로 설계하는 기기에 경량화된 클럭트리를 재구성하여 전체적인 성능향상을 기대할 수 있다.

## Function

합성된 RTL code를 기반으로 pre-route과정에서 worst placement를 random buffer insertion 알고리즘을 통해 진행한다.

pre-route 과정 이후, worst placement 결과를 이용하여 최적의 clock tree synthesis를 진행한다.

각 과정마다, 표준편차, 삽입된 버퍼의 개수 등의 결과를 비교하여, 경량화된 CTS 결과를 분석 가능하다.

정확한 비교를 위해, Qflow의 기존 코드의 static timing analysis (STA)와 경량화된 CTS의 STA 결과 비교하여, 최대 주파수 분석 가능하다. 



## How to Use

Visual Studio 2022 실행하여 디버깅
 \- Qflow를 통해 합성된 RTL verilog code, CTS 알고리즘 첨부

RTL verilog code의 route 정보, pre-route 단계의 route 정보, CTS 단계의 route 정보

각 route 정보와 buffer insertion 개수, 표준편차, 평균 등에 관한 정보 출력

Qflow STA 결과와 경량화된 CTS 결과 비교하여, 최대 주파수에 대한 정보 비교 가능 (http://opencircuitdesign.com/qflow/)



## OS

 Windows



## Language

 C++/C언어



## Required Program

 Visual studio, Qflow(Open source)



## Sized

250MB