#ifndef IO_DATA
#define IO_DATA

typedef uint8_t u8;
typedef uint32_t u32;

enum level_mode
{
     easy, normal, hardcore
};

struct figure_data
{
     u8 FigureForm;
     u8 FigureType;
     u8 Rotation;
     u8 Flip; 
};

struct level_data
{
     u8 LevelStatus;
     u8 LevelNumber;

     u8 RowAmount;
     u8 ColumnAmount;
     u8 FigureAmount;
     
     u8 **BitField;
     figure_data *FigureData;
};
level_data *IOCreateLevelData();
void IODestroyLevelData(level_data* LevelData);

struct level_binary_data
{
     u32 LevelAmount;
     level_data *LevelData;
};
level_binary_data* IOCreateLevelBinaryData();
void IODestroyLevelBinaryData(level_binary_data * LevelBinaryData);

void IOReadLevelBinaryData(level_mode Mode, level_binary_data *LevelBinaryData);
void IOWriteLevelBinaryData(level_mode Mode, level_binary_data *LevelBinaryData);

#endif
