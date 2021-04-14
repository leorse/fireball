#OBJS specifies which files to compile as part of the project
OBJS = ./src/main.cpp ./src/SDLManagment.cpp ./src/Contexte.cpp ./src/Particule.cpp ./src/PartManager.cpp

#CC specifies which compiler we're using
CC = g++

INCLUDE_PATHS = -I./include
LIBRARY_PATHS =
ifeq ($(OS),Windows_NT)
	#INCLUDE_PATHS specifies the additional include paths we'll need
	INCLUDE_PATHS += -IF:\nim\nim-1.4.4\dist\mingw64\include\SDL2

	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS += -LF:\nim\nim-1.4.4\dist\mingw64\lib
endif

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -std=c++14 -D__CT_MEMORY_DEBUG__=1
#-Wl,-subsystem,windows

LINKER_FLAGS =
ifeq ($(OS),Windows_NT)
#LINKER_FLAGS specifies the libraries we're linking against
	LINKER_FLAGS += -lmingw32 
else
	LINKER_FLAGS += -lm
endif
LINKER_FLAGS += -lSDL2_ttf -lSDL2main -lSDL2 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./output/fireballCpp

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)