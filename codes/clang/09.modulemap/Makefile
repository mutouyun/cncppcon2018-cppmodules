# Define vars

CC = clang
CX = clang++
LB = clang++
CFLAGS = -stdlib=libc++ -fmodules-ts -fmodules -fbuiltin-module-map
LFLAGS = -stdlib=libc++

# Compile

all: main

main.o: main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -fmodule-map-file=module.modulemap -fmodules-cache-path=. -c main.cpp

main: main.o
	@echo Linking $^...
	@$(LB) $(LFLAGS) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o
	@echo Clean complete.

# PHONY

.PHONY: all clean
