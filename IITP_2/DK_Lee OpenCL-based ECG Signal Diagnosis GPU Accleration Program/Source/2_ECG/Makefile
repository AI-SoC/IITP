.SUFFIXES: .cpp.o
SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}

DEFINE=DEFAULT

ifeq ($(OS),Windows_NT)
	TARGET = main.exe
	RM = del

	OPENCL_DIR	= "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\include"
	OPENCL_LIB	= "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\lib\x64"

	CFLAGS = -c -D${DEFINE} -I${OPENCL_DIR}
	LFLAGS = -L${OPENCL_LIB} -lOpenCL

else
	TARGET = ./main
	RM = rm -rf

	CFLAGS = -c -D${DEFINE}
	LFLAGS = -lOpenCL
endif


CC = g++

help:
	@echo "make all"
	@echo "make dis"

all: ${TARGET}
	${TARGET} data/DB.csv data/R_peak.csv

01:
	make all DEFINE=PRINT01
	gnuplot plot/01_plot_data.bat

02:
	make all DEFINE=PRINT02
	gnuplot plot/02_plot_tpl.bat

03:
	make all DEFINE=PRINT03
	gnuplot plot/03_plot_tpl.bat
	
04:
	make all DEFINE=PRINT04
	gnuplot plot/04_plot_dp.bat
	

${TARGET}: ${OBJS}
	${CC} -o $@ $^ ${LFLAGS} 

.cpp.o:
	${CC} $< ${CFLAGS}


clean:
	${RM} ${TARGET} *.o *.txt *.s

dis:
	objdump -S -d ${TARGET} > dis.s
