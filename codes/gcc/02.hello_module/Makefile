# Define vars

CC_PATH = /usr/local/bin/gcc-modules/bin

CC = $(CC_PATH)/gcc
CX = $(CC_PATH)/g++
LB = $(CC_PATH)/g++
CFLAGS = -c -fmodules-ts

# Compile

all: main

hello.nms hello.o: hello.cxx
	@echo Compiling $^...
	@$(CX) $(CFLAGS) $^

main.o: hello.nms main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) main.cpp

main: main.o hello.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o *.nms *.gcm
	@echo Clean complete.

# PHONY

.PHONY: all clean
