#ifndef F_MANAGER
#define F_MANAGER

#include <SDL.h>
#include <SDL_image.h>

#include <memory>
#include <vector>
#include <stdio.h>
#include <chrono>

#include "Figure.h"
#include "Vector2.h"

#define ANIMATION_SPEED 50.0
#define ALPHA_SPEED     120.0

using namespace std::chrono;

class Figure_Manager{
public:
     Figure_Manager();
     ~Figure_Manager();

     void change_figures(std::vector<std::pair<Figure_Form, Figure_Type>>&,
			 std::vector<uint8_t>&);
     
     void toogle_stick_effect(int index);
     void animate_stick_effect();
     
     void align_horisontally(std::vector<uint8_t>&);
     void align_vertically();
     void restart();

     std::vector<std::shared_ptr<Figure>> *get_container();
     SDL_Rect get_idle_zone();
     std::shared_ptr<Figure> get_figure_at(int index);
     int get_figure_amount();

     bool is_grabbed();
     bool is_attached(int index);
     bool is_being_restarted();
     bool is_being_rotated();

     bool move_figure(int index, SDL_Point &target);
     void handle_event(SDL_Event &event);
     
     void figure_high_priority(int index);
     void figure_low_priority(int index);
     
     void update();
     void draw();
     void draw_shadow(int index);
private:
     int  check_input(int x, int y);
     void grab_figure(int index);
     void release_figure();
     bool check_edge_collision();
     void load_effect(const char*);
     void rotate_figure(int index);
     void return_to_idle_zone(int index);
private:
     SDL_Texture *idle_effect  = nullptr;
     Mix_Chunk *return_sound   = nullptr;
     Mix_Chunk *idle_sound     = nullptr;
     Mix_Chunk *rotation_sound = nullptr;
     Mix_Chunk *release_sound  = nullptr;
     Mix_Chunk *grab_sound     = nullptr;
          
     SDL_Rect default_zone;
     int block_width = 30; //TODO
      
     Uint32 start_ticks = 0; // TODO

     std::vector<int> O {0};
     std::vector<std::shared_ptr<Figure>> figure_container;

     int current;
     bool grabbed;
     bool adjust_move;
     int x_rel, y_rel;

     SDL_Rect idle_rect;
     uint8_t alpha;
     bool is_idle;

     bool idle;// need to be initialized
     int idle_index;
     const float speed = 1000.0f; //TODO

     bool is_rotating;
     int r_index;
     const int r_vel = 630.0f; //TODO
     float r_target;

     bool hit;
     int hit_index;
     bool mouse_left_button;
     bool mouse_right_button;

     bool restart_figures;

     bool stick_effect          = false;
     int stick_effect_index     = -1;
     uint8_t stick_effect_alpha = 200;

     // Vector2 vel;
     float approach_radius = 50;
};

#endif 
