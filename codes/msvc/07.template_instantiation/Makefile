# Define vars

CC = cl
CX = cl
LB = link
CFLAGS = /experimental:module /std:c++latest /EHsc /MD /c

# Compile

all: main

mod.ifc mod.obj: mod.ixx
	@echo Compiling $**...
	@$(CX) $(CFLAGS) $**

main.obj: mod.ifc main.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) main.cpp

main: main.obj mod.obj
	@echo Linking $**...
	@$(LB) $**

# Targets

clean:
	@echo Cleaning directorys...
	@del /f /q *.exe *.obj *.ifc
	@echo Clean complete.

# PHONY

.PHONY: all clean
