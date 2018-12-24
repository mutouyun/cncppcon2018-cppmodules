# Define vars

CC = clang
CX = clang++
LB = clang++
CFLAGS = -fmodules-ts

# Compile

all: main

hello.pcm: hello.cppm
	@echo Compiling $^...
	@$(CX) $(CFLAGS) --precompile $^

hello.o: hello.pcm
	@$(CX) -c $^

main.o: hello.pcm main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fprebuilt-module-path=. main.cpp

main: main.o hello.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o *.pcm
	@echo Clean complete.

# PHONY

.PHONY: all clean
