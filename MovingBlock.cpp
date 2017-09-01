#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <stdio.h>

#include "window.h"
#include "Vector2.h"
#include "Figure.h"
#include "MovingBlock.h"


void MovingBlock::release_horizontal_block()
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

     for (int i = 0; i < row_field->size(); ++i)
     {
	  target_area.x = grid_area->x + (i * block_size);

	  if(block_centerX < target_area.x)                      continue;
	  else if(block_centerX > target_area.x + target_area.w) continue;
	  else
	  {
	       BlockQuad.x = target_area.x;
	       BlockQuad.y = target_area.y;

	       if((*row_field)[column_index] == 2)
	       {
		    (*row_field)[column_index] = 0;
	       }

	       column_index = i;
	       (*row_field)[column_index] = 2;
	  }
     }

}

void MovingBlock::release_vertical_block()
{
     printf("MovingBlock::release_vertical_block()\n");
     int block_size = active_block_size;

     SDL_Rect previous_cell = {};
     previous_cell.x = grid_area->x + (block_size * column_index);
     previous_cell.y = grid_area->y + (block_size * row_index);
     previous_cell.w = block_size;
     previous_cell.h = block_size;
     
     SDL_Rect target_area = {};
     target_area.x = grid_area->x + (column_index * block_size);
     target_area.y = 0;
     target_area.w = block_size;
     target_area.h = block_size;

     int block_centerY = BlockQuad.y + (BlockQuad.h / 2) ;

     for (int i = 0; i < column_field.size(); ++i)
     {
	  target_area.y = grid_area->x + (i * block_size);

	  if(block_centerY < target_area.y)                      continue;
	  else if(block_centerY > target_area.y + target_area.h) continue;
	  else
	  {
	       BlockQuad.x = target_area.x;
	       BlockQuad.y = target_area.y;
	       
	       if(*column_field[row_index] == 2)
	       {
		    *column_field[row_index] = 0;
	       }

	       row_index = i;
	       *column_field[row_index] = 2;
	  }
     }

}

void MovingBlock::check_vertical_collision()
{
     int block_size = active_block_size;
     int start_y = grid_area->y + (block_size * row_index);

     int up_cursor          = start_y;
     int up_cursor_length   = 0;
     int down_cursor_length = block_size;

     for (int i = row_index-1; i >= 0; ++i)
     {
	  if(*column_field[i] == 0)
	  {
	       up_cursor -= block_size;
	       up_cursor_length += block_size; 
	  }
	  else break; 
     }

     for (int i = row_index + 1; i < column_field.size(); ++i)
     {
	  if(*column_field[i] == 0)
	  {
	       down_cursor_length += block_size;
	  }
	  else break; 
     }

     CollisionQuad.y = up_cursor;
     CollisionQuad.h = up_cursor_length + down_cursor_length;
}

void MovingBlock::check_horizontal_collision()
{
     int block_size = active_block_size;
     int start_x = grid_area->x + (block_size * column_index);

     int left_cursor        = start_x;
     int left_cursor_width  = 0;
     int right_cursor_width = block_size;

     for (int i = column_index-1; i >= 0; --i)
     {
	  if((*row_field)[i] == 0)
	  {
	       left_cursor -= block_size;
	       left_cursor_width += block_size;
	  }
	  else break; 
     }
     for (int i = column_index + 1; i < row_field->size(); ++i)
     {
	  if((*row_field)[i] == 0)
	  {
	       right_cursor_width += block_size;
	  }
	  else break;
     }

     CollisionQuad.x = left_cursor; 
     CollisionQuad.w = left_cursor_width + right_cursor_width;
}

void MovingBlock::move_block_vertically(int offset_y )
{
     printf("move_block_vertically(int)\n");
     int block_size = active_block_size;

     SDL_Rect previous_cell = {} ;
     previous_cell.x = grid_area->x + (block_size * column_index);
     previous_cell.y = grid_area->y + (block_size * row_index);
     previous_cell.w = block_size;
     previous_cell.h = block_size;

     Vector2 velocity;
     velocity.x = 0;
     velocity.y = offset_y;

     BlockQuad.y += round(velocity.y);

     if(BlockQuad.y <= CollisionQuad.y)
     {
	  BlockQuad.y = CollisionQuad.y;
     }
     else if(BlockQuad.y + BlockQuad.h >= CollisionQuad.y + CollisionQuad.h)
     {
	  BlockQuad.y = (CollisionQuad.y + CollisionQuad.h) - BlockQuad.h;
     }

     bool up_dir    = false;
     bool down_dir = false;

     if(BlockQuad.y >= previous_cell.y + previous_cell.h)  down_dir = true;
     else if(BlockQuad.y + BlockQuad.h <= previous_cell.y) up_dir = true;

     if(up_dir || down_dir)
     {
	  if(*column_field[row_index] == 2)
	  {
	       *column_field[row_index] = 0;
	  }

	  up_dir == true
	       ? row_index -= 1
	       : row_index += 1;

	  check_vertical_collision();
	  *column_field[row_index] = 2;
     }
}

void MovingBlock::move_block_horizontally(int offset_x)
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
	  BlockQuad.x = (CollisionQuad.x + CollisionQuad.w) - BlockQuad.w;
     }

     bool left_dir  = false;
     bool right_dir = false;

     if(BlockQuad.x >= previous_cell.x + previous_cell.w) right_dir = true; 
     else if(BlockQuad.x + BlockQuad.w <= previous_cell.x) left_dir = true; 

     if(left_dir || right_dir)
     {
	  if((*row_field)[column_index] == 2)
	  {
	       (*row_field)[column_index] = 0;
	  }

	  left_dir == true
	       ? column_index -= 1
	       : column_index += 1;
	       
	  check_horizontal_collision();
	  (*row_field)[column_index] = 2;
     }
}

