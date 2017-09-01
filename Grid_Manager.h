#ifndef GRID
#define GRID

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "Figure_Manager.h"
#include "Level_Info.h"
#include "MovingBlock.h"

enum GRID_STATE
{
     GRID_EMPTY = 0,
     GRID_FULL  = 1,
     GRID_ZERO  = 2,
     GRID_HIT   = 3
};

struct Sticked_Unit
{
     Sticked_Unit() : index(0){}
     ~Sticked_Unit(){}
     
     int index;
     int row[4]{0};
     int col[4]{0};

     SDL_Point targ_cntr;
     bool is_sticked = false;
};

class Grid_Manager
{
public:
     Grid_Manager();
     ~Grid_Manager();

     bool GridMouseClick(int, int);
     bool CellMouseClick(int, int, int*, int*);
     bool check_rectangle_collision(int, int, SDL_Rect* );

     void change_block(int, int);

     void update_grid(int, int); 
     void update_grid(Figure_Manager*, int, int, Level_Info&);
     void restart_grid();

     int get_column_amount();
     int get_row_amount();
     int get_block_size();
     int get_last_stick_index();

     bool is_full();
     bool unattach_figure(int index);

     void handle_event(SDL_Event&);
     int  update();
     void draw();
private:
     void add_moving_block(int, int);
     void delete_moving_block(int, int);
     
     void load_grid_texture(SDL_Texture *&, const char* path);
     bool is_figure_inside(std::shared_ptr<Figure> fig);
     bool animate_start();
private:
     SDL_Texture* GridCellTexture    = NULL;
     SDL_Texture* MovingBlockTexture = NULL; 
     Mix_Chunk *piece_snap  = NULL;
     Mix_Chunk *block_sound = NULL;

     GRID_STATE state     = GRID_EMPTY;
     int size             = 40;
     int row_amount       = 0;
     int column_amount    = 0;
     int mov_indx      = 0; 
     
     bool start_animation = true;
     bool mouse_pressed   = false;
     bool block_grabbed   = false; 

     SDL_Rect grid_area;

     std::vector<MovingBlock> mov_bl; 
     std::vector<std::vector<uint8_t>> bit_field;
     std::vector<Sticked_Unit> stick_list;
     std::vector<uint8_t> ani_block;
     Figure_Manager* manager;
};



#endif
