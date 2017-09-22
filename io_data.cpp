#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include "io_data.h"

static
char* ModeToChar(level_mode Mode)
{
     switch(Mode)
     {
	  case easy:     return "easy.bin";
	  case normal:   return "normal.bin";
	  case hardcore: return "hardcore.bin";
     }

}

void IOReadLevelBinaryData(level_mode Mode, level_binary_data *LevelBinaryData)
{
     printf("IOReadLevelBinaryData()\n");

     size_t ObjRead = 0;
     level_data *LevelData = 0;
     SDL_RWops *BinaryFile = 0;

     BinaryFile = SDL_RWFromFile(ModeToChar(Mode), "rb");

     if(!BinaryFile)
     {
	  printf("Failed to open BinaryFile!\n");
	  return;
     }

     if(!LevelBinaryData)
     {
	  LevelBinaryData = (level_binary_data*)malloc(sizeof(level_binary_data));
	  if(!LevelBinaryData)
	  {
	       printf("Failed to malloc level_binary_data!\n");
	       SDL_RWclose(BinaryFile);
	       return;
	  }

	  LevelBinaryData->LevelAmount = 0;
	  LevelBinaryData->LevelData   = 0;
     }

     ObjRead = SDL_RWread(BinaryFile, LevelBinaryData, sizeof(level_binary_data), 1);
     if(!ObjRead)
     {
	  printf("zero objects for level_binary_data!\n");
	  SDL_RWclose(BinaryFile);
	  return;
     }

     LevelBinaryData->LevelData =
	  (level_data*)malloc(sizeof(level_data) * LevelBinaryData->LevelAmount);
     if(!LevelBinaryData->LevelData)
     {
	  printf("Failed to malloc level_data!\n");
	  SDL_RWclose(BinaryFile);
	  return;
     }

     for (int i = 0; i < LevelBinaryData->LevelAmount; ++i)
     {
	  LevelBinaryData->LevelData[i]->BitField   = 0;
	  LevelBinaryData->LevelData[i]->FigureData = 0;
     }

     
     LevelData = LevelBinaryData->LevelData;
     for (int i = 0; i < LevelBinaryData->LevelAmount; ++i)
     {
	  ObjRead = SDL_RWread(BinaryFile, LevelData, sizeof(level_data), 1);
	  if(!ObjRead)
	  {
	       printf("zero objects for level_data!\n"); 
	       SDL_RWclose(BinaryFile);
	       return;
	  }

	  LevelData->BitField = (u8**)malloc(sizeof(u8*)*LevelData->RowAmount);
	  for (int j = 0; j < LevelData.RowAmount; ++j)
	  {
	       LevelData->BitField[j] = (u8*)malloc(sizeof(u8) * LevelData->ColumnAmount);
	       ObjRead = SDL_RWread(BinaryFile, LevelData->BitField[j], sizeof(u8), LevelData->ColumnAmount);
	       if(!ObjRead)
	       {
		    printf("zero objects for bit_field!\n");
		    SDL_RWclose(BinaryFile);
		    return;
	       }
	  }

	  LevelData->FigureData =
	       (figure_data*)malloc(sizeof(figure_data) * LevelData->FigureAmount);
	  
	  ObjRead = SDL_RWread(BinaryFile, LevelData->FigureData, sizeof(figure_data), LevelData->FigureAmount);
	  if(!ObjRead)
	  {
	       printf("zero objects for figure_data!\n");
	       SDL_RWclose(BinaryFile);
	       return;
	  }
	  printf("ObjRead for figure_data = %d\n", ObjRead);
	  LevelBinaryData->LevelData = LevelData;
     }

     SDL_RWclose(BinaryFile);
}

void IOWriteLevelBinaryData(level_mode Mode, level_binary_data *LevelBinaryData)
{
     printf("IOWriteLevelData()\n");

     if(!LevelBinaryData) return;

     SDL_RWops *BinaryFile = 0;
     size_t ObjWritten     = 0;
     level_data *LevelData = 0;

     BinaryFile = SDL_RWFromFile(ModeToChar(Mode), "wb");
     if(!BinaryFile)
     {
	  printf("Failed to open BinaryFile for writing!- %s\n", SDL_GetError());
	  return;
     }

     ObjWritten = SDL_RWwrite(BinaryFile, LevelBinaryData, sizeof(level_binary_data), 1);

     LevelData = LevelBinaryData->LevelData;
     for (int i = 0;
	  i < LevelBinaryData->LevelAmount; ++i)
     {
	  ObjWritten = SDL_RWwrite(BinaryFile, LevelData, sizeof(level_data), 1);
	  printf("ObjWritten for level_data = %d\n", ObjWritten);

	  for (int l = 0 ; l < LevelData->RowAmount; ++l)
	  {
	       ObjWritten = SDL_RWwrite(BinaryFile, LevelData->BitField[l], sizeof(u8), LevelData->ColumnAmount);
	       printf("ObjWritten for bitfield = %d\n", ObjWritten);
	  }

	  ObjWritten = SDL_RWwrite(BinaryFile, LevelData->FigureData, sizeof(figure_data), LevelData->FigureAmount);
	  printf("ObjWritten for figure_data = %d\n", ObjWritten);
     }

}

level_data* IOCreateLevelData(u32 LevelAmount)
{
     printf("IOCreateLevelData()\n");
     
     level_data *LevelData = 0;

     LevelData = (level_data*)malloc(sizeof(level_data) * LevelAmount);
     if(LevelData)
     {
	  LevelData->BitField   = 0;
	  LevelData->FigureData = 0;

	  return LevelData;
     }
     else
     {
	  printf("Failed to malloc level_data!\n");
	  return NULL;
     }
     
}

void IODestroyLevelData(level_data *LevelData)
{
     printf("IODestroyLevelData()\n");

     if(LevelData)
     {
	  if(Leveldata->BitField)
	  {
	       for (int i = 0; i < LevelData->RowAmount; ++i)
	       {
		    free(LevelData->BitField[i]);
		    LevelData->BitField[i] = 0;
	       }
	       
	       free(LevelData->BitField);
	       LevelData->BitField = 0;
	  }

	  if(LevelData->FigureData)
	  {
	       free(LevelData->FigureData);
	       LevelData->FigureData = 0;
	  }

	  free(LevelData);
	  LevelData = 0;
     }
}

level_binary_data* IOCreateBinaryData()
{
     printf("IOCreateBinaryData()\n");

     level_binary_data* LevelBinaryData = 0;

     LevelBinaryData = (level_binary_data*)malloc(sizeof(level_binary_data));
     if(LevelBinaryData)
     {
	  LevelBinaryData->LevelAmount       = 0;
	  LevelBinaryData->LevelData         = 0;
	  return LevelBinaryData;
     }
     else
     {
	  printf("Failed to malloc level_binary_data!()\n");
	  return NULL;
     }
}


void IODestroyLevelBinaryData(level_binary_data* LevelBinaryData)
{
     printf("IODestroyLevelBinaryData()\n");
     if(LevelBinaryData)
     {
	  if(LevelBinaryData->LevelAmount > 0)
	  {
	       for (int i = 0; i < LevelBinaryData->LevelAmount; ++i)
	       {
		    IODestroyLevelData(LevelBinaryData->LevelData[i]);
	       }
	  }

	  free(LevelBinaryData);
	  LevelBinaryData = 0;
     }
}
