OBJS = ./src/main.c ./src/particule.c ./src/contexte.c ./src/initSDL.c \
       ./src/ctools.c ./src/linkedList.c ./src/affichage.c ./src/shader.c

CC = gcc

INCLUDE_PATHS = -I./include
LIBRARY_PATHS =
ifeq ($(OS),Windows_NT)
	# Personnaliser SDL2_DIR selon votre installation
	SDL2_DIR ?= F:/nim/nim-1.4.4/dist/mingw64
	INCLUDE_PATHS += -I$(SDL2_DIR)/include/SDL2 -I$(SDL2_DIR)/include
	LIBRARY_PATHS += -L$(SDL2_DIR)/lib
endif

COMPILER_FLAGS = -std=c99 -D__CT_MEMORY_DEBUG__=1

LINKER_FLAGS =
ifeq ($(OS),Windows_NT)
	LINKER_FLAGS += -lmingw32
else
	LINKER_FLAGS += -lm
endif
LINKER_FLAGS += -lSDL2_ttf -lSDL2main -lSDL2
ifeq ($(OS),Windows_NT)
	LINKER_FLAGS += -lglew32 -lopengl32
else
	LINKER_FLAGS += -lGLEW -lGL
endif

OUT_DIR  = ./output
BIN_NAME = $(OUT_DIR)/fireballC

.PHONY: all clean

all: $(OUT_DIR) $(BIN_NAME) $(OUT_DIR)/shaders ressources

$(OUT_DIR):
ifeq ($(OS),Windows_NT)
	if not exist ".\output" mkdir ".\output"
else
	mkdir -p $(OUT_DIR)
endif

$(BIN_NAME): $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BIN_NAME)

# Copie les shaders dans output/ à chaque build (léger, toujours à jour)
$(OUT_DIR)/shaders: $(wildcard ./shaders/*)
ifeq ($(OS),Windows_NT)
	if not exist ".\output\shaders" mkdir ".\output\shaders"
	xcopy /Y /Q ".\shaders\*" ".\output\shaders\"
else
	mkdir -p $(OUT_DIR)/shaders
	cp -r ./shaders/. $(OUT_DIR)/shaders/
endif

# Copie le contenu de ressources/ directement dans output/
.PHONY: ressources
ressources:
ifeq ($(OS),Windows_NT)
	xcopy /Y /Q ".\ressources\*" ".\output\"
else
	cp ./ressources/* $(OUT_DIR)/
endif

clean:
ifeq ($(OS),Windows_NT)
	if exist ".\output\fireballC.exe" del /Q ".\output\fireballC.exe"
	if exist ".\output\shaders"       rmdir /S /Q ".\output\shaders"
	$(foreach f,$(notdir $(wildcard ./ressources/*)),\
		if exist ".\output\$(f)" del /Q ".\output\$(f)" &)
else
	rm -f  $(BIN_NAME) $(BIN_NAME).exe
	rm -rf $(OUT_DIR)/shaders
	rm -f  $(addprefix $(OUT_DIR)/, $(notdir $(wildcard ./ressources/*)))
endif
