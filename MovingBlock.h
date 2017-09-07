#ifndef MOVING_BLOCK_H
#define MOVING_BLOCK_H

using namespace std;

struct MovingBlock
{
     bool is_vertical = false;
     bool is_moving   = false;
     bool move_left   = false;
     
     SDL_Rect BlockQuad = {};
     SDL_Texture *BlockTexture = NULL;

     SDL_Rect* grid_area = NULL;
     
     vector<uint8_t> line_field; 

     int target_x     = 0;
     int target_y     = 0;
     int row_index    = 0;
     int column_index = 0;

     void create_line_field(const vector<uint8_t>&);
     void update_line_field(const vector<uint8_t>&);
     
     void update_shift_animation();
     void shift_block();

     void print_line_field();
};


#endif
