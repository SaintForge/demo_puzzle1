#ifndef MENU
#define MENU

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>
#include <string>

#include "Level_Manager.h"
#include "IO_Manager.h"

#ifdef PC
static const char* button_texture_path = "..\\data\\sprites\\button.png";
static const char* menu_background_path = "..\\data\\sprites\\menu_background.png";
static const char* font_path = "..\\data\\Karmina-Bold.otf";
#else
static const char* button_texture_path = "data/sprites/button.png";
static const char* menu_background_path = "data/sprites/menu_background.png";
static const char* font_path = "data/Karmina-Bold.otf";
#endif

static const int font_size = 40;

extern int lvl_amount;

#define GAME_EXIT 9000
#define MENU_EXIT 9001
#define MENU_OK   9002
#define NEW_GAME  9003

struct Button{
     SDL_Rect pos;
     std::string text;
     int status;
     int time;
};

enum MENU_STATE{
     MAIN     = 0,
     START    = 1,
     SETTINGS = 2,
     EXIT     = 3
};

struct Main_Menu{
     SDL_Texture *background_texture;
     SDL_Texture *button_texture;
     TTF_Font *font;
     TTF_Font *level_font;

     std::vector<Button> buttons;
     SDL_Rect background_area;

     bool mouse_left_button;
     MENU_STATE state;

     int hit_button;
};

Main_Menu *init_menu();
void destroy_menu(Main_Menu* menu);

int handle_event(Main_Menu *menu, SDL_Event *event);
void update_menu(Main_Menu *menu);

#endif
