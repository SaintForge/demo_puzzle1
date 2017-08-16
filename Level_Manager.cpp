#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <iostream>

#include "Level_Manager.h"

Level_Manager::Level_Manager(){
     printf("Level_Manager()::Level_Manager()\n");
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();
     
     grid_area.x = 0; grid_area.y = 0;
     grid_area.w = width;
     grid_area.h = height;

     int unit_width = grid_manager.get_block_size();
     
     figure_area = figure_manager.get_idle_zone();

     menu_bar_area.x = 0; menu_bar_area.y = 0;
     menu_bar_area.w = width;
     menu_bar_area.h = unit_width*2;

     exit_button.w = 50;
     exit_button.h = 50;
     // exit_button.x = (menu_bar_area.w >> 1) - (exit_button.w >> 1);
     // exit_button.y = (menu_bar_area.h >> 1) - (exit_button.h >> 1);
     exit_button.x = 0 + (exit_button.w>>1);
     exit_button.y = 0 + (exit_button.h>>1);

     restart_button.w = 50;
     restart_button.h = 50;
     restart_button.x = width - (restart_button.w) - (restart_button.w>>1);
     restart_button.y = 0 + (restart_button.h>>1);

#ifdef _WIN32
     load_image(menu_exit_texture, "..\\data\\sprites\\exit_button2.png");
     load_image(menu_restart_texture, "..\\data\\sprites\\restart_button2.png");

     begin_sound = Mix_LoadWAV("..\\data\\sound\\focus_enter_new.wav");
     complete_sound_1 = Mix_LoadWAV("..\\data\\sound\\menu_enter1-cut.wav");
     complete_sound_2 = Mix_LoadWAV("..\\data\\sound\\menu_enter2.wav");
     font = TTF_OpenFont("..\\data\\Karmina-Bold.otf", 50);
#else
     load_image(menu_exit_texture, "data/sprites/exit_button2.png");
     load_image(menu_restart_texture, "data/sprites/restart_button2.png");

     begin_sound = Mix_LoadWAV("data/sound/focus_enter_new.wav");
     complete_sound_1 = Mix_LoadWAV("data/sound/menu_enter1-cut.wav");
     complete_sound_2 = Mix_LoadWAV("data/sound/menu_enter2.wav");
     font = TTF_OpenFont("data/Karmina-Bold.otf", 50);
#endif
     
     Mix_VolumeChunk(begin_sound, 0);
     Mix_VolumeChunk(complete_sound_1, 10);
     Mix_VolumeChunk(complete_sound_2, 10);

     // music = Mix_LoadMUS("..\\data\\sound\\windchime_chiming.wav");

     // if(!music)
     // 	  printf("Failed loading the music = %s\n", Mix_GetError());

     // Mix_PlayMusic(music, -1);
     // Mix_VolumeMusic(5);
     // Mix_FadeInMusic(music, -1, 10000);

}

Level_Manager::~Level_Manager(){
     
     if(begin_sound) Mix_FreeChunk(begin_sound);
     if(complete_sound_1) Mix_FreeChunk(complete_sound_1);
     if(complete_sound_2) Mix_FreeChunk(complete_sound_2);
     if(music) Mix_FreeMusic(music);
}

void Level_Manager::load_image(SDL_Texture *&sprite, const char* path){
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     SDL_Surface* tmp_surface;
     SDL_Texture* tmp_texture;
     
     if(sprite)
	  SDL_DestroyTexture(sprite);

     tmp_surface = IMG_Load(path);
     if(tmp_surface){ 
	  tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
	  if(tmp_texture){
	       sprite = tmp_texture;
	       SDL_SetTextureBlendMode(sprite, SDL_BLENDMODE_BLEND);
	       SDL_FreeSurface(tmp_surface);
	  }
	  else{
	       SDL_FreeSurface(tmp_surface);
	       printf("failed to load a texture from the surface! %s\n", SDL_GetError());
	       return;
	  }
     }
     else{
	  printf("failed to load a picture with a given path! - %s\n", IMG_GetError());
	  return;
     }
}

