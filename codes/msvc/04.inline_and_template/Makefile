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

say.ifc say.obj: say.ixx
	@echo Compiling $**...
	@$(CX) $(CFLAGS) $**

foo.ifc foo.obj: foo.ixx
	@echo Compiling $**...
	@$(CX) $(CFLAGS) $**

foo_impl.obj: foo.ifc foo_impl.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) foo_impl.cpp

main.obj: hello.ifc say.ifc foo.ifc main.cpp
	@echo Compiling $**...
	@$(CX) $(CFLAGS) main.cpp

main: main.obj hello.obj foo_impl.obj
	@$(LB) $**

# Targets

clean:
	@echo Cleaning directorys...
	@del /f /q *.exe *.obj *.ifc
	@echo Clean complete.

# PHONY

.PHONY: all clean
