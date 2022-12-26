
CXXFLAGS = -masm=intel
CXXFLAGS += -O0
#CXXFLAGS += -DUSEOPENCV

ifeq ($(OS), Windows_NT)
LIBOS = windows
LIBFLAGS = -lopencv_highgui3414 -lopencv_core3414 -lopencv_videoio3414 -lopencv_imgproc3414
else
LIBOS = linux
LIBFLAGS = -lopencv_highgui -lopencv_core -lopencv_videoio -lopencv_imgproc
endif

SRC_HOME = ./
APP_HOME = ../bin
APP_NAME = fcws

INC_PATH = ../lib/${LIBOS}
LIB_HOME = ../lib/${LIBOS}
LIB_PATH = ${LIB_HOME}/bin

OBJS = main.o carnavi_fcws.o

vpath %.cpp	${SRC_HOME}
vpath %.hpp	${SRC_HOME}

all: ${APP_NAME} 

${APP_NAME}: ${OBJS}
	g++ -L${LIB_PATH} -o $@ ${OBJS} ${LIBFLAGS}

.cpp.o:
	g++ $(CXXFLAGS) -c -I${INC_PATH} $<

clean:
ifeq ($(OS), Windows_NT)
	del *.o
else
	rm -rf *.o
endif
