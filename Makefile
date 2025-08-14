PROJECTNAME = SDLtest
OUTPUT_DIR = build

INCLUDE_DIRS = -Iinclude -Iinclude/imgui
LIB_DIRS = -Llib

LIBS = -lmingw32 -lSDL3

OPTIMISATION = -O1

SRC = $(wildcard src/*.cpp) $(wildcard imgui/*.cpp)

default:
	g++ $(SRC) -o $(OUTPUT_DIR)/$(PROJECTNAME) $(INCLUDE_DIRS) $(LIB_DIRS) $(LIBS) $(OPTIMISATION)