# RPC-based Remote Python Code Execution

## Program Developer

Myeongjin Kang, Daejin Park



## Feature

성은이 낮거나 프로그램이 설치되지 않은 환경에서 원격으로 서버에 연결하여 프로그램을 실행하고, 입력값에 따른 결과값을 출력으로 받을 수 있다.



## Function

RPC(Remote Procedure Call)을 사용하여 서버와 클라이언트를 연결 후 클라이언트에서 MSG로 메시지를 보낸다. 서버는 이를 수신후, 성공시 수신한 메시지에 맞게 파이선 프로그램을 실행하여 이후 수신받는 데이터값에 따라 처리한 후 다시 반환한다.



## How to Use

각 Server와 Cleint에서 실행될 코드르 Make를 통해 컴파일 후 실행한다. Client는 실행파일을 실행시키며 전송할 데이터를 함께 입력하여 server의 결과값을 반환 받는다.



## OS

Linux



## Language

C++, 파이썬



## Required Program

GCC Compiler, Make, Python, RPC

## Size

29475 byte