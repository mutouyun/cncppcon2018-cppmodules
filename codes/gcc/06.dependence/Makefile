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

mod.nms: mod.cxx
	@echo Compiling $^...
	@$(CX) $(CFLAGS) $^

hello_impl.o: hello.nms hello_impl.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) hello_impl.cpp

mod_impl.o: mod.nms mod_impl.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) mod_impl.cpp

main.o: hello.nms mod.nms main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) main.cpp

main: main.o hello_impl.o mod_impl.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o *.nms *.gcm
	@echo Clean complete.

# PHONY

.PHONY: all clean
