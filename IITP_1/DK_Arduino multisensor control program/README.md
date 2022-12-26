# Arduino multisensor control program

## Program developer

Dongyku Jung, Daejin Park 

## Feature

 싱글 프로세스 마이크로 컨트롤러인 아두이노 보드로 여러개의 다종 센서로부터 전송되는 데이터들을 딜레이 없이 수신하고 제어함으로써 센서의 실시간성과 정확성을 보장하는 데이터 수집을 수행

## Function

 실시간 OS를 이용하여 센서로부터 전송되는 데이터를 순서에 따라 수신하며, 발생하는 지연시간에 의해 수신 주기가 변경되지 않도록 버퍼의 크기와 우선순위를 조절하며 센서를 제어함

## How to use

 업데이트 대상 타겟 아두이노 보드를 호스트 Windows 기반의 호스트 PC와 연결, Arduino IDE 실행 후 프로그램 파일을 열고 보드가 연결된 포트명을 입력하여 보드에서 프로그램을 수행함

## OS

Windows

## Language

 C

## Required Program

 Arduino IDE

## Size

 8 KB