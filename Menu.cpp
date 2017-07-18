#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "Window.h"
#include "Menu.h"

SDL_Texture* init_texture(const char* path){
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Texture *tmp_texture = NULL;
     SDL_Surface *tmp_surface = NULL;
     
     tmp_surface = IMG_Load(path);
     if(!tmp_surface){
	  printf("Failed loading tmp_surface! - %s\n ",SDL_GetError());
	  SDL_Log("Failed loading tmp_surface! - %s\n ");
	  return 0;
     }
     
     tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
     if(!tmp_texture){
     	  printf("Failed loading tmp_texture! - %s\n ", SDL_GetError());
	  SDL_Log("Failed loading tmp_surface! - %s\n ");
     	  SDL_FreeSurface(tmp_surface);
     	  return NULL;
     }

     SDL_FreeSurface(tmp_surface);
     return tmp_texture;
}


TTF_Font* init_font(const char* path, int size){
     TTF_Font* tmp_font = TTF_OpenFont(path, size);
     if(!tmp_font){
	  printf("Failed opening font! - %s\n",TTF_GetError());
	  SDL_Log("Failed loading tmp_surface! - %s\n ");
	  tmp_font =  NULL;
     }
     return tmp_font;
}


Main_Menu* init_menu(){
     Main_Menu* menu = new Main_Menu;
     if(!menu) return NULL;

     menu->button_texture = init_texture(button_texture_path);
     if(!menu->button_texture){
	  SDL_Log("Failed loading textuere! - %s\n ", SDL_GetError());
	  free(menu);
	  return NULL;
     }

     menu->background_texture = init_texture(menu_background_path);
     if(!menu->background_texture){
     	  SDL_Log("Failed loading textuere! - %s\n ", SDL_GetError());
     	  SDL_DestroyTexture(menu->button_texture);
     	  free(menu);
     	  return NULL;
     }

     menu->font = init_font(font_path, font_size);
     if(!menu->font){
     	  printf("failed opening ttf_font - %s\n",TTF_GetError());
     	  SDL_Log("Failed loading font! - %s\n ", TTF_GetError());
     	  SDL_DestroyTexture(menu->button_texture);
     	  free(menu);
     	  return NULL;
     }

     menu->level_font = init_font(font_path, font_size - 10);
     if(!menu->level_font){
     	  printf("failed opening ttf_font - %s\n",TTF_GetError());
     	  SDL_Log("Failed loading font! - %s\n ", TTF_GetError());
     	  TTF_CloseFont(menu->font);
     	  SDL_DestroyTexture(menu->button_texture);
     	  free(menu);
     	  return NULL;
     }

     int width = Window_Info::get_width();
     int height = Window_Info::get_height();

     int w = 300;
     int h = 100;
     int x = (width >> 1) - (w>>1);
     int y = (height / 3) - (h >> 1);

     menu->buttons.push_back({x, y, w, h, "Start"});
     menu->buttons.push_back({x, y + h, w, h, "Settings"});
     menu->buttons.push_back({x, y + (h<<1), w, h, "Exit"});

     menu->mouse_left_button = false;

     menu->state = MENU::MAIN;
     menu->hit_button = -1;

     update_menu(menu);
     SDL_RenderClear(Window_Info::get_renderer());

     menu->background_area.x = 0;
     menu->background_area.y = 0;
     menu->background_area.w = width;
     menu->background_area.h = height;
     return menu;
}

void destroy_menu(Main_Menu *menu){
     if(menu->button_texture) SDL_DestroyTexture(menu->button_texture);
     if(menu->background_texture) SDL_DestroyTexture(menu->background_texture);
     if(menu->font) TTF_CloseFont(menu->font);
     if(menu->level_font) TTF_CloseFont(menu->level_font);
     // if(menu->buttons) free(menu->buttons);
     if(menu) delete menu;
     menu = NULL;
}

