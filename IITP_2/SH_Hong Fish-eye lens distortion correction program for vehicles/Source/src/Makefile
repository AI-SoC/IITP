
GCCFLAGS = arm-linux-gnueabihf-g++
#g++
#arm-linux-gnueabihf-g++
CXXFLAGS = 
#-masm=intel
#CXXFLAGS += -O0
#CXXFLAGS += -msse4.2
#CXXFLAGS += -DUSE_OPENCV

ifeq ($(OS), Windows_NT)
LIBOS = windows
LIBFLAGS = -lopencv_highgui3414 -lopencv_core3414 -lopencv_videoio3414 -lopencv_imgproc3414 -lopencv_imgcodecs3414
else
LIBOS = linux
LIBFLAGS = -lopencv_highgui -lopencv_core -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs
endif

SRC_HOME = ./
APP_HOME = ../bin
APP_NAME = image 

INC_PATH = -I/usr/local/arm/opencv/include
#-I/usr/local/include
#-I/usr/local/arm/opencv/include
LIB_PATH = -L/usr/local/arm/opencv/lib
#-L/usr/local/lib
#-L/usr/local/arm/opencv/lib

OBJS = main.o

vpath %.cpp	${SRC_HOME}
vpath %.hpp	${SRC_HOME}

all: ${APP_NAME} 

${APP_NAME}: ${OBJS}
	$(GCCFLAGS) -o $@ ${OBJS} ${LIB_PATH} ${LIBFLAGS}

.cpp.o:
	$(GCCFLAGS) $(CXXFLAGS) -c $< ${INC_PATH}

clean:
ifeq ($(OS), Windows_NT)
	del *.o
	del *.exe
else
	rm -rf *.o
	rm -rf image
endif
