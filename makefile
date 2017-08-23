C_FLAGS := -std=c++11
CC := g++
RM := rm

.PHONY: $(TARGET)
.PHONY: clean
.PHONY: pre_build

VPATH := obj/
OBJ_PATH := obj/
EXE := build/puzzle_demo.exe

OBJ_LIST := demo_main.o \
	Figure.o \
	Figure_Manager.o \
	Grid_Manager.o \
	Window.o \
	Level_Manager.o \
	New_Menu.o \
	Game_Engine.o \
	Game_Data.o \
	Grid_Editor.o \
	Figure_Editor.o

INCLUDE_PATH = -I"W:\stuff\Libs\SDL2_all\include"
LIB_PATH := -L"W:\stuff\Libs\SDL2_all\lib"
LIB_FLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

OBJ := $(patsubst %,$(OBJ_PATH)%,$(OBJ_LIST))

# Build .o from .cpp
$(OBJ_PATH)%.o : %.cpp
	@echo [CC] $<
	@g++ $(CFLAGS) -o $@ $(INCLUDE_PATH) -c $<

$(EXE) : $(OBJ)
	@echo
	@echo Output:
	@$(CC) -o $@ $^ $(LIB_PATH) $(LIB_FLAGS)
	@echo     $@

clean:
	rm $(OBJ_PATH)*.o
	rm $(EXE)
