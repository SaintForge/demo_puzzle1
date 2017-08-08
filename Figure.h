#ifndef T_BONES
#define T_BONES

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "window.h"

static int default_block_size = 26;
static int active_block_size  = 50;
static int get_defblock_size() { int size = default_block_size; return size;}
static int get_activeblock_size() { int size = active_block_size; return active_block_size; }

enum Figure_Form{
     O_figure = 0, I_figure,
     L_figure, J_figure,
     Z_figure, S_figure,
     T_figure
};

enum Figure_Type{
     classic,
     stone,
     mirror
};
//TODO: add a figure that transforms to another maybe???

class Figure{
public:
     Figure(Figure_Form form, Figure_Type type);
     ~Figure();

     SDL_Rect         get_area();
     SDL_Point        get_center();
     SDL_Point        get_def_center();
     
     SDL_Point*       get_shell();
     SDL_Point*       get_def_shell();
     Figure_Type      get_type();
     
     int              get_size();
     int              get_width();
     int              get_height();
     int              get_block_size();

     
     bool is_attached();
     bool is_sticked();
     bool is_idle();
     bool is_flip();

     void set_to(int x, int y);
     void move_figure(float shift_x, float shift_y);

     void transparent_on();
     void transparent_off();


     void set_default(int x, int y);
     void set_default_state();
     void set_idle();
     
     void mouse_attach(bool attach);
     void grid_stick();

     void flip_figure();
     void rotate_shell(float angle_dx = 0);
     void update_angle(float angle);
     
     void draw();
private:
     void load_figure(Figure_Form, Figure_Type);
     void load_figure_image(const char* path);
     
     void scale_image(int scale_dx);
private:
     SDL_Texture *sprite  = nullptr;

     SDL_Rect sprite_area;
     Figure_Type fig_type;
     
     SDL_RendererFlip flip = SDL_FLIP_NONE;
     float angle = 0;
     
     bool idle            = true;
     bool stick           = false;
     
     // for the skeleton
     float         angle_offset[4];
     float 	   def_angle;

     SDL_Point     rel_center;
     SDL_Point     center;
     SDL_Point     def_center;
     SDL_Point     point[4];
     SDL_Point     def_point[4];
};

#endif
