#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

struct Level_Editor
{
     Level_Editor(TTF_Font*&,  int RowNumber, int ColumnNumber);
     ~Level_Editor();
     
     void HandleEvent();
     void Render();
     
     void UpdateRow(int);
     void UpdateColumn(int);
private:
     void InitTexture(SDL_Texture*&, SDL_Rect*, const char*);
private:
     int RowNumber;
     int ColumnNumber;

     SDL_Rect EditorBar;
     SDL_Rect ColumnNumberQuad;
     SDL_Rect RowNumberQuad;
     SDL_Rect MinusSignQuad;
     SDL_Rect PlusSignQuad; 
     
     SDL_Texture *ColumnNumberTexture = NULL;
     SDL_Texture *RowNumberTexture    = NULL;
     SDL_Texture *MinusSignTexture    = NULL;
     SDL_Texture *PlusSignTexture     = NULL;

     TTF_Font *Font = NULL; 
};


#endif
