# Define vars

CC = cl
CX = cl
LB = link
CFLAGS = /experimental:module /std:c++latest /EHsc /MD /c

# Compile

all: main

hello.ifc hello.obj: hello.ixx
	@echo Compiling $**...
	@$(CX) $(CFLAGS) $**

mod.ifc mod.obj: mod.ixx
	@echo Compiling $**...
	@$(CX) $(CFLAGS) $**

mod_impl.obj: mod.ifc mod_impl.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) mod_impl.cpp

hello_impl.obj: hello.ifc mod.ifc hello_impl.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) hello_impl.cpp

main.obj: hello.ifc main.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) main.cpp

main: main.obj hello.obj hello_impl.obj mod.obj mod_impl.obj
	@echo Linking $**...
	@$(LB) $**

# Targets

clean:
	@echo Cleaning directorys...
	@del /f /q *.exe *.obj *.ifc
	@echo Clean complete.

# PHONY

.PHONY: all clean
