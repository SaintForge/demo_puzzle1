#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include <string>
#include <sstream>
#include <utility>

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "Window.h"
#include "Menu.h"
#include "Level_Manager.h"
#include "IO_Manager.h"

bool is_running    = true;
bool escape_button = false;

int lvl_amount = 10; //TODO:read that value from binary

int SDL_main(int argc, char *argv[])
{
     srand(time(NULL));
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();

     Level_Manager *level = nullptr;
     int lvl = 0;

     Main_Menu *menu = init_menu();
     if(menu == NULL)
	  SDL_Log("Error Error Error \n");
     
     SDL_Event event;
     while(is_running)
     {
	  while(SDL_PollEvent(&event) != 0)
	  {
	       if(menu && !level){
		    int menu_result = handle_event(menu, &event);
		    if(menu_result == GAME_EXIT){
			 is_running = false;
			 printf("Game is finished!\n");
		    }
		    else if(menu_result >= 1 && menu_result <= lvl_amount){
			 if(level) delete level;
			 
			 lvl = menu_result;
			 printf("Loading level %d\n",lvl);
			 level = new Level_Manager();

			 Level_Info info;
			 read_level_info(info, lvl);
			 
			 level->next_level(info);
		    } 
	       }
	       else if(level){
		    if(level->handle_event(event) == LEVEL_EXIT){
			 delete level;
			 level = NULL;
		    }
	       }
	  }


	  // update and draw stuff

	  if(level){
	       if(level->update() == LEVEL_COMPLETE){
		    uint32_t time_result = level->total_time();
		    Level_Info info;
		    read_level_info(info, lvl);
		    
		    if(info.time == 0 || (info.time > time_result))
			info.time = time_result;
		    if(info.status == 0)
			info.status = 1;

		    update_level_info(info, lvl);
		    
		    lvl++;
		    if(lvl > lvl_amount){
			 printf("lvl > lvl_amount\n");
			 lvl = 1;
		    }

		    Level_Info info2;
		    read_level_info(info2, lvl);
		    if(!info2.status)
			 info2.status = 1;
		    update_level_info(info2, lvl);

		    level->next_level(info2);
		    printf("Level #%d\n",lvl);
	       }

	       level->draw();
	  }
	  else if(menu)
	       update_menu(menu);


	  SDL_RenderPresent(RenderScreen);
	  SDL_RenderClear(RenderScreen);
	  
	  // int time_loop = SDL_GetTicks() - start_loop;
	  // if(time_loop < 0) continue;

	  // int sleep_loop = 16 - time_loop;
	  // if(sleep_loop > 0)
	  //      SDL_Delay(sleep_loop);
     }

     return 0;

}
