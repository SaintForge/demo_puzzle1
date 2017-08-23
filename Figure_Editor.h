#ifndef FIGURE_EDITOR_H
#define FIGURE_EDITOR_H

#include <SDL.h>
#include "Figure.h"
#include "Figure_Manager.h"

struct Figure_Editor
{
     TTF_Font *Font;
     SDL_Rect EditorBar;

     SDL_Rect NewFigureQuad;
     SDL_Rect DelFigureQuad;
     SDL_Rect NewTypeQuad;
     SDL_Rect NewFormQuad;
     SDL_Rect RotateFigureQuad;
     SDL_Rect FlipFigureQuad;

     SDL_Texture *NewFigureTexture    = NULL;
     SDL_Texture *DelFigureTexture    = NULL;
     SDL_Texture *NewTypeTexture      = NULL;
     SDL_Texture *NewFormTexture      = NULL;
     SDL_Texture *RotateFigureTexture = NULL;
     SDL_Texture *FlipFigureTexture   = NULL;
     
     Figure_Manager* FigureManager;

     bool FigureIsSelected  = false;
     bool LeftMousePressed  = false;
     bool RightMousePressed = false;

     Figure_Editor(TTF_Font*& Font, Figure_Manager* FigureManager);
     ~Figure_Editor();
     
     void InitTexture(SDL_Texture*& Texture, SDL_Rect* Target, const char* Text);
     void InitTexture(SDL_Texture*& Texture, const char* Path, SDL_Rect *Target);
     void HandleEvent(SDL_Event *event);
     void RenderEditor();
};

#endif
