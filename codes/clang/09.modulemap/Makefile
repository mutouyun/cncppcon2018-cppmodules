# Define vars

CC = clang
CX = clang++
LB = clang++
CFLAGS = -fmodules-ts

# Compile

all: main

main.o: main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -fmodules -fmodule-map-file=module.modulemap -c main.cpp

main: main.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o
	@echo Clean complete.

# PHONY

.PHONY: all clean
