#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "Game_Engine.h"

Game_Engine::Game_Engine()
{
    printf("Game_Engine()\n");
}

Game_Engine::~Game_Engine()
{
    printf("~Game_Engine()!\n");
}

void Game_Engine::run()
{
    SDL_Renderer *RenderScreen = Window_Info::get_renderer();
    menu = new Menu(Select_Menu::MAIN_PAGE);
    
    while(game_loop)
    {
	while(SDL_PollEvent(&event) != 0)
	{
	    if(menu && !level)
	    {
		menu_events();
	    }
	    else if(level)
	    {
		level_events();
	    }
	}

	if(level)
	{
	    update_level();
	}
	else if(menu)
	{
	    update_menu();
	}
	
	SDL_RenderPresent(RenderScreen);
	SDL_RenderClear(RenderScreen);
    }
}

void Game_Engine::menu_events()
{
    int menu_result = menu->handle_event(event);
    switch(menu_result)
    {
	case EXIT:
	{
	    game_loop = false;
	    printf("Game is finished!\n");
	} break;
	default:
	{
	    if(menu_result >= 1 && menu_result < 1000)
	    {
		if(level)
		{
		     delete level;
		}
		
		level_number = menu_result;
			 
		printf("Loading level %d\n",level_number);
		level = new Level_Manager();

		printf("opening data!\n");
		GameData data;
		if(data.open_data())
		{
		    std::string lvl_mode = menu->get_lvl_mode();

		    int tmp_index = 0;
		    int start_index = data.read_variable(tmp_index, lvl_mode, level_amount);
		    data.set_searching_point(start_index);
				
		    Level_Info lvl_info;
		    data.load_level(level_number, lvl_info);
				
		    level->next_level(lvl_info, level_number);
		    current_lvl_time = lvl_info.time;
		}
	    }
	} break;
    }
}

void Game_Engine::level_events()
{
    if(level->handle_event(event) == LEVEL_EXIT){
	delete level;
	level = NULL;
    }
}

void Game_Engine::update_menu()
{
    menu->update();
    menu->render();
}

void Game_Engine::update_level()
{
    if(level->update() == LEVEL_COMPLETE)
    {
	uint32_t time_result = level->total_time();
		    
	GameData data;

	if(data.open_data())
	{
	    std::string lvl_mode = menu->get_lvl_mode();
	    int tmp_index = 0;
	    int start_index = data.read_variable(tmp_index, lvl_mode, level_amount);
	    data.set_searching_point(start_index);
			
	    if(current_lvl_time == 0 || (time_result < current_lvl_time))
	    {
		Level_Info lvl_info;
		data.load_level(level_number, lvl_info);
		lvl_info.time = time_result;
		data.save_level(level_number, lvl_info);
	    }

	    if(level_number > level_amount - 1)
	    {
		printf("lvl > lvl_amount\n");
		delete level;
		level = NULL;
	    }
	    else
	    {
		level_number++;
		Level_Info lvl_info;

		data.load_level(level_number, lvl_info);
		    
		if(lvl_info.status) lvl_info.status = 0;
		menu->update_button(level_number);
			
		data.save_level(level_number, lvl_info);
		data.save_data();

		level->next_level(lvl_info, level_number);
		printf("Level #%d\n",level_number);
	    }
	}
	else
	{
	    printf("Coudn't read the level\n");
	}
    }

    if(level)
    {
	 level->draw();
    }
}

#endif
