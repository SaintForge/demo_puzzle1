#include <stdio.h>

#include "Level_Editor.h"


void InitTexture(TTF_Font *& Font,  SDL_Texture*& Texture, SDL_Rect* Quad, const char* Text)
{
     printf("InitTexture()\n");
     if(!Font) return;
     if(!Text) return;
     
     if(Texture)
     {
	  SDL_DestroyTexture(Texture);
     }
     
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(Font,  Text,  {255, 255, 255});
     if(!tmp_surface)
     {
	  printf("Failed to init SDL_Surface at Grid_Editor::InitNumberTexture = %s", TTF_GetError() );
	  return;
     }

     Quad->w = tmp_surface->w / 2;
     Quad->h = tmp_surface->h / 2; 

     Texture = SDL_CreateTextureFromSurface(RenderScreen,  tmp_surface);
     if(!Texture)
     {
	  printf("Failed to init SDL_Texture at Grid_Editor::InitNumberTexture = %s", SDL_GetError() );
     }

     SDL_FreeSurface(tmp_surface);
}


Level_Editor::Level_Editor(TTF_Font *& Font,
			   Figure_Manager * FigureManager, Grid_Manager * GridManager)
{
     printf("Level_Editor::Level_Editor()\n");

     if(!Font)
     {
	  printf("Passing null Font in Level_Editor::Level_Editor()\n");
	  return;
     }

     InitTexture(Font, SaveButtonTexture, &SaveButtonQuad, "Save" );
     InitTexture(Font, ReloadButtonTexture, &ReloadButtonQuad, "Reload" );

     GridEditor = new Grid_Editor(Font, GridManager ) ;
     FigureEditor = new Figure_Editor(Font, FigureManager ) ;

     int Width     = Window_Info::get_width();
     int BarWidth  = Width - FigureEditor->EditorBar.w;
     int BarHeight = FigureEditor->EditorBar.h + GridEditor->EditorBar.h;
     EditorBar.w = BarWidth;
     EditorBar.h = BarHeight;
     EditorBar.x = Width - EditorBar.w;
     EditorBar.y = 0;

     printf("end Level_Editor::Level_Editor()\n");
}

Level_Editor::~Level_Editor()
{
     printf("Level_Editor::~Level_Editor()\n");

     if(SaveButtonTexture) SDL_DestroyTexture(SaveButtonTexture) ;
     if(ReloadButtonTexture) SDL_DestroyTexture(ReloadButtonTexture) ;
     if(GridEditor) delete GridEditor;
     if(FigureEditor) delete FigureEditor;
}

static
bool ProcessMouseInput(SDL_Rect* TargetQuad, int x, int y)
{
     if(x < TargetQuad->x) return false ;
     else if(x > TargetQuad->x + TargetQuad->w) return false ;
     else if(y < TargetQuad->y) return false ;
     else if(y > TargetQuad->y + TargetQuad->h) return false;
     else return true;
}

void Level_Editor::HandleEvents(SDL_Event * event)
{
     if(event->type == SDL_MOUSEBUTTONDOWN
	&& event->button.button == SDL_BUTTON_LEFT)
     {
	  if(!LeftMousePressed)
	  {
	       LeftMousePressed = true;

	       int x_button = event->button.x;
	       int y_button = event->button.y;

	       SDL_Rect TargetPosition = {};
	       TargetPosition.x = EditorBar.x;
	       TargetPosition.y = EditorBar.y;
	       TargetPosition.w = EditorBar.w;
	       TargetPosition.h = EditorBar.h / 2;

	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("Save button click!\n ");
		    SaveButtonPressed = true;
		    return;
	       }
	       
	       TargetPosition.y += TargetPosition.h;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("Reload button click!\n");
		    ReloadButtonPressed = true;
		    return;
	       }
	  }
     }
     else if(event->type == SDL_MOUSEBUTTONUP
	     && event->button.button == SDL_BUTTON_LEFT)
     {
	  if(LeftMousePressed)
	  {
	       LeftMousePressed    = false;
	       SaveButtonPressed   = false;
	       ReloadButtonPressed = false;
	  }
     }

     GridEditor->HandleEvent(event);
     FigureEditor->HandleEvent(event);
}

static
void RenderUIElement(SDL_Texture*& Texture, SDL_Rect* TextureRect, SDL_Rect* UiRect, bool Pressed)
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     if(Pressed)
     {
	  SDL_SetRenderDrawColor(RenderScreen, 255, 255, 0, 255);
	  SDL_RenderFillRect(RenderScreen, UiRect );
	  SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
     }

     SDL_RenderDrawRect(RenderScreen, UiRect);
     
     TextureRect->x = (UiRect->x + UiRect->w / 2 ) - (TextureRect->w / 2);
     TextureRect->y = (UiRect->y + UiRect->h / 2 ) - (TextureRect->h / 2);
     UiRect->y += UiRect->h;

     SDL_RenderCopy(RenderScreen, Texture, 0, TextureRect );
}

void Level_Editor::RenderEditor()
{
     SDL_Renderer * RenderScreen = Window_Info::get_renderer();

     if(GridEditor) GridEditor->RenderEditor() ;
     if(FigureEditor) FigureEditor->RenderEditor() ;

     SDL_Rect TargetPosition = {};
     TargetPosition.x = EditorBar.x;
     TargetPosition.y = EditorBar.y;
     TargetPosition.w = EditorBar.w;
     TargetPosition.h = EditorBar.h / 2;
     
     SDL_SetRenderDrawColor(RenderScreen, 255, 0, 0, 255);
     SDL_RenderFillRect(RenderScreen, &EditorBar);
     SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
     
     RenderUIElement(SaveButtonTexture, &SaveButtonQuad, &TargetPosition, SaveButtonPressed );
     RenderUIElement(ReloadButtonTexture, &ReloadButtonQuad, &TargetPosition, ReloadButtonPressed );

     SDL_RenderDrawRect(RenderScreen, &EditorBar);
}
