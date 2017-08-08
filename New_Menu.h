#include <SDL.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <vector>
#include <string>
#include <stack>

#include "Game_Data.h"
#include "Vector2.h"

static const char* button_texture_path = "..\\data\\sprites\\circle_touch.png";
static const char* button_highligth = "..\\data\\sprites\\circle_touch2.png";
static const char* font_path = "..\\data\\Karmina-Bold.otf";

static const int font_size = 40;

enum Select_Menu{
    MAIN_PAGE, // play, sound, about, feedback
    DIFF_PAGE, // easy, normal, hard
    EASY_PAGE, // 1-20 level page, back
    NORMAL_PAGE,
    HARD_PAGE,
    SETTINGS_PAGE, // dunno yet
    ABOUT_PAGE
};

struct Button_Info{
    int status;
    SDL_Rect pos;
    bool is_button = false;
    std::vector<std::string> str_array;
};

struct Level_Mode{
public:
    Level_Mode(){}
    ~Level_Mode(){}
    
    int scroll_target   = 0;
    int move_capacity   = 0;
    uint32_t time_ticks = 0;
    
    Vector2 velocity;
    
    SDL_Rect scroll_bounds;

    uint32_t page_number;
    uint32_t page_number_max;
    std::string page_info;
    SDL_Rect page_info_rect;
};

class Layer{
public:
    Layer();
    void init_layer(Select_Menu, SDL_Texture*&,
		    SDL_Texture*&, TTF_Font*&);
    ~Layer();
    
    int do_event(SDL_Event&);
    
    void open();
    void close();
    
    bool is_level_screen();
    void update_level_screen(int index);
    
    bool update();
    void render();
private:
    bool update_button_animation();
    void render_text(SDL_Rect&, std::vector<std::string>&);
    
    void check_input();
    bool check_scroll_area(int x, int y);
    
    void init_buttons(Select_Menu);
    void next_page(int offset, bool);
    void prev_page(int offset, bool);
    void animate_scroll();

    void update_page_number(int num);
    void update_page_info_texture();
    void delete_page_info();
private:
    SDL_Texture *button_texture;
    SDL_Texture *highlight_texture;
    SDL_Texture *page_info_texture;
    TTF_Font *font;

    std::vector<Button_Info> btn;
    int click_pos;
    int click_index = -1;
    int speed;

    Level_Mode *lvl_info = NULL;

    bool level_page       = false;
    bool scroll_animation = false;
    bool is_scrolling     = false;
    bool is_moving        = false;
    
    bool is_closing = false;
    bool is_opening = false;
    bool is_pressed = false;
};

#define NOTHING 1000
#define PLAY    1001
#define EXIT    1002

#define EASY    1003
#define NORMAL  1004
#define HARD    1005

class Menu
{
public:
    Menu(Select_Menu);
    ~Menu();
 
    int handle_event(SDL_Event&);
    const std::string& get_lvl_mode();

    void update();
    void update_button(int index);
    void render();
private:
    void init_texture(SDL_Texture*&, const char*);
    void init_font();

    int check_input(int x, int y);
    void render_text(Button_Info&);
private:
    SDL_Texture *button_texture;
    SDL_Texture *highlight_texture;
    TTF_Font* font;

    std::string lvl_mode;

    Select_Menu menu_layer;
    int select_button;
    std::stack<Layer> layer;

    bool is_loading;
    bool is_pressed;
};

