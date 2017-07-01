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

enum GRID_STATE{
     GRID_EMPTY = 0,
     GRID_FULL  = 1,
     GRID_ZERO  = 2,
     GRID_HIT   = 3
};

struct Sticked_Unit{
     Sticked_Unit() : index(0){}
     ~Sticked_Unit(){}
     
     int index;
     int row[4]{0};
     int col[4]{0};
     SDL_Point targ_cntr;
};

class Grid_Manager{
public:
     Grid_Manager();
     ~Grid_Manager();

     void update_grid(Figure_Manager*,
		      int, int,
		      Level_Info&);
     void restart_grid();
     bool is_full();
     int get_block_size();
     int get_last_stick_index();

     bool unattach_figure(int index);

     int update();
     void draw();
private:
     void load_grid_texture(SDL_Texture *&, const char* path);
     bool is_figure_inside(std::shared_ptr<Figure> fig);
     bool animate_start();
private:
     SDL_Texture* texture   = NULL;
     SDL_Texture* texture2  = NULL;
     Mix_Chunk *piece_snap  = NULL;
     Mix_Chunk *block_sound = NULL;

     GRID_STATE state     = GRID_EMPTY;
     int size             = 50;
     int row_amount       = 0;
     int column_amount    = 0;
     bool stick_effect    = false;
     
     bool start_animation = true;

     SDL_Rect grid_area;
     
     std::vector<std::vector<uint8_t>> bit_field;
     std::vector<Sticked_Unit> stick_list;
     std::vector<uint8_t> ani_block;
     Figure_Manager* manager;
};



#endif