void Level_Manager::next_level(Level_Info &info, int level_number){
     figure_manager.change_figures(info.figure_type, info.figure_angle);
     
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();
     
     int x = width>>1;
     int y = menu_bar_area.h + (height - figure_manager.get_idle_zone().h)>>1;
     grid_manager.update_grid(&figure_manager, x, y, info);
     
     level_complete = false;
     alpha = 255;

     Mix_PlayChannel( -1, begin_sound, 0 );
     start_time = SDL_GetTicks();
     
     this->level_number = level_number;

     load_level_number(lvl_number_texture, {255, 255, 255});
     load_level_number(lvl_number_shadow_texture, {0, 0, 0});

     alpha_level_number = 0;
     SDL_SetTextureAlphaMod(lvl_number_texture, alpha_level_number);
     SDL_SetTextureAlphaMod(lvl_number_shadow_texture, alpha_level_number);

     level_line[0].h = level_line[1].h = 2;
     
     level_line[0].w = 10;
     level_line[1].w = 10;

     level_line[0].y = level_line[1].y = level_number_rect.y + level_number_rect.h/2;

     level_line[0].x = (menu_bar_area.w / 2) - (active_block_size+level_line[0].w);
     level_line[1].x = (menu_bar_area.w / 2) + (active_block_size);

     level_number_animation = true;
}

void Level_Manager::update_level_number_animation()
{
    if(level_number_animation)
    {
	int margin = default_block_size;
	bool left_stop  = false;
	bool right_stop = false;
	bool alpha_stop  = false;
	
	if(level_line[0].x > margin)
	{
	    level_line[0].x -=2;
	    level_line[0].w +=2;
	}
	else left_stop = true;
	    

	if(level_line[1].x + level_line[1].w < Window_Info::get_width() - margin)
	{
	    level_line[1].w += 2;
	}
	else right_stop = true;

	if(alpha_level_number < 250)
	{
	    alpha_level_number += 3;

	}
	else
	{
	    alpha_level_number = 255;
	    alpha_stop = true;
	}

	SDL_SetTextureAlphaMod(lvl_number_texture, alpha_level_number);
	SDL_SetTextureAlphaMod(lvl_number_shadow_texture, alpha_level_number);

	if(left_stop && right_stop && alpha_stop)
	    level_number_animation = false;
    }
}

void Level_Manager::load_level_number(SDL_Texture*& texture, SDL_Color color){
    if(texture)
	SDL_DestroyTexture(texture);

    SDL_Renderer *RenderScreen = Window_Info::get_renderer();
    SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font,
						      std::to_string(level_number).c_str(),
						      color);
    if(!tmp_surface){
	printf("Failed at loading surface at Level_Manager::load_level_number - %s", SDL_GetError());
	return;
    }

    level_number_rect.w = tmp_surface->w;
    level_number_rect.h = tmp_surface->h;
    level_number_rect.x = menu_bar_area.x + (menu_bar_area.w / 2) - (level_number_rect.w/2);
    level_number_rect.y = menu_bar_area.y + (menu_bar_area.h / 2) - (level_number_rect.h/2);
    
    texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
    if(!lvl_number_texture){
	printf("Failed at loading texture at Level_Manager::load_level_number - %s", SDL_GetError());
	SDL_FreeSurface(tmp_surface);
	return;
    }

    SDL_FreeSurface(tmp_surface);
}

