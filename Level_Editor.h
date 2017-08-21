#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

struct Level_Editor
{
     Level_Editor(int RowNumber, int ColumnNumber);
     ~Level_Editor();
     
     void HandleEvent();
     void Render();
     void Update();
private:
     void InitFont();
     void InitNumberTexture(SDL_Texture*& Texture, SDL_Rect *TextureRect, int Number);
     void InitTexture(SDL_Texture*&, const char*);
private:
     int RowNumber;
     int ColumnNumber;

     SDL_Rect EditorBar;

     SDL_Texture *MinusSignTexture    = NULL;
     SDL_Texture *PlusSignTexture     = NULL;
     SDL_Texture *ColumnNumberTexture = NULL;
     SDL_Texture *RowNumberTexture    = NULL;

     TTF_Font *Font = NULL;
};


#endif
