#ifndef LEVEL
#define LEVEL

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <utility>
#include <vector>
#include <sstream>

#include "Window.h"
#include "Level_Info.h"
#include "Figure.h"
#include "Figure_Manager.h"
#include "Grid_Manager.h"
#include "Level_Editor.h"

#define LEVEL_INCOMPLETE 0
#define LEVEL_COMPLETE   1
#define LEVEL_EXIT       2

class Level_Manager{
public:
    Level_Manager();
    ~Level_Manager();

    void next_level(Level_Info& info);
    int handle_event(SDL_Event&);
    uint32_t total_time();

    int update();
    void draw();
private:
    void draw_timer(uint8_t r, uint8_t g, uint8_t b);
    void draw_frame();
    
    void load_image(SDL_Texture *&texture, const char* path);
    void load_level_number(SDL_Texture*&, SDL_Color);

    void update_level_number_animation();

    void init_editor_mode();
    void delete_editor_mode(); 
private:
    SDL_Texture *menu_exit_texture     = nullptr;
    SDL_Texture *menu_restart_texture  = nullptr;
    SDL_Texture *lvl_number_texture    = nullptr;
    SDL_Texture *lvl_number_shadow_texture = nullptr;
     
    Mix_Chunk *begin_sound             = nullptr;
    Mix_Chunk *complete_sound_1        = nullptr;
    Mix_Chunk *complete_sound_2        = nullptr;
    Mix_Music *music                   = nullptr;
    
    TTF_Font *font                     = nullptr;
    Level_Editor *level_editor         = nullptr; 
     
    SDL_Rect figure_area;
    SDL_Rect menu_bar_area;

    SDL_Rect exit_button;
    SDL_Rect restart_button;
    SDL_Rect level_number_rect;
    SDL_Rect level_line[2];

    uint8_t alpha = 255;
    uint8_t alpha_level_number = 0;
    bool level_complete = false;
    bool toogle_sound_completion = false;
    bool level_number_animation  = false;
    
    bool toggle_level_editor     = false;
    bool toggle_tilda_key        = false;
     
    Grid_Manager    grid_manager;
    Figure_Manager  figure_manager;

    Uint32 start_time;
    Uint32 end_time;
    int level_number;
    std::stringstream timer_text;
};

#endif
