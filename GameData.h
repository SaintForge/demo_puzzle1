#ifndef GAME_DATA_H
#define GAME_DATA_H

using namespace std;

typedef uint8_t u8;
typedef uint32_t u32;

struct Grid_Data
{
     u8  RowAmount;
     u8  ColumnAmount;
     u8 *BitField;
};

struct Figure_Data
{
     u8 FigureForm;
     u8 FigureType;
     u8 Rotation;
     u8 Flip; 
};

struct Level_Data
{
     u8 LevelStatus;
     u8 FigureAmount;
     
     Grid_Data *GridData;
     Figure_Data *FigureData;      
};

struct Level_Binary_Data
{
     u32 LevelAmount;
     Level_Data *LevelData;
};

Level_Binary_Data * OpendLevelBinaryData();
void SaveLevelBinaryData(Level_Binary_Data *LevelBinaryData);

#endif
