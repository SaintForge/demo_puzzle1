#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <stdio.h>

#include "window.h"
#include "Vector2.h"
#include "Figure.h"
#include "MovingBlock.h"

void MovingBlock::create_line_field(const std::vector<uint8_t> &bit_field)
{
     if(!line_field.empty())
     {
	  line_field.clear();
     }

     for (int i = 0 ; i < bit_field.size(); ++i)
     {
	  line_field.push_back(bit_field[i]);
     }
}

void MovingBlock::update_line_field(const std::vector<uint8_t>& bit_field )
{
     for (int i = 0; i < line_field.size(); ++i)
     {
	  line_field[i] = bit_field[i];
     }
}

void MovingBlock::print_line_field()
{
     for(int n : line_field)
	  printf("%d ",n);
     printf("\n");
}

void MovingBlock::update_shift_animation()
{
     if(!is_moving) return;

     if(target_x == BlockQuad.x && target_y == BlockQuad.y)
     {
	  is_moving = false;
	  return;
     }

     const int max_vel = active_block_size / 4;

     Vector2 vel;
     vel.x = target_x - BlockQuad.x;
     vel.y = target_y - BlockQuad.y;

     vel.limit(max_vel);

     BlockQuad.x += vel.x;
     BlockQuad.y += vel.y;
}

void MovingBlock::shift_block()
{
     int index = 0;
     is_vertical == true
	  ? index = row_index
	  : index = column_index;
     
     int BlockSize = 0;
     if(move_left)
     {
	  if(index + 1 > line_field.size()) return;
	  if(line_field[index + 1] != 0)    return;
	  line_field[index] = 0;
	  
	  BlockSize = active_block_size;
	  index += 1;
	  move_left = false;
     }
     else
     {
	  if(index - 1 < 0)              return;
	  if(line_field[index - 1] != 0) return;
	  line_field[index] = 0; 

	  index -= 1;
	  BlockSize = -active_block_size;
	  move_left = true;
     }

     is_moving = true;
     
     if(is_vertical)
     {
	  line_field[index] = 3;
	  target_x = BlockQuad.x;
	  target_y = BlockQuad.y + BlockSize;
	  row_index = index;
     }
     else
     {
	  line_field[index] = 2;
	  target_x = BlockQuad.x + BlockSize;
	  target_y = BlockQuad.y;
	  column_index = index;
     }
}
