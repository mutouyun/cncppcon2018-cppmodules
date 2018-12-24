# Define vars

CC = clang
CX = clang++
LB = clang++
CFLAGS = -fmodules-ts

# Compile

all: main

mod.pcm: mod.cppm
	@echo Compiling $^...
	@$(CX) $(CFLAGS) --precompile $^

main.o: mod.pcm main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fprebuilt-module-path=. -ftemplate-depth=2048 main.cpp

main: main.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o *.pcm
	@echo Clean complete.

# PHONY

.PHONY: all clean
