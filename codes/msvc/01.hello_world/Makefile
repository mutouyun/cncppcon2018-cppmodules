# Define vars

CC = cl
CX = cl
LB = link
CFLAGS = /experimental:module /std:c++latest /EHsc /MD /c

# Compile

all: main

main.obj: main.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) $**

main: main.obj
	@$(LB) $**

# Targets

clean:
	@echo Cleaning directorys...
	@del /f /q *.exe *.obj
	@echo Clean complete.

# PHONY

.PHONY: all clean
