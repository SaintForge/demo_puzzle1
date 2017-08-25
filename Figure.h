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
static int active_block_size  = 40;

enum Figure_Form
{
     O_figure,
     I_figure,
     L_figure,
     J_figure,
     Z_figure,
     S_figure,
     T_figure
};

enum Figure_Type
{
     classic,
     stone,
     mirror
};
//TODO: add a figure that transforms to another maybe???

class Figure
{
public:
     Figure(Figure_Form form, Figure_Type type);
     ~Figure();

     SDL_Rect         get_area();
     SDL_Point        get_center();
     SDL_Point        get_def_center();
     
     SDL_Point*       get_shell();
     SDL_Point*       get_def_shell();
     Figure_Form      get_form();
     Figure_Type      get_type();
     
     int get_size();
     int get_width();
     int get_height();
     int get_angle();
     int get_block_size();
     
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
     void scale_image(int scale_dx);
     void set_idle();
     
     void mouse_attach(bool attach);
     void grid_stick();

     void flip_figure();
     void rotate_shell(float angle_dx = 0);
     void update_angle(float angle);
     
     void draw();
     void draw_shell();
private:
     void load_figure(Figure_Form, Figure_Type);
     void load_figure_image(const char* path);
     

private:
     SDL_Texture *sprite = NULL;

     SDL_Rect sprite_area;

     Figure_Form fig_form;
     Figure_Type fig_type;
     
     SDL_RendererFlip flip = SDL_FLIP_NONE;
     
     bool idle            = true;
     bool stick           = false;
     
     // for the skeleton
     float         angle = 0;
     float         angle_offset[4];
     float 	   def_angle;

     SDL_Point     rel_center;
     SDL_Point     center;
     SDL_Point     def_center;
     SDL_Point     point[4];
     SDL_Point     def_point[4];
};

#endif
