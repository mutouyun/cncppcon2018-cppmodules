# Define exports

export CC ?= cl
export CX ?= cl
export LB ?= link

export CFLAGS ?= /experimental:module /std:c++latest /EHsc /MD /c

# Compile

all: main.exe

main.obj: main.cpp
	@echo Compiling $<...
	@$(CX) $(CFLAGS) $<

main.exe: main.obj
	@$(LB) $<

# Targets

clean:
	@echo Cleaning directorys...
	@del /f /q *.exe *.obj
	@echo Clean complete.

# PHONY

.PHONY: all clean
