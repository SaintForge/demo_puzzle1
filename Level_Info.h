#ifndef LEVEL_INFO
#define LEVEL_INFO

#include <vector>

#include "Figure.h"

using figure_info = std::pair<Figure_Form, Figure_Type>;
using figure = std::vector<figure_info>;
using angle = std::vector<uint8_t>;
using bit_pos = std::vector<std::pair<uint8_t, uint8_t>>;
using bit_type = std::vector<uint8_t>;

struct Level_Info{
     int status;
     int time;
     
     int row;
     int column;
          
     bit_type bit;
     bit_pos pos;
     
     figure figures;
     angle angles;
};


#endif
