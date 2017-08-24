#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Figure_Editor.h"
#include "Grid_Editor.h"
#include "Figure_Manager.h"
#include "Grid_Manager.h"

struct Level_Editor
{
     Level_Editor(TTF_Font *&, Figure_Manager*, Grid_Manager*);
     ~Level_Editor();

     SDL_Rect EditorBar; 
     SDL_Rect SaveButtonQuad;
     SDL_Rect ReloadButtonQuad;

     SDL_Texture *SaveButtonTexture   = NULL;
     SDL_Texture *ReloadButtonTexture = NULL; 

     Grid_Editor   *GridEditor;
     Figure_Editor *FigureEditor;

     bool LeftMousePressed    = false;

     bool SaveButtonPressed   = false;
     bool ReloadButtonPressed = false; 

     void HandleEvents(SDL_Event * Event);
     void RenderEditor();
};


#endif
