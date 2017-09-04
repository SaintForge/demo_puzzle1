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

     for (int i = 0; i < line_field.size(); ++i)
     {
	  target_area.x = grid_area->x + (i * block_size);

	  if(block_centerX < target_area.x)                      continue;
	  else if(block_centerX > target_area.x + target_area.w) continue;
	  else
	  {
	       BlockQuad.x = target_area.x;
	       BlockQuad.y = target_area.y;

	       if(line_field[column_index] == 2)
	       {
		    line_field[column_index] = 0;
	       }

	       column_index = i;
	       line_field[column_index] = 2;
	  }
     }

}

void MovingBlock::print_line_field()
{
     for(int n : line_field)
	  printf("%d ",n);
     printf("\n");

     printf("quad.x = %d\n",CollisionQuad.x);
     printf("quad.y = %d\n",CollisionQuad.y);
     printf("quad.w = %d\n",CollisionQuad.w);
     printf("quad.h = %d\n",CollisionQuad.h);
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

     for (int i = 0; i < line_field.size(); ++i)
     {
	  target_area.y = grid_area->y + (i * block_size);

	  if(block_centerY < target_area.y)                      continue;
	  else if(block_centerY > target_area.y + target_area.h) continue;
	  else
	  {
	       BlockQuad.x = target_area.x;
	       BlockQuad.y = target_area.y;
	       
	       if(line_field[row_index] == 3)
	       {
		    line_field[row_index] = 0;
	       }

	       row_index = i;
	       line_field[row_index] = 3;
	  }
     }

}

void MovingBlock::update_collision_quad()
{
     printf("update_collision_quad()\n");
     printf("before collision!\n ");
     print_line_field();
     printf("column_index = %d\n",column_index);

     int BlockSize = active_block_size;
     
     int StartPoint = 0;
     int StartIndex = 0;
     // int * QuadCursor = NULL;
     // int * QuadLength = NULL;

     if(is_vertical)
     {
	  StartPoint = grid_area->y + (BlockSize * row_index);
	  StartIndex = row_index;
     }
     else
     {
	  StartPoint = grid_area->x + (BlockSize * column_index );
	  StartIndex = column_index;
     }

     int Cursor           = StartPoint;
     int FirstSideLength  = 0;
     int SecondSideLength = BlockSize;

     printf("first side!\n ");
     for (int i = StartIndex-1; i >= 0; --i)
     {
	  if(line_field[i] == 0)
	  {
	       printf("sum\n");
	       Cursor -= BlockSize;
	       FirstSideLength += BlockSize;
	  }
	  else break; 
     }

     printf("second side!\n ");
     for (int i = StartIndex + 1; i < line_field.size(); ++i)
     {
	  if(line_field[i] == 0)
	  {
	       printf("sum!\n");
	       SecondSideLength += BlockSize;
	  }
	  else break; 
     }

     if(is_vertical)
     {
	  CollisionQuad.y = Cursor;
	  CollisionQuad.h = FirstSideLength + SecondSideLength;
     }
     else
     {
	  CollisionQuad.x = Cursor;
	  CollisionQuad.w = FirstSideLength + SecondSideLength;
     }

     printf("after!\n");
     print_line_field();

}

void MovingBlock::move_block_vertically(int offset_y )
{
     // printf("move_block_vertically(int)\n");
     int block_size = active_block_size;

     SDL_Rect previous_cell = {};
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
	  if(line_field[row_index] == 3)
	  {
	       line_field[row_index] = 0;
	  }

	  up_dir == true
	       ? row_index -= 1
	       : row_index += 1;

	  update_collision_quad();
	  line_field[row_index] = 3;
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
	  if(line_field[column_index] == 2)
	  {
	       line_field[column_index] = 0;
	  }

	  left_dir == true
	       ? column_index -= 1
	       : column_index += 1;
	       
	  update_collision_quad();
	  line_field[column_index] = 2;
     }
}

