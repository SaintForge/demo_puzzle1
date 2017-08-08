#ifndef LEVEL_INFO
#define LEVEL_INFO

#include <vector>

#include "Figure.h"

struct Level_Info{
    int status;
    int time;
     
    int row;
    int column;
    std::vector<int> cell_type;
    std::vector<std::pair<int, int>> cell_pos;
    
    std::vector<std::pair<Figure_Form, Figure_Type>> figure_type;
    std::vector<int> figure_angle;
};


#endif
