#ifndef MOVING_BLOCK_H
#define MOVING_BLOCK_H

using namespace std;

struct MovingBlock
{
     SDL_Rect BlockQuad = {} ;
     SDL_Rect CollisionQuad = {};
     SDL_Texture *BlockTexture = NULL;

     SDL_Rect* grid_area = NULL;
     
     int row_index    = 0;
     int column_index = 0;
     void toggle_moving_block();
     
     void release_horizontal_block(vector<vector<uint8_t>>&);
     void check_horizontal_collision(vector<vector<uint8_t>>&);

     void move_block_horizontally(int offset, vector<vector<uint8_t>>& );
     void move_block_vertically(int offset);
};


#endif
