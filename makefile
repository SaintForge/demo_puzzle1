C_FLAGS := -std=c++11 -g
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
	Game_Data.o

INCLUDE_PATH = -I"W:\SDL\sdl_dev_lib\include"
LIB_PATH := -L"W:\SDL\sdl_dev_lib\lib\x86"
LIB_FLAGS := -lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer

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
