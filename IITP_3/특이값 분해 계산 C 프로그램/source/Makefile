CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lm
LDLIBS=

INC_DIR=-I./inc
SRC_DIR=./src
OBJ_DIR=./obj

SRCS=$(notdir $(wildcard $(SRC_DIR)/*.c))
OBJS=$(SRCS:.c=.o)
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))

TARGET=main

help:
	@echo "make all"
	@echo "make run"
	@echo "make clean"

all: $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@ -MD $(LDFLAGS)

$(OBJ_DIR)/$(TARGET).o : $(TARGET).c
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJECTS) $(OBJ_DIR)/$(TARGET).o
	$(CC) $(CFLAGS) $(OBJECTS) $(OBJ_DIR)/$(TARGET).o -o $(TARGET) $(LDFLAGS)

run:
	if [ -a log ]; then rm log; fi;
	./$(TARGET) data.txt | tee log

clean:
	rm -rf ./$(TARGET) ./obj/* massif.out.* log
