PROJECTNAME = SDLtest
OUTPUT_DIR = build

INCLUDE_DIRS = -Iinclude -Iinclude/imgui
LIB_DIRS = -Llib

LIBS = -lmingw32 -lSDL3 -lSDL3_image

OPTIMISATION = -O3
VERSION = -std=c++23

SRC = $(wildcard src/*.cpp) $(wildcard imgui/*.cpp)

default:
	g++ $(VERSION) $(SRC) -o $(OUTPUT_DIR)/$(PROJECTNAME) $(INCLUDE_DIRS) $(OPTIMISATION) $(LIB_DIRS) $(LIBS) 