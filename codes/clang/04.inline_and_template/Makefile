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

say.pcm: say.cppm
	@echo Compiling $^...
	@$(CX) $(CFLAGS) --precompile $^

foo.pcm: foo.cppm
	@echo Compiling $^...
	@$(CX) $(CFLAGS) --precompile $^

foo_impl.o: foo.pcm foo_impl.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fmodule-file=$^

main.o: hello.pcm say.pcm foo.pcm main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fprebuilt-module-path=. main.cpp

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
