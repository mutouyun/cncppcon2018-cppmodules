# Define vars

CC_PATH = /usr/local/bin/gcc-modules/bin

CC = $(CC_PATH)/gcc
CX = $(CC_PATH)/g++
LB = $(CC_PATH)/g++
CFLAGS = -c -fmodules-ts

# Compile

all: main

hello.nms: hello.cxx
	@echo Compiling $^...
	@$(CX) $(CFLAGS) $^

say.nms: say.cxx
	@echo Compiling $^...
	@$(CX) $(CFLAGS) $^

foo.nms: foo.cxx
	@echo Compiling $^...
	@$(CX) $(CFLAGS) $^

foo_impl.o: foo.nms foo_impl.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) foo_impl.cpp

main.o: hello.nms say.nms foo.nms main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) main.cpp

main: main.o hello.o say.o foo.o foo_impl.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o *.nms *.gcm
	@echo Clean complete.

# PHONY

.PHONY: all clean
