#include <SDL.h>

#include <stdio.h>

#include "Game_Data.h"
#include "Window.h"

Game_Data::Game_Data(){
     printf("Game_Data()\n");
#ifdef PC
     ITexture = load_image(ITexture, "..\\data\\sprites\\i_white_2.png");
     OTexture = load_image(OTexture, "..\\data\\sprites\\o_white_2.png");
     ZTexture = load_image(ZTexture, "..\\data\\sprites\\z_white_2.png");
     STexture = load_image(STexture, "..\\data\\sprites\\s_white_2.png");
     TTexture = load_image(TTexture, "..\\data\\sprites\\t_white_2.png");
     LTexture = load_image(LTexture, "..\\data\\sprites\\l_white_2.png");
     JTexture = load_image(JTexture, "..\\data\\sprites\\j_white_2.png");
#else
     ITexture = load_image(ITexture, "data/sprites/i_white_2.png");
     OTexture = load_image(OTexture, "data/sprites/o_white_2.png");
     ZTexture = load_image(ZTexture, "data/sprites/z_white_2.png");
     STexture = load_image(STexture, "data/sprites/s_white_2.png");
     TTexture = load_image(TTexture, "data/sprites/t_white_2.png");
     LTexture = load_image(LTexture, "data/sprites/l_white_2.png");
     JTexture = load_image(JTexture, "data/sprites/j_white_2.png");
#endif
}

Game_Data::~Game_Data(){
     printf("~Game_Data()\n");
}

void Game_Data::Destroy_Texture(SDL_Texture *&texture){
     if(texture)
	  SDL_DestroyTexture(texture);
}

SDL_Texture*
Game_Data::load_image(SDL_Texture *&texture, const char* path){
     SDL_Renderer* RenderScreen = Window_Info::get_window_info().get_renderer();
     SDL_Surface* tmp_surface;
     SDL_Texture* tmp_texture;

     if(texture)
	  SDL_DestroyTexture(texture);
     
     tmp_surface = IMG_Load(path);
     if(tmp_surface){
	  tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
	  if(tmp_texture){
	       texture = tmp_texture;
	       SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	       SDL_FreeSurface(tmp_surface);
	       return texture;
	  }
	  else{
	       SDL_FreeSurface(tmp_surface);
	       printf("failed to load a texture from the surface! %s\n", SDL_GetError());
	       return NULL;
	  }
     }
     else{
	  printf("failed to load a picture with a given path! - %s\n", IMG_GetError());
	  return NULL;
     }
}


SDL_Texture* Game_Data::load_ITexture(){
     return ITexture;
}
SDL_Texture* Game_Data::load_OTexture(){
     return OTexture;
}
SDL_Texture* Game_Data::load_ZTexture(){
     return ZTexture;
}
SDL_Texture* Game_Data::load_STexture(){
     return STexture;
}
SDL_Texture* Game_Data::load_TTexture(){
     return TTexture;
}
SDL_Texture* Game_Data::load_LTexture(){
     return LTexture;
}
SDL_Texture* Game_Data::load_JTexture(){
     return JTexture;
}