int check_button(int x, int y, std::vector<Button> &buttons){
     for (int i = 0; i < buttons.size(); i++) {
	  if(x < buttons[i].pos.x)
	       continue;
	  else if(x > buttons[i].pos.x + buttons[i].pos.w)
	       continue;
	  else if(y < buttons[i].pos.y)
	       continue;
	  else if(y > buttons[i].pos.y + buttons[i].pos.h)
	       continue;
	  else{
	       return i;
	  }
     }
     return -1;
}


void return_to_main(Main_Menu *menu){
     if(!menu->buttons.empty())
	  menu->buttons.clear();


     int width = Window_Info::get_width();
     int height = Window_Info::get_height();

     int w = 300;
     int h = 100;
     int x = (width >> 1) - (w>>1);
     int y = (height / 3) - (h >> 1);

     menu->buttons.push_back({x, y, w, h, "Start"});
     menu->buttons.push_back({x, y + h, w, h, "Settings"});
     menu->buttons.push_back({x, y + (h<<1), w, h, "Exit"});
     menu->state = MAIN;
}

void select_layer(std::vector<Button> &buttons, std::vector<std::string> &text){
     if(!buttons.empty())
	  buttons.clear();
     
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();

     int w = 300;
     int h = 100;
     int x = (width >> 1) - (w>>1);
     int y = (height / 3) - (h >> 1);

     for (int i = 0; i < text.size(); i++) {
	  buttons.push_back({x, y +(h*i), w, h, text[i]});
     }


     // menu->buttons.push_back({x, y, w, h, "Start"});
     // menu->buttons.push_back({x, y + h, w, h, "Settings"});
     // menu->buttons.push_back({x, y + (h<<1), w, h, "Exit"});
}

// that's because MinGW still didn't fix that but with std::to_string
#include <sstream>
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

void init_level_buttons(Main_Menu *menu, int start){
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();

     menu->buttons[1].pos.x = (width >> 1) - (menu->buttons[1].pos.w>>1);
     menu->buttons[1].pos.y = height - (menu->buttons[1].pos.h);

     menu->buttons[0].pos.w = 200;
     menu->buttons[0].pos.h = 100;
     menu->buttons[0].pos.x = 0;
     menu->buttons[0].pos.y = menu->buttons[1].pos.y - menu->buttons[0].pos.h;
     menu->buttons[0].text = "Back";

     menu->buttons[2].pos.w = 200;
     menu->buttons[2].pos.h = 100;
     menu->buttons[2].pos.x = width - (menu->buttons[2].pos.w);
     menu->buttons[2].pos.y = menu->buttons[1].pos.y - menu->buttons[2].pos.h;
     menu->buttons[2].text = "Next";

     int buttons_amount = 0;
     if(start < lvl_amount)
	  buttons_amount = (lvl_amount - start) % 20;

     printf("buttons_amount = %d\n",buttons_amount);

     int block_size = 90;
     int start_x = (width >> 1) - (block_size * 4)/2;
     int start_y = (height / 3) - (block_size * 5)/2;

     int pitch_y = -1;
     int index = start;
     int start_index = 0;
     
     Document doc;
     read_bin(doc);
     while(buttons_amount != 0){
	  Level_Info lvl;
	  read_level_info(lvl, index+1);

	  Button button;
	  button.text = patch::to_string(index+1);
	  button.status = lvl.status;
	  if(button.status)
	       start_index = index;
	  button.time  = lvl.time;

	  if(index % 4 == 0) pitch_y++;
	  
	  button.pos.w = button.pos.h = block_size;
	  button.pos.x = start_x + ((index % 4) * block_size);
	  button.pos.y = start_y + (pitch_y * block_size);
	  menu->buttons.push_back(button);

	  index++;
	  buttons_amount--;
     }

     std::string tmp_string = "Start Level #";
     tmp_string.append(patch::to_string(start_index+1));
     
     menu->buttons[1].text = tmp_string.c_str();
}

