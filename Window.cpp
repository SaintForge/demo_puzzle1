#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


#include <stdio.h>
#include <string.h>

#include "window.h"

Window_Info::Window_Info(){
     printf("Window_Info::Window_Info()\n");
     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

     // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

     if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
     {
	  printf("Failed to SDL_Init: %s", SDL_GetError());
	  return;
     }
     else
     {
	  int flags = IMG_INIT_PNG;
	  if(!(IMG_Init(flags) & flags))
	  {
	       printf("Failed to IMG_Init: %s", IMG_GetError());
	       return;
	  }
	  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
	       printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	       return;
	  }
	  if(TTF_Init()==-1){
	       printf("Failed to TTF_Init: %s", TTF_GetError());
	       return;
	  }
	  else
	  {
	       window = SDL_CreateWindow("Test",
					 100,
					 25,
					 window_width,
					 window_height,
					 SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI);
	       if(window)
	       {
		    const char* want_renderer = "opengl";
		    int numdrivers = SDL_GetNumRenderDrivers();
		    int driver_index;
		    SDL_RendererInfo info;// = malloc(sizeof(SDL_RendererInfo*));
		    for(driver_index=0;driver_index<numdrivers;driver_index++) {
			 SDL_GetRenderDriverInfo(driver_index,&info);
			 if(strcmp(info.name, want_renderer) == 0) { break; };
		    }
		    
		    renderer = SDL_CreateRenderer(window, driver_index, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
		    
		    if(renderer)
		    {
			 SDL_RendererInfo info;
			 if(SDL_GetRendererInfo(renderer, &info)){
			      printf("Renderer - %s\n", info.name);
			 }
			 else{
			      printf("Failed to read Renderer -%s\n",SDL_GetError());
			      printf("Renderer - %s\n", info.name);
			 }
			 if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			      printf("Hint was set!\n");
			 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			 SDL_RenderClear(renderer);
			 SDL_RenderPresent(renderer);
			 return;
		    }
		    else
		    {
			 printf("Failed to load Renderer: %s", SDL_GetError());
			 return;
		    }
	       }
	       else
	       {
		    printf("Failed to load Window: %s",SDL_GetError());
		    return;
	       }
	  }
     }
}

Window_Info::~Window_Info(){
     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
     IMG_Quit();
     Mix_Quit();
     TTF_Quit();
     SDL_Quit();
     printf("Window_Info::~Window_Info()\n");
}

SDL_Renderer* Window_Info:: get_renderer(){
     return renderer ? renderer : NULL;
}

SDL_Window* Window_Info::get_window(){
     return window ? window : NULL;
}

uint32_t Window_Info::get_width(){
     return window_width;
}

uint32_t Window_Info::get_height(){
     return window_height;
}

