#include <stdio.h>

#include "Window.h"
#include "New_Menu.h"
#include "Level_Manager.h"
// #include "IO_Manager.h"
#include "Game_Data.h"

class Game_Engine{
public:
    Game_Engine();
    virtual ~Game_Engine();

    void run();
private:
    void menu_events();
    void level_events();

    void update_menu();
    void update_level();
private:
    Level_Manager *level = NULL;
    Menu *menu = NULL;
        
    SDL_Event event;
    int level_number = 0;
    int level_amount = 0;
    uint32_t current_lvl_time = 0;
    bool escape_button = false;
    bool game_loop = true;
};