int handle_event(Main_Menu* menu, SDL_Event *event){
     if(event->type == SDL_QUIT)
	  return GAME_EXIT;

     if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
	  if(!menu->mouse_left_button){
	       printf("mouse down\n");
	       menu->mouse_left_button = true;
	       
	       int result = check_button(event->button.x,
					 event->button.y,
					 menu->buttons);
	       if(result != -1){
		    if(result > 2){
			 if(menu->buttons[result].status)
			      menu->hit_button = result;
		    }
		    else
			 menu->hit_button = result;
	       }
	  }
     }
     else if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT){
	  if(menu->mouse_left_button){
	       printf("mouse up\n");
	       menu->mouse_left_button = false;
	       if(menu->hit_button != -1){
		    
		    switch(menu->state){
			 case MAIN:{
			      switch(menu->hit_button){
				   case 0:{
					init_level_buttons(menu, 0);
					menu->state = START;
				   } break;
				   case 1:{
					std::vector<std::string> tmp = {"Music: On", "Sound: On", "Back"};
					select_layer(menu->buttons, tmp);
					menu->state = SETTINGS;
				   } break;
				   case 2:{
					std::vector<std::string> tmp = {"Yes", "No"};
					select_layer(menu->buttons, tmp);
					menu->state = EXIT;
				   } break;
			      }
			 } break;
			 case START:{
			      if(menu->hit_button == 0){
				   std::vector<std::string> tmp = {"Start", "Settings", "Exit"};
				   select_layer(menu->buttons, tmp);
				   menu->state = MAIN;
			      }
			      else if(menu->hit_button == 2){
				   
			      }
			      else if(menu->hit_button == 1){
				   
			      }
			      else{
				   int index = menu->hit_button;
				   menu->hit_button = -1;
				   return atoi(menu->buttons[index].text.c_str());
			      }
			 } break;
			 case SETTINGS:{
			      switch(menu->hit_button){
				   case 0:{
				   
				   }break;
				   case 1:{
				   
				   } break;
				   case 2:{
					std::vector<std::string> tmp = {"Start", "Settings", "Exit"};
					select_layer(menu->buttons, tmp);
					menu->state = MAIN;
				   } break;
			      }
			 } break;
			 case EXIT:{
			      switch(menu->hit_button){
				   case 0:{
					return GAME_EXIT;
				   } break;
				   case 1:{
					std::vector<std::string> tmp = {"Start", "Settings", "Exit"};
					select_layer(menu->buttons, tmp);
					menu->state = MAIN;
				   } break;
			      }
			 } break;
			 
		    }

		    printf("sas\n");
		    menu->hit_button = -1;
	       }
	  }
     }
     else if(event->type == SDL_MOUSEMOTION){
	  if(menu->hit_button != -1){
	       int index = menu->hit_button;
	       int x = event->button.x;
	       int y = event->button.y;
	       if(x < menu->buttons[index].pos.x) menu->hit_button = -1;
	       else if(x > menu->buttons[index].pos.x + menu->buttons[index].pos.w) menu->hit_button = -1;
	       else if(y < menu->buttons[index].pos.y) menu->hit_button = -1;
	       else if(y > menu->buttons[index].pos.y + menu->buttons[index].pos.h) menu->hit_button = -1;
	       else{}
	  }
     }
     return MENU_OK;
}

void draw_button(Button &button, TTF_Font *& font, uint8_t r, uint8_t g, uint8_t b, bool level){
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();

     SDL_Color color = {r, g, b};

     const char *c_string = button.text.c_str();
     SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, c_string, color);
     if(!tmp_surface){
	  printf("Failed at Menu::draw_button() - %s\n", TTF_GetError());
	  return;
     }

     SDL_Rect rect;
     rect.w = tmp_surface->w;
     rect.h = tmp_surface->h;
     rect.x = button.pos.x + (button.pos.w>>1) - (rect.w>>1);
     if(level)
	  rect.y = button.pos.y + (button.pos.h>>1) - (rect.h>>1);
     else
	  rect.y = button.pos.y + (button.pos.h>>1) - (rect.h>>1);

     SDL_Texture *tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
     if(!tmp_texture){
	  printf("Failed at Menu::draw_button() - %s\n", IMG_GetError());
	  SDL_FreeSurface(tmp_surface);
	  return;
     }

     SDL_RenderCopy(RenderScreen, tmp_texture, 0, &rect);

     SDL_FreeSurface(tmp_surface);
     SDL_DestroyTexture(tmp_texture);

}


