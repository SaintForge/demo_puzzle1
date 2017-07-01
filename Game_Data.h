#ifndef MEDIA
#define MEDIA

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Game_Data{
public:
     static Game_Data& get_media(){
	  static Game_Data game_data;
	  return game_data;
     }
     
     SDL_Texture* load_ITexture();
     SDL_Texture* load_OTexture();
     SDL_Texture* load_ZTexture();
     SDL_Texture* load_STexture();
     SDL_Texture* load_TTexture();
     SDL_Texture* load_LTexture();
     SDL_Texture* load_JTexture();

     void Destroy_Texture(SDL_Texture *&);
private:
     Game_Data();
     ~Game_Data();
     SDL_Texture* load_image(SDL_Texture *&texture, const char* path);
private:
     SDL_Texture* ITexture = nullptr;
     SDL_Texture* OTexture = nullptr;
     SDL_Texture* ZTexture = nullptr;
     SDL_Texture* STexture = nullptr;
     SDL_Texture* TTexture = nullptr;
     SDL_Texture* LTexture = nullptr;
     SDL_Texture* JTexture = nullptr;
};

#endif
