#ifndef MOVING_BLOCK_H
#define MOVING_BLOCK_H

struct MovingBlock
{
     SDL_Rect BlockQuad = {};
     SDL_Rect CollisionQuad = {};
     SDL_Texture *BlockTexture = NULL;
     
     int row_index    = 0;
     int column_index = 0;
     
     void toggle_moving_block();
     
     void release_horizontal_block(std::vector<std::vector<uint8_t>>&, SDL_Rect *);
     void check_horizontal_collision(std::vector<std::vector<uint8_t>>&, SDL_Rect *);

     void move_block_horizontally(int, SDL_Rect*, std::vector<std::vector<uint8_t>>&);
     void move_block_vertically(int);
};


#endif
