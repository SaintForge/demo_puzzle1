#ifndef MOVING_BLOCK_H
#define MOVING_BLOCK_H

using namespace std;

struct MovingBlock
{
     bool is_vertical;
     SDL_Rect BlockQuad = {};
     SDL_Rect CollisionQuad = {};
     SDL_Texture *BlockTexture = NULL;

     SDL_Rect* grid_area = NULL;
     
     vector<uint8_t> *row_field;
     vector<uint8_t*> column_field; 
     
     int row_index    = 0;
     int column_index = 0;
     
     void release_horizontal_block();
     void release_vertical_block();
     
     void check_horizontal_collision();
     void check_vertical_collision();

     void move_block_horizontally(int offset);
     void move_block_vertically(int offset);
};


#endif