void draw_level_info(Button &button, Uint32 milliseconds, TTF_Font *&font, bool active){
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();


     SDL_Color color;
     if(active) color = {255, 0, 0};
     else color = {255, 255, 255};

     std::stringstream time_text;
     time_text.str("");
     Uint32 seconds = (milliseconds / 1000) % 60;
     Uint32 minutes = (milliseconds / 1000) / 60;
     if(minutes < 10) time_text << "0" << minutes << ":";
     else time_text << minutes << ";";
     if(seconds < 10) time_text << "0" << seconds;
     else time_text << seconds;

     SDL_Surface *number_surface = TTF_RenderUTF8_Blended(font, button.text.c_str(), color);
     SDL_Surface *time_surface = TTF_RenderUTF8_Blended(font, time_text.str().c_str(), {0, 0, 0});
     
     if(!number_surface || !time_surface){
	  printf("Failed at loading surface Menu::draw_level_info() - %s\n", TTF_GetError());
	  return;
     }

     SDL_Texture *number_texture = SDL_CreateTextureFromSurface(RenderScreen, number_surface);
     SDL_Texture *time_texture = SDL_CreateTextureFromSurface(RenderScreen, time_surface);
     if(!number_texture || !time_texture){
	  printf("Failed at loading texture Menu::draw_level_info() - %s\n", IMG_GetError());
	  SDL_FreeSurface(number_surface);
	  SDL_FreeSurface(time_surface);
	  return;
     }

     SDL_Rect rect;
     rect.w = number_surface->w;
     rect.h = number_surface->h;
     rect.x = button.pos.x + (button.pos.w>>1) - (rect.w>>1);
     rect.y = button.pos.y + (button.pos.h>>2) - (rect.h>>1);

     SDL_RenderCopy(RenderScreen, number_texture, 0, &rect);

     rect.w = time_surface->w;
     rect.h = time_surface->h;
     rect.x = button.pos.x + (button.pos.w>>1) - (rect.w>>1);
     rect.y = button.pos.y + (button.pos.h>>1) - (rect.h>>1);
     SDL_RenderCopy(RenderScreen, time_texture, 0, &rect);

     SDL_FreeSurface(number_surface);
     SDL_FreeSurface(time_surface);
     
     SDL_DestroyTexture(number_texture);
     SDL_DestroyTexture(time_texture);

}

void update_menu(Main_Menu *menu){
     if(menu == NULL)
	  return;

     SDL_Renderer *RenderScreen = Window_Info::get_renderer();

     // SDL_RenderCopy(RenderScreen, menu->background_texture, 0, &menu->background_area);
     
     for (int i = 0; i < menu->buttons.size(); i++) {
	  SDL_RenderCopy(RenderScreen, menu->button_texture, 0, &menu->buttons[i].pos);
	  
	  if(!menu->buttons[i].text.empty()){
	       if(i < 3){
		    if(i == menu->hit_button)
		    	 draw_button(menu->buttons[i], menu->font, 255, 0, 0, false);
		    else
		    	 draw_button(menu->buttons[i], menu->font, 0, 0, 0, false);
	       }
	       else{
		    if(menu->buttons[i].status == 1){
		    	 bool active = i == menu->hit_button ? true : false;
		    	 draw_level_info(menu->buttons[i], menu->buttons[i].time, menu->level_font, active);
		    }
		    else{
		    	 draw_button(menu->buttons[i], menu->font, 0, 0, 0, false);
		    }
	       }
	  }
     }
}
