#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Grid_Manager.h"

struct Grid_Editor
{
     Grid_Editor(TTF_Font*& Font, Grid_Manager* grid);
     ~Grid_Editor();
     
     void HandleEvent(SDL_Event *event);
     void RenderEditor();
     
     void UpdateRow(int);
     void UpdateColumn(int);
     
     void InitTexture(SDL_Texture*&, SDL_Rect*, const char*);
     
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
     Grid_Manager *grid; 

     bool MousePressed        = false;
     
     bool MinusRowPressed      = false;
     bool PlusRowPressed      = false;
     bool MinusColumnPressed  = false;
     bool PlusColumnPressed   = false; 
};


#endif
