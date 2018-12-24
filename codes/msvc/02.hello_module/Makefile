# Define exports

export CC ?= cl
export CX ?= cl
export LB ?= link

export CFLAGS ?= /experimental:module /std:c++latest /EHsc /MD /c

# Compile

all: main.exe

hello.ifc hello.obj: hello.ixx
	@echo Compiling $<...
	@$(CX) $(CFLAGS) $<

main.obj: hello.ifc main.cpp
	@echo Compiling $^...
	@$(CX) $(CFLAGS) /module:reference $^

main.exe: main.obj hello.obj
	@$(LB) $^

# Targets

clean:
	@echo Cleaning directorys...
	@del /f /q *.exe *.obj *.ifc
	@echo Clean complete.

# PHONY

.PHONY: all clean
