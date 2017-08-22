#include <stdio.h>
#include <string>

#include "Window.h"
#include "Figure.h"
#include "Grid_Editor.h"

Grid_Editor::Grid_Editor(TTF_Font *& Font, Grid_Manager *grid )
{
     printf("Grid_Editor(int, int )\n");

     if(!Font)
     {
	  printf("Passing null Font in Grid_Editor::Grid_Editor()");
	  return;
     }

     RowNumber = grid->get_row_amount() ;
     ColumnNumber = grid->get_column_amount();

     this->Font = Font;
     this->grid = grid; 
     
     // InitFont();
     InitTexture(ColumnNumberTexture, &ColumnNumberQuad,std::to_string(ColumnNumber).c_str());
     InitTexture(RowNumberTexture, &RowNumberQuad, std::to_string(RowNumber).c_str());
     InitTexture(MinusSignTexture, &MinusSignQuad,"-");
     InitTexture(PlusSignTexture, &PlusSignQuad, "+");

     int width = Window_Info::get_width();
     EditorBar.w = active_block_size * 6;
     EditorBar.h = active_block_size;
     EditorBar.x = 0;
     EditorBar.y = 0;//(width >> 1) - (EditorBar.w>>1);
}

Grid_Editor::~Grid_Editor()
{
     printf("Grid_Editor::~Grid_Editor()\n");
     if(MinusSignTexture) SDL_DestroyTexture(MinusSignTexture);
     if(PlusSignTexture) SDL_DestroyTexture(PlusSignTexture);
     if(ColumnNumberTexture) SDL_DestroyTexture(ColumnNumberTexture);
     if(RowNumberTexture) SDL_DestroyTexture(RowNumberTexture);
}

// void Grid_Editor::InitFont()
// {
//      Font = TTF_OpenFont("..\\data\\Karmina-Bold.otf", default_block_size);
//      if(!Font)
//      {
// 	  printf("Failed to initialize Font at Grid_Editor::InitFont()\n - %s ", TTF_GetError() );
// 	  return;
//      }
// }

void Grid_Editor::InitTexture(SDL_Texture*& Texture, SDL_Rect* Quad, const char* Text)
{
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

     Quad->w = tmp_surface->w;
     Quad->h = tmp_surface->h; 

     Texture = SDL_CreateTextureFromSurface(RenderScreen,  tmp_surface);
     if(!Texture)
     {
	  printf("Failed to init SDL_Texture at Grid_Editor::InitNumberTexture = %s", SDL_GetError() );
     }

     SDL_FreeSurface(tmp_surface);
}

bool ProcessMouseInput(SDL_Rect* TargetQuad, int x, int y)
{
     if(x < TargetQuad->x) return false ;
     else if(x > TargetQuad->x + TargetQuad->w) return false ;
     else if(y < TargetQuad->y) return false ;
     else if(y > TargetQuad->y + TargetQuad->h) return false;
     else return true;
}

void Grid_Editor::HandleEvent(SDL_Event *event)
{
     if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
     {
	  if(!MousePressed)
	  {
	       MousePressed = true;
	       printf("MousePressed = true\n");

	       int old_row_number    = RowNumber;
	       int old_column_number = ColumnNumber;
	       
	       int x_button = event->button.x;
	       int y_button = event->button.y;

	       SDL_Rect TargetPosition = {};
	       TargetPosition.x = EditorBar.x;
	       TargetPosition.y = EditorBar.y;
	       TargetPosition.w = active_block_size;
	       TargetPosition.h = active_block_size;

	       /*  Minus Column button  */
	       if(ProcessMouseInput(&TargetPosition,  x_button, y_button))
	       {
		    RowNumber -= 1; 
		    UpdateRow(RowNumber);
	       }

	       TargetPosition.x += active_block_size*2;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    RowNumber += 1;
		    UpdateRow(RowNumber);
	       }

	       TargetPosition.x += active_block_size;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    ColumnNumber -= 1;
		    UpdateColumn(ColumnNumber);
	       }

	       TargetPosition.x += active_block_size*2;
	       if(ProcessMouseInput(&TargetPosition, x_button, y_button))
	       {
		    ColumnNumber += 1;
		    UpdateColumn(ColumnNumber);
	       }

	       if(old_row_number != RowNumber
		  || old_column_number != ColumnNumber)
	       {
		    grid->update_grid(RowNumber, ColumnNumber);
	       }

	       grid->check_input(x_button, y_button);

	  }
     }
     else if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
     {
	  if(MousePressed)
	  {
	       MousePressed = false;
	       printf("MousePresed = false\n");
	  }
     }
}


void RenderUIElement(SDL_Texture*& Texture, SDL_Rect* TextureRect, SDL_Rect* UiRect)
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();
     
     TextureRect->x = (UiRect->x + UiRect->w / 2 ) - (TextureRect->w / 2);
     TextureRect->y = (UiRect->y + UiRect->w / 2 ) - (TextureRect->h / 2);
     UiRect->x += UiRect->h;

     SDL_RenderCopy(RenderScreen, Texture, 0, TextureRect );
}

void Grid_Editor::RenderEditor()
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Rect TargetPosition = { EditorBar.x, EditorBar.y,
				 active_block_size, active_block_size };
     
     SDL_SetRenderDrawColor(RenderScreen, 0, 20, 147, 255);
     SDL_RenderFillRect(RenderScreen, &EditorBar);
     SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
     SDL_RenderDrawRect(RenderScreen, &EditorBar);
     
     RenderUIElement(MinusSignTexture, &MinusSignQuad, &TargetPosition);
     RenderUIElement(RowNumberTexture, &RowNumberQuad, &TargetPosition);
     RenderUIElement(PlusSignTexture, &PlusSignQuad, &TargetPosition);
     RenderUIElement(MinusSignTexture, &MinusSignQuad, &TargetPosition);
     RenderUIElement(ColumnNumberTexture, &ColumnNumberQuad, &TargetPosition);
     RenderUIElement(PlusSignTexture, &PlusSignQuad, &TargetPosition);

}


void Grid_Editor::UpdateRow(int RowNumber)
{
     if(RowNumber <= 0 ) return;
     
     this->RowNumber = RowNumber;
     InitTexture(RowNumberTexture, &RowNumberQuad, std::to_string(RowNumber).c_str());
}

void Grid_Editor::UpdateColumn(int ColumnNumber)
{
     if(ColumnNumber <= 0) return;

     this->ColumnNumber = ColumnNumber;
     InitTexture(ColumnNumberTexture, &ColumnNumberQuad, std::to_string(ColumnNumber).c_str());
}


