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

mod.pcm: mod.cppm
	@echo Compiling $^...
	@$(CX) $(CFLAGS) --precompile $^

hello_impl.o: hello.pcm mod.pcm hello_impl.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fprebuilt-module-path=. -fmodule-file=hello.pcm hello_impl.cpp

mod_impl.o: mod.pcm mod_impl.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fprebuilt-module-path=. -fmodule-file=$^

main.o: hello.pcm main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) -c -fprebuilt-module-path=. main.cpp

main: main.o hello_impl.o mod_impl.o
	@echo Linking $^...
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@rm *.out *.o *.pcm
	@echo Clean complete.

# PHONY

.PHONY: all clean
