#include <stdio.h>
#include <string>

#include "Window.h"
#include "Figure.h"
#include "Level_Editor.h"

Level_Editor::Level_Editor(TTF_Font *& Font, int RowNumber, int ColumnNumber)
{
     printf("Level_Editor(int, int )\n");

     if(!Font)
     {
	  printf("Passin null Font in Level_Editor::Level_Editor()");
	  return;
     }

     this->RowNumber = RowNumber;
     this->ColumnNumber = ColumnNumber;

     this->Font = Font;
     
     // InitFont();
     InitTexture(ColumnNumberTexture, &ColumnNumberQuad,std::to_string(ColumnNumber).c_str());
     InitTexture(RowNumberTexture, &RowNumberQuad, std::to_string(RowNumber).c_str());
     InitTexture(MinusSignTexture, &MinusSignQuad,"-");
     InitTexture(PlusSignTexture, &PlusSignQuad, "+");

     int width = Window_Info::get_width();
     EditorBar.w = active_block_size;
     EditorBar.h = active_block_size * 6;
     EditorBar.x = 0;
     EditorBar.y = (width >> 1) - (EditorBar.w>>1);
}

Level_Editor::~Level_Editor()
{
     if(MinusSignTexture) SDL_DestroyTexture(MinusSignTexture);
     if(PlusSignTexture) SDL_DestroyTexture(PlusSignTexture);
     if(ColumnNumberTexture) SDL_DestroyTexture(ColumnNumberTexture);
     if(RowNumberTexture) SDL_DestroyTexture(RowNumberTexture);
}

// void Level_Editor::InitFont()
// {
//      Font = TTF_OpenFont("..\\data\\Karmina-Bold.otf", default_block_size);
//      if(!Font)
//      {
// 	  printf("Failed to initialize Font at Level_Editor::InitFont()\n - %s ", TTF_GetError() );
// 	  return;
//      }
// }

void Level_Editor::InitTexture(SDL_Texture*& Texture, SDL_Rect* Quad, const char* Text)
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
	  printf("Failed to init SDL_Surface at Level_Editor::InitNumberTexture = %s", TTF_GetError() );
	  return;
     }

     Quad->w = tmp_surface->w;
     Quad->h = tmp_surface->h; 

     Texture = SDL_CreateTextureFromSurface(RenderScreen,  tmp_surface);
     if(!Texture)
     {
	  printf("Failed to init SDL_Texture at Level_Editor::InitNumberTexture = %s", SDL_GetError() );
     }

     SDL_FreeSurface(tmp_surface);
}

void Level_Editor::HandleEvent()
{
     
}


void Level_Editor::Render()
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Rect TargetPosition = {
	  EditorBar.x,
	  EditorBar.y,
	  EditorBar.w,
	  EditorBar.w
     };
     
     MinusSignQuad.x = (TargetPosition.x + TargetPosition.w / 2) - (MinusSignQuad.w / 2);
     MinusSignQuad.y = (TargetPosition.y + TargetPosition.w / 2) - (MinusSignQuad.h / 2);
     TargetPosition.y += EditorBar.w;
     SDL_RenderCopy(RenderScreen, MinusSignTexture, 0, &MinusSignQuad);

     RowNumberQuad.x = (TargetPosition.x + TargetPosition.w / 2) - (RowNumberQuad.w / 2);
     RowNumberQuad.y = (TargetPosition.y + TargetPosition.w / 2) - (RowNumberQuad.h / 2);
     TargetPosition.y += EditorBar.w;
     SDL_RenderCopy(RenderScreen, RowNumberTexture, 0,  &RowNumberQuad);

     PlusSignQuad.x = (TargetPosition.x + TargetPosition.w / 2) - (PlusSignQuad.w / 2);
     PlusSignQuad.y = (TargetPosition.y + TargetPosition.w / 2) - (PlusSignQuad.h / 2);
     TargetPosition.y += EditorBar.w;
     SDL_RenderCopy(RenderScreen, PlusSignTexture, 0, &PlusSignQuad);

     MinusSignQuad.x = (TargetPosition.x + TargetPosition.w / 2) - (MinusSignQuad.w / 2);
     MinusSignQuad.y = (TargetPosition.y + TargetPosition.w / 2) - (MinusSignQuad.h / 2);
     TargetPosition.y += EditorBar.w;
     SDL_RenderCopy(RenderScreen, MinusSignTexture, 0, &MinusSignQuad);
     
     ColumnNumberQuad.x = (TargetPosition.x + TargetPosition.w / 2)- (ColumnNumberQuad.w / 2);
     ColumnNumberQuad.y = (TargetPosition.y + TargetPosition.w / 2)- (ColumnNumberQuad.h / 2);
     TargetPosition.y += EditorBar.w;
     SDL_RenderCopy(RenderScreen, ColumnNumberTexture, 0, &ColumnNumberQuad);

     PlusSignQuad.x = (TargetPosition.x + TargetPosition.w / 2) - (PlusSignQuad.w / 2);
     PlusSignQuad.y = (TargetPosition.y + TargetPosition.w / 2) - (PlusSignQuad.h / 2);
     TargetPosition.y += EditorBar.w;
     SDL_RenderCopy(RenderScreen, PlusSignTexture, 0, &PlusSignQuad);
}


void Level_Editor::UpdateRow(int RowNumber )
{
     
}

void Level_Editor::UpdateColumn(int ColumnNumber )
{
     
}
