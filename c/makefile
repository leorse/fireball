#OBJS specifies which files to compile as part of the project
OBJS = main.c particule.c contexte.c initSDL.c ctools.c linkedList.c affichage.c

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IF:\nim\nim-1.4.4\dist\mingw64\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LF:\nim\nim-1.4.4\dist\mingw64\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -std=c99 -D__CT_MEMORY_DEBUG__=1
# -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2_ttf -lSDL2main -lSDL2 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = fireballC

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)