int Level_Manager::handle_event(SDL_Event &event){
     if(!level_complete)
	  figure_manager.handle_event(event);
     if(event.key.keysym.sym == SDLK_AC_BACK)
	  return LEVEL_EXIT;
     if(event.type == SDL_QUIT)
	  return LEVEL_EXIT;
     if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
	  return LEVEL_EXIT;

     if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
	  if(!figure_manager.is_grabbed()){
	       
	       int x = event.button.x;
	       int y = event.button.y;
	       if(x < exit_button.x){}
	       else if(x > exit_button.x + exit_button.w){}
	       else if(y < exit_button.y){}
	       else if(y > exit_button.y + exit_button.h){}
	       else{
		    printf("LEVEL_EXIT\n");
		    return LEVEL_EXIT;
	       }

	       if(x < restart_button.x){}
	       else if(x > restart_button.x + restart_button.w){}
	       else if(y < restart_button.y){}
	       else if(y > restart_button.y + restart_button.h){}
	       else{
		    printf("RESTART\n");
		    figure_manager.restart();
		    grid_manager.restart_grid();
	       }
	  }
     }
     
     return LEVEL_INCOMPLETE;
}

int Level_Manager::update(){
     int result = LEVEL_INCOMPLETE;
     int grid_state;

     if(!level_complete){
	  figure_manager.update();
	  if(alpha > 2) alpha -= 5;
	  grid_state = grid_manager.update();
	  if(grid_state == GRID_FULL){
	       level_complete = true;
	       
	       if(toogle_sound_completion){
		    Mix_PlayChannel( -1, complete_sound_1, 0 );
		    toogle_sound_completion = false;
	       }
	       else{
		    Mix_PlayChannel( -1, complete_sound_2, 0 );
		    toogle_sound_completion = true;
	       }
	       
	  }
	  else if(grid_state == GRID_HIT){
	       int index = grid_manager.get_last_stick_index();
	       if(index != -1)
		    figure_manager.toogle_stick_effect(index);
	  }
     }
     else{
	  if(alpha < 252){
	       alpha += 3;
	  }
	  else{ result = LEVEL_COMPLETE;}
     }

     update_level_number_animation();

     return result;
}

void Level_Manager::draw(){
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     
     SDL_SetRenderDrawColor(RenderScreen, 42, 6, 21, 255);
     // SDL_SetRenderDrawColor(RenderScreen, 71, 19, 29, 255);
     SDL_RenderFillRect(RenderScreen, NULL);
     
     grid_manager.draw();
     figure_manager.draw();

     // draw_timer(255, 255, 255);

     // SDL_RenderCopy(RenderScreen, menu_exit_texture, 0, &exit_button);
     // SDL_RenderCopy(RenderScreen, menu_restart_texture, 0, &restart_button);
     
     {
	 SDL_Rect rect = level_number_rect;
	 rect.x += 5;
	 rect.y += 5;
	 SDL_RenderCopy(RenderScreen, lvl_number_shadow_texture, 0, &rect);
	 SDL_RenderCopy(RenderScreen, lvl_number_texture, 0, &level_number_rect);
     }
     
     // SDL_SetRenderDrawColor(RenderScreen, 255, 255, 255, 255);
     // SDL_RenderDrawRect(RenderScreen, &level_number_rect);
     {
	 SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
	 SDL_Rect rect = level_line[0];
	 rect.x += 3; rect.y += 3;
	 SDL_RenderFillRect(RenderScreen, &rect);
	 rect = level_line[1];
	 rect.x += 3; rect.y += 3;
	 SDL_RenderFillRect(RenderScreen, &rect);

	 SDL_SetRenderDrawColor(RenderScreen, 255, 255, 255, 255);
	 SDL_RenderFillRect(RenderScreen, &level_line[0]);
	 SDL_RenderFillRect(RenderScreen, &level_line[1]);
	 SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
     }



     if(!level_complete){
	 if(alpha > 5){
	     SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, alpha);
	     SDL_RenderFillRect(RenderScreen, NULL);
	 }
     }
     else{
	 if(alpha < 255){
	     SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, alpha);
	     SDL_RenderFillRect(RenderScreen, NULL);
	 }
     }
}

uint32_t Level_Manager::total_time(){
     return end_time - start_time;
}
