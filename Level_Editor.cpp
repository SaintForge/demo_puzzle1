#include <stdio.h>
#include <string>

#include "Window.h"
#include "Level_Editor.h"

Level_Editor::Level_Editor(int RowNumber, int ColumnNumber)
{
     printf("Level_Editor(int, int )\n");

     this->RowNumber = RowNumber;
     this->ColumnNumber = ColumnNumber;
     
     InitFont();
     InitTexture(ColumnNumber, std::to_string(ColumnNumber));
     InitTexture(RowNumber, std::to_string(RowNumber));
     InitTexture(MinusSignTexture, "-");
     InitTexture(PlusSignTexture, "+");
}

Level_Editor::~Level_Editor()
{
     if(ColumnNumberTexture) SDL_DestroyTexture(ColumnNumberTexture);
     if(RowNumberTexture) SDL_DestroyTexture(RowNumberTexture);
     if(Font) TTF_CloseFont(Font);
}

void Level_Editor::InitFont()
{
     Font = TTF_OpenFont("..\\data\\Karmina-Bold.otf", 30);
     if(!Font)
     {
	  printf("Failed to initialize Font at Level_Editor::InitFont()\n - %s ", TTF_GetError() );
	  return;
     }
}

void Level_Editor::InitTexture(SDL_Texture*& Texture, const char* Text)
{
     if(!Font) return;
     if(!Text) return;
     if(Texture)
     {
	  SDL_DestroyTexture(Texture);
     }
     
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(Font,  std::to_string(number).c_str(),
						       {255, 255, 255}) ;
     if(!tmp_surface)
     {
	  printf("Failed to init SDL_Surface at Level_Editor::InitNumberTexture = %s", TTF_GetError() );
	  return;
     }

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


void Level_Editor::Renderer()
{
     
}


void Level_Editor::Update()
{
     
}
