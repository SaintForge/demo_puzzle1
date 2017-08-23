#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Figure_Editor.h"
// #include "Figure_Manager.h"


Figure_Editor::Figure_Editor(TTF_Font *& Font, Figure_Manager* FigureManager)
{
     printf("Figure_Editor::Figure_Editor()\n");
     
     if(!Font)
     {
	  printf("Passing null Font in Figure_Editor::Figure_Editor()\n");
	  return;
     }

     this->Font = Font;
     this->FigureManager = FigureManager;
     this->FigureManager->DeveloperMode = true;
     this->FigureManager->SelectedFigure= 0;

     InitTexture(NewFigureTexture, &NewFigureQuad, "+");
     InitTexture(DelFigureTexture, &DelFigureQuad, "-");
     InitTexture(RotateFigureTexture, "..\\data\\sprites\\restart_button2.png", &RotateFigureQuad);
     InitTexture(FlipFigureTexture, "..\\data\\sprites\\flip_button.png" ,  &FlipFigureQuad );
     InitTexture(NewTypeTexture, &NewTypeQuad, ">");
     InitTexture(NewFormTexture, &NewFormQuad, "<");


     int width = Window_Info::get_width();
     EditorBar.w = active_block_size * 6;
     EditorBar.h = active_block_size;
     EditorBar.x = 0;
     EditorBar.y = active_block_size;
}

Figure_Editor::~Figure_Editor()
{
     printf("~Figure_Editor::Figure_Editor()\n");

     this->FigureManager->DeveloperMode = false;

     if(NewFigureTexture) SDL_DestroyTexture(NewFigureTexture);
     if(DelFigureTexture) SDL_DestroyTexture(DelFigureTexture);
     if(NewTypeTexture) SDL_DestroyTexture(NewTypeTexture);
     if(NewFormTexture) SDL_DestroyTexture(NewFormTexture);
     if(RotateFigureTexture) SDL_DestroyTexture(RotateFigureTexture);
     if(FlipFigureTexture) SDL_DestroyTexture(FlipFigureTexture);
}

void Figure_Editor::InitTexture(SDL_Texture*& Texture, const char* Path, SDL_Rect* Quad)
{
     if(!Path) return;
     if(Texture)
     {
	  SDL_DestroyTexture(Texture);
     }

     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     SDL_Surface* tmp_surface = IMG_Load(Path);
     if(!tmp_surface)
     {
	  printf("Failed to init SDL_Surface at Figure_Editor::InitTexture() - %s\n", IMG_GetError());
	  return;
     }

     Quad->w = active_block_size;
     Quad->h = active_block_size;

     Texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
     if(!Texture)
     {
	  printf("Failed to init SDL_Texture at Figure_Editor::InitTexture = %s", SDL_GetError() );
     }

     SDL_FreeSurface(tmp_surface);
}

void Figure_Editor::InitTexture(SDL_Texture*& Texture, SDL_Rect* Quad, const char* Text)
{
     if(!Font) return;
     if(!Text) return;
     if(Texture)
     {
	  SDL_DestroyTexture(Texture);
     }
     
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(Font,  Text,  {0, 0, 0});
     if(!tmp_surface)
     {
	  printf("Failed to init SDL_Surface at Figure_Editor::InitTexture = %s", TTF_GetError() );
	  return;
     }

     Quad->w = tmp_surface->w;
     Quad->h = tmp_surface->h; 

     Texture = SDL_CreateTextureFromSurface(RenderScreen,  tmp_surface);
     if(!Texture)
     {
	  printf("Failed to init SDL_Texture at Figure_Editor::InitTexture = %s", SDL_GetError() );
     }

     SDL_FreeSurface(tmp_surface);
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

void Figure_Editor::HandleEvent(SDL_Event* event)
{
     if(event->type == SDL_MOUSEBUTTONDOWN
	&& event->button.button == SDL_BUTTON_RIGHT)
     {
	  if(!RightMousePressed)
	  {
	       int x_button = event->button.x;
	       int y_button = event->button.y;

	       int index = FigureManager->check_mouse_click(x_button, y_button);
	       if(index >= 0) FigureManager->select_figure(index);

	       RightMousePressed = true;
	  }
     }
     else if(event->type == SDL_MOUSEBUTTONUP
	     && event->button.button == SDL_BUTTON_RIGHT)
     {
	  if(RightMousePressed)
	  {
	       RightMousePressed = false;
	  }
     }
     else if(event->type == SDL_MOUSEBUTTONDOWN
	     && event->button.button == SDL_BUTTON_LEFT)
     {
	  if(!LeftMousePressed)
	  {
	       LeftMousePressed = true;
	       printf("MousePressed = true\n");

	       int x_button = event->button.x;
	       int y_button = event->button.y;

	       SDL_Rect TargetPosition = {};
	       TargetPosition.x = EditorBar.x;
	       TargetPosition.y = EditorBar.y;
	       TargetPosition.w = active_block_size;
	       TargetPosition.h = active_block_size;
	       
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("New Figure hit!\n");
		    FigureManager->add_new_figure(O_figure, classic);
	       }

	       TargetPosition.x += TargetPosition.w;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("Delete Figure hit!\n");
		    FigureManager->delete_figure();
	       }

	       TargetPosition.x += TargetPosition.w;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("changing form!\n");
		    FigureManager->change_figure_form();
	       }

	       TargetPosition.x += TargetPosition.w;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("changing type!\n");
		    FigureManager->change_figure_type();    
	       }

	       TargetPosition.x += TargetPosition.w;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("rotating button!\n");
		    FigureManager->change_figure_angle();
	       }

	       TargetPosition.x += TargetPosition.w;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    printf("flip the button \n");
		    FigureManager->change_figure_flip();
	       }
	  }
     }
     else if(event->type == SDL_MOUSEBUTTONUP
	     && event->button.button == SDL_BUTTON_LEFT)
     {
	  if(LeftMousePressed)
	  {
	       LeftMousePressed = false;
	       printf("MousePresed = false\n");
	  }
     }
}

static
void RenderUIElement(SDL_Texture*& Texture, SDL_Rect* TextureRect, SDL_Rect* UiRect)
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_RenderDrawRect(RenderScreen, UiRect);
     
     TextureRect->x = (UiRect->x + UiRect->w / 2 ) - (TextureRect->w / 2);
     TextureRect->y = (UiRect->y + UiRect->w / 2 ) - (TextureRect->h / 2);
     UiRect->x += UiRect->h;

     SDL_RenderCopy(RenderScreen, Texture, 0, TextureRect );
}

void Figure_Editor::RenderEditor()
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Rect TargetPosition = {EditorBar.x, EditorBar.y,
				active_block_size, active_block_size};

     SDL_SetRenderDrawColor(RenderScreen, 0, 255, 0, 255);
     SDL_RenderFillRect(RenderScreen, &EditorBar);
     SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
     SDL_RenderDrawRect(RenderScreen, &EditorBar);

     RenderUIElement(NewFigureTexture, &NewFigureQuad, &TargetPosition);
     RenderUIElement(DelFigureTexture, &DelFigureQuad, &TargetPosition);
     RenderUIElement(NewFormTexture, &NewFormQuad, &TargetPosition);
     RenderUIElement(NewTypeTexture, &NewTypeQuad, &TargetPosition);
     RenderUIElement(RotateFigureTexture, &RotateFigureQuad, &TargetPosition);
     RenderUIElement(FlipFigureTexture, &FlipFigureQuad, &TargetPosition);


}
