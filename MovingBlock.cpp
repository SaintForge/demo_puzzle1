#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <stdio.h>

#include "window.h"
#include "Vector2.h"
#include "Figure.h"
#include "MovingBlock.h"


void MovingBlock::release_horizontal_block(std::vector<std::vector<uint8_t>> & bit_field,
					   SDL_Rect *grid_area )
{
     printf("release_horizontal_block!\n");
     int block_size = active_block_size;

     SDL_Rect previous_cell = {};
     previous_cell.x = grid_area->x + (block_size * column_index);
     previous_cell.y = grid_area->y + (block_size * row_index);
     previous_cell.w = block_size;
     previous_cell.h = block_size;
     
     SDL_Rect target_area = {};
     target_area.x = 0;
     target_area.y = grid_area->y + (row_index * block_size);
     target_area.w = block_size;
     target_area.h = block_size;
     
     int block_centerX = BlockQuad.x + (BlockQuad.w / 2);

     for (int i = 0 ; i < bit_field.size(); ++i)
     {
	  target_area.x = grid_area->x + (i * block_size);

	  if(block_centerX < target_area.x)                      continue;
	  else if(block_centerX > target_area.x + target_area.w) continue;
	  else
	  {
	       BlockQuad.x = target_area.x;
	       BlockQuad.y = target_area.y;

	       if(bit_field[row_index][column_index] == 2)
	       {
		    bit_field[row_index][column_index] = 0;
	       }

	       column_index = i;
	       bit_field[row_index][column_index] = 2;
	  }
     }

}
void MovingBlock::check_horizontal_collision(std::vector<std::vector<uint8_t>> & bit_field,
     SDL_Rect *grid_area )
{
     int block_size = active_block_size;
     int start_x = grid_area->x + (block_size * column_index);
     int start_y = grid_area->y + (block_size * row_index);

     int left_cursor        = start_x;
     int left_cursor_width  = 0;
     int right_cursor_width = block_size;

     if(bit_field[row_index][column_index] == 1)
     {
	  printf("WTF SON!\n ");
     }

     for (int i = column_index-1; i >= 0; --i)
     {
	  printf("left i = %d\n", i );
	  printf("bit_field[row_index][i] = %d\n",bit_field[row_index][i]);
	  if(bit_field[row_index][i] == 0)
	  {
	       left_cursor -= block_size;
	       left_cursor_width += block_size;
	       printf("left_area + 40\n");
	  }
	  else break; 
     }
     for (int i = column_index + 1; i < bit_field[row_index].size(); ++i)
     {
	  if(bit_field[row_index][i] == 0)
	  {
	       right_cursor_width += block_size;
	       printf("right_area + 40\n");
	  }
	  else break;
     }

     CollisionQuad.x = left_cursor; 
     CollisionQuad.w = left_cursor_width + right_cursor_width;

     printf("moving_area.x = %d\n", CollisionQuad.x);
     printf("moving_area.w = %d\n", CollisionQuad.w);    
}

void MovingBlock::move_block_horizontally(int offset_x, SDL_Rect *grid_area,
					  std::vector<std::vector<uint8_t>> & bit_field)
{
     int block_size = active_block_size;
     
     SDL_Rect previous_cell = {};
     previous_cell.x = grid_area->x + (block_size * column_index);
     previous_cell.y = grid_area->y + (block_size * row_index);
     previous_cell.w = block_size;
     previous_cell.h = block_size;
     
     Vector2 velocity;
     velocity.x = offset_x;
     velocity.y = 0;

     BlockQuad.x += round(velocity.x);
     /*

       BUG(Max): There's a bug such that when I move my mouse out of window it jumpes 
       to the opposite direction. It's not so bad, but I should try this on mobile device

     */
     if(BlockQuad.x <= CollisionQuad.x)
     {
	  BlockQuad.x = CollisionQuad.x; 
     }
     else if(BlockQuad.x + BlockQuad.w >= CollisionQuad.x + CollisionQuad.w )
     {
	  printf("right collision\n ");
	  printf("%d + %d >= %d + %d\n", \
		 BlockQuad.x, BlockQuad.w, CollisionQuad.x, CollisionQuad.w );
	  BlockQuad.x = (CollisionQuad.x + CollisionQuad.w) - BlockQuad.w;
	  printf("BlockQuad.x = %d\n",BlockQuad.x);
     }

     if(BlockQuad.x >= previous_cell.x + previous_cell.w)
     {
	  printf("BlockQuad.x >= previous_cell.x + previous_cell.w\n");
	  if(bit_field[row_index][column_index] == 2)
	  {
	       bit_field[row_index][column_index] = 0;
	  }

	  column_index += 1;
	  check_horizontal_collision(bit_field, grid_area );
	  bit_field[row_index][column_index] = 2;
     }
     else if(BlockQuad.x + BlockQuad.w <= previous_cell.x)
     {
	  printf("BlockQuad.x <= previous_cell.x\n");
	  if(bit_field[row_index][column_index] == 2)
	  {
	       bit_field[row_index][column_index] = 0;
	  }

	  column_index -= 1;
	  check_horizontal_collision(bit_field, grid_area );
	  bit_field[row_index][column_index] = 2;
     }
}

void MovingBlock::move_block_vertically(int offset_y)
{
     
}