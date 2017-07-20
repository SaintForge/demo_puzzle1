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
#include "New_Menu.h"
#include "Level_Manager.h"
#include "IO_Manager.h"

bool is_running    = true;
bool escape_button = false;

int SDL_main(int argc, char *argv[])
{
     srand(time(NULL));
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();

     Level_Manager *level = nullptr;
     int lvl = 0;
     
     Menu *menu = new Menu(MAIN_PAGE);

     std::string lvl_mode = "easy"; //TODO: menu.get_lvl_mode();
     int lvl_amount = 0;
     uint32_t current_lvl_time = 0;

     SDL_Event event;
     while(is_running)
     {
	  while(SDL_PollEvent(&event) != 0)
	  {
	       if(menu && !level){
		    int menu_result = menu->handle_event(event);
		    switch(menu_result){
			case EXIT:{
			    is_running = false;
			    printf("Game is finished!\n");
			} break;
			case EASY:{
			    lvl_mode = "easy";
			} break;
			case NORMAL:{
			    lvl_mode = "normal";
			} break;
			case HARD:{
			    lvl_mode = "hard";
			} break;
			default:{
			    if(menu_result >= 1 && menu_result < 1000){
				if(level) delete level;
				lvl = menu_result;
			 
				printf("Loading level %d\n",lvl);
				level = new Level_Manager();

				Document doc;
				read_bin(doc);

				Text_Iterator it = doc.begin();
				read_variable(doc, it, lvl_mode, lvl_amount);
				
				Level_Info info;
				read_level_info(doc, doc.begin(), info, lvl, lvl_mode);
				
				level->next_level(info);
				current_lvl_time = info.time;
			    }
			} break;
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
		    
		    Document doc;
		    read_bin(doc);
			
		    if(current_lvl_time == 0 || (time_result < current_lvl_time)){
			Level_Info lvl_info;
			read_level_info(doc, doc.begin(), lvl_info, lvl, lvl_mode);
			update_level_info(doc, lvl_info, lvl, lvl_mode);
		    }

		    if(lvl > lvl_amount - 1){
			 printf("lvl > lvl_amount\n");
			 delete level;
			 level = NULL;
		    }
		    else{
			lvl++;
			
			Level_Info lvl_info;
			read_level_info(doc, doc.begin(), lvl_info, lvl, lvl_mode);
		    
			if(lvl_info.status) lvl_info.status = 0;
			menu->update_button(lvl);
			
			update_level_info(doc, lvl_info, lvl, lvl_mode);
			save_bin(doc);

			level->next_level(lvl_info);
			printf("Level #%d\n",lvl);
		    }
	       }

	       if(level) level->draw();
	  }
	  else if(menu){
	      menu->update();
	      menu->render();
	  }


	  SDL_RenderPresent(RenderScreen);
	  SDL_RenderClear(RenderScreen);
     }

     return 0;

}
