#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <stdio.h>

#include "window.h"
#include "Grid_Manager.h"

Grid_Manager::Grid_Manager()
{
     printf("Grid_Manager::Grid_Manager()\n");
     
#ifdef _WIN32
     load_grid_texture(GridCellTexture, "..\\data\\sprites\\grid_cell.png");
     load_grid_texture(MovingBlockTexture, "..\\data\\sprites\\o_s.png");
     piece_snap = Mix_LoadWAV("..\\data\\sound\\piece_snap2.wav");
#else
     load_grid_texture(GridCellTexture, "..\\data\\sprites\\grid_cell.png");
     load_grid_texture(MovingBlockTexture, "..\\data\\sprites\\grid_cell2.png");
     piece_snap = Mix_LoadWAV("data/sound/piece_snap2.wav");
#endif
     Mix_VolumeChunk(piece_snap, 8);
}

void print_grid(std::vector<std::vector<uint8_t>> &v)
{
     printf("------------\n");
     for (int i = 0; i < v.size(); i++)
     {
	  for (int j = 0; j < v[i].size(); j++)
	  {
	       printf("%d ", v[i][j]);
	  }
	  printf("\n");
     }
     printf("------------\n");
}
Grid_Manager::~Grid_Manager()
{
     printf("Grid_Manager::~Grid_Manager()\n");
     if(GridCellTexture)
     {
	  SDL_DestroyTexture(GridCellTexture);
     }

     if(MovingBlockTexture)
     {
	  SDL_DestroyTexture(MovingBlockTexture);
     }
     
     if(piece_snap)
     {
	  Mix_FreeChunk(piece_snap);
     }
     
     if(block_sound)
     {
	  Mix_FreeChunk(block_sound);
     }
}


int Grid_Manager::get_column_amount()
{
     return column_amount;
}
int Grid_Manager::get_row_amount()
{
     return row_amount; 
}

int Grid_Manager::get_block_size()
{
     return size;
}

bool Grid_Manager::GridMouseClick(int x_mouse, int y_mouse)
{
     printf("Grid_Manager::check_input()\n");
     
     if(x_mouse < grid_area.x)                    return false ;
     else if(y_mouse < grid_area.y)               return false ;
     else if(x_mouse > grid_area.x + grid_area.w) return false ;
     else if(y_mouse > grid_area.y + grid_area.h) return false ;
     else
     {
	  return true; 
     }

}

bool Grid_Manager::CellMouseClick(int mouse_x, int mouse_y, int *row, int *column)
{
     if(!row || !column)
     {
	  return false;
     }

     SDL_Rect targ_rect = {0, 0, size, size };
     
     for (int i = 0 ; i < row_amount; ++i)
     {
	  for (int j = 0; j < column_amount; ++j)
	  {
	       targ_rect.x = grid_area.x + (j*size);
	       targ_rect.y = grid_area.y + (i*size);
		    
	       if(mouse_x <= targ_rect.x)                    continue;
	       else if(mouse_y <= targ_rect.y)               continue;
	       else if(mouse_x > targ_rect.x + targ_rect.w)  continue;
	       else if(mouse_y > targ_rect.y + targ_rect.h)  continue;
	       else
	       {
		    *row = i;
		    *column = j;
		    return true; 
	       }
	  }
     }

     return false; 
}

void Grid_Manager::change_block(int row_index, int column_index)
{
     printf("Grid_Manager::change_block()\n");
     
     if(row_index < 0 || row_index >= row_amount)
     {
	  return;
     }
     
     if(column_index < 0 || column_index >= column_amount)
     {
	  return;
     }

     if(bit_field[row_index][column_index] != 2)
     {
	  bit_field[row_index][column_index] += 1;
	  
	  if(bit_field[row_index][column_index] == 2)
	  {
	       add_moving_block(row_index, column_index);
	  }
     }
     else
     {
	  if(bit_field[row_index][column_index] == 2)
	  {
	       delete_moving_block(row_index, column_index);
	  }
	  
	  bit_field[row_index][column_index] = 0;
     }
}

void Grid_Manager::add_moving_block(int row_index, int column_index )
{
     printf("Grid_Manager::add_moving_block(int, int )\n");
     
     if(row_index < 0 || column_index < 0)
     {
	  return;
     }

     MovingBlock moving_block = {};
     moving_block.BlockQuad.x = grid_area.x + (column_index * size);
     moving_block.BlockQuad.y = grid_area.y + (row_index * size);
     moving_block.BlockQuad.w = size;
     moving_block.BlockQuad.h = size;

     moving_block.BlockTexture = MovingBlockTexture;
     
     moving_block.row_index = row_index;
     moving_block.column_index = column_index;

     moving_block.grid_area = &grid_area;

     mov_bl.push_back(moving_block);
}

void Grid_Manager::delete_moving_block(int row_index, int column_index )
{
     printf("Grid_Manager::delete_moving_block(int, int )\n");

     printf("row_index = %d\n", row_index );
     printf("column_index = %d\n", column_index );
     printf("mov_block.row_index = %d\n", mov_bl[0].row_index );
     printf("mov_block.row_index = %d\n", mov_bl[0].column_index );
     

     for (int i = 0; i < mov_bl.size(); ++i)
     {
	  if(mov_bl[i].row_index == row_index
	     && mov_bl[i].column_index == column_index)
	  {
	       mov_bl.erase(mov_bl.begin() + i );
	       printf("deleted!\n");
	  }
     }

}

void Grid_Manager::update_grid(int row, int column)
{
     printf("Grid_Manager::update_grid()\n");
     
     if(row <= 0 || column <= 0)
     {
	  return;
     }
     
     if(!stick_list.size() > 0)
     {
     	  stick_list.clear();
     }

     row_amount = row;
     column_amount = column;

     int x_pos = (grid_area.x + grid_area.w / 2);
     int y_pos = (grid_area.y + grid_area.h / 2);

     grid_area.w = (size) * column;
     grid_area.h = (size) * row;

     grid_area.x = x_pos - (grid_area.w>>1);
     grid_area.y = y_pos - (grid_area.h>>1);

     //check the case when new amount is higher than old one
     bit_field.resize(row_amount);
     for (int i = 0; i < row_amount; ++i)
     {
	  bit_field[i].resize(column_amount);
     }
}

void Grid_Manager::update_grid(Figure_Manager *man, int x, int y, Level_Info& info)
{
     if(!stick_list.empty())
     {
	  stick_list.clear();
     }

     row_amount = info.row;
     column_amount = info.column;

     grid_area.w = (size) * info.column;
     grid_area.h = (size) * info.row;

     grid_area.x = x - (grid_area.w>>1);
     grid_area.y = y - (grid_area.h>>1);

     bit_field.resize(row_amount);
     for (int i = 0; i < row_amount; i++)
     {
	  bit_field[i].resize(column_amount);
	  for (int j = 0; j < column_amount; j++)
	  {
	       bit_field[i][j] = 0;
	  }
     }
     
     if(!info.cell_pos.empty() && (info.cell_pos.size() == info.cell_type.size()))
     {
	  for (int i = 0; i < info.cell_pos.size(); i++)
	  {
	       int x = info.cell_pos[i].first;
	       int y = info.cell_pos[i].second;
	       bit_field[x][y] = info.cell_type[i];
	  }
     }
     state = GRID_EMPTY;
     manager = man;
     stick_list.reserve(manager->get_figure_amount());

     //TODO: do normal distibution here!
     
     start_animation = true;
     int block_amount = row_amount * column_amount;
     int def = active_block_size / block_amount;
     if(ani_block.empty())
     {
	  ani_block.reserve(block_amount);
	  for (int i = 0; i < block_amount; i++)
	  {
	       ani_block.push_back(rand() % (default_block_size));
	  }
     }

}

bool Grid_Manager::is_figure_inside(std::shared_ptr<Figure> fig)
{
     const SDL_Point *shell = fig->get_shell();
     
     for (int j = 0; j < 4; j++)
     {
	  if(shell[j].x <= grid_area.x)                    return false;
	  else if(shell[j].y <= grid_area.y)               return false; 
	  else if(shell[j].x > grid_area.x + grid_area.w)  return false; 
	  else if(shell[j].y > grid_area.y + grid_area.h)  return false;
     }

     return true;
}

void Grid_Manager::restart_grid()
{
     if(!stick_list.empty())
     {
	  int row_index    = 0;
	  int column_index = 0;

	  for (int i = 0; i < stick_list.size(); ++i)
	  {
	       for (int j = 0; j < 4; ++j)
	       {
		    row_index = stick_list[i].row[j];
		    column_index = stick_list[i].col[j];
		    
		    bit_field[row_index][column_index] = 0;
	       }
	  }

	  stick_list.clear();
     }
}

bool Grid_Manager::check_rectangle_collision(int x, int y, SDL_Rect* area)
{
     if(x < area->x)                return false;
     else if(y < area->y)           return false; 
     else if(x > area->x + area->w) return false;
     else if(y > area->y + area->h) return false;
     else                           return true;
}

void Grid_Manager::handle_event(SDL_Event& event)
{
     //TODO(Max): Do the Android version of that
     if(event.type == SDL_MOUSEBUTTONDOWN
	&& event.button.button == SDL_BUTTON_LEFT )
     {
	  if(!mouse_pressed)
	  {
	       mouse_pressed = true;

	       SDL_Rect area;
	       int x_mouse = event.button.x;
	       int y_mouse = event.button.y;

	       if(GridMouseClick(x_mouse, y_mouse ))
	       {
		    for (int i = 0 ; i < mov_bl.size(); ++i)
		    {
			 int row_index    = mov_bl[i].row_index;
			 int column_index = mov_bl[i].column_index;
			      
			 if(check_rectangle_collision(x_mouse, y_mouse, &mov_bl[i].BlockQuad))
			 {
			      mov_bl[i].check_horizontal_collision(bit_field);
			      block_grabbed = true;
			      mov_indx = i;
			      // bit_field[row_index][column_index] = 0;
			 }
		    }

	       }
	  }
     }
     else if(event.type  == SDL_MOUSEBUTTONUP
	     && event.button.button == SDL_BUTTON_LEFT)
     {
	  if(mouse_pressed)
	  {
	       mouse_pressed = false;
		    
	       if(block_grabbed)
	       {
		    block_grabbed = false;
		    mov_bl[mov_indx].release_horizontal_block(bit_field);
	       }
	  }
     }
     else if(event.type == SDL_MOUSEMOTION)
     {
	  if(mouse_pressed)
	  {
	       if(block_grabbed)
	       {
		    int offset_x = event.motion.xrel;
		    printf("offset_x = %d\n", offset_x );
		    mov_bl[mov_indx].move_block_horizontally(offset_x, bit_field );
	       }
	  }
     }
}


// void Grid_Manager::release_horizontal_block()
// {
//      int row_index = mov_bl[mov_indx].row_index;
//      int previous_column_index = mov_bl[mov_indx].column_index;
//      SDL_Rect previous_area =
// 	  {
// 	       grid_area.x + (size * previous_column_index),
// 	       0,
// 	       size,
// 	       size
// 	  };
     
//      SDL_Rect target_area =
// 	  {
// 	       0,
// 	       grid_area.y + (row_index * size),
// 	       size,
// 	       size
// 	  };  

//      int block_centerX = mov_bl[mov_indx].BlockQuad.x + (mov_bl[mov_indx].BlockQuad.w/2);

//      for (int i = 0; i < bit_field[row_index].size(); ++i)
//      {
// 	  target_area.x = grid_area.x + (i * size);

// 	  if(block_centerX < target_area.x)
// 	  {
// 	       continue;
// 	  }
// 	  else if(block_centerX > target_area.x + target_area.w)
// 	  {
// 	       continue;
// 	  }
// 	  else
// 	  {
// 	       mov_bl[mov_indx].BlockQuad.x = target_area.x;
// 	       mov_bl[mov_indx].BlockQuad.y = target_area.y;

// 	       mov_bl[mov_indx].row_index    = row_index;
// 	       mov_bl[mov_indx].column_index = i;

// 	       bit_field[row_index][i] = 2;
// 	       break;
// 	  }
//      }

//      block_centerX = mov_bl[mov_indx].BlockQuad.x + \
// 	  (mov_bl[mov_indx].BlockQuad.w/2);

//      if(block_centerX > previous_area.x + (previous_area.w / 2))
//      {
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index] = 0;
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index+1] = 2;

// 	  mov_bl[mov_indx].column_index += 1;
//      }
//      else if(block_centerX < previous_area.x)
//      {
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index] = 0;
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index-1] = 2;

// 	  mov_bl[mov_indx].column_index -= 1;
//      }
     

// }

// void Grid_Manager::move_block_horizontally(int offset_x)
// {
//      printf("Grid_Manager::move_block_horizontally()\n");
     
//      int previous_column_index = mov_bl[mov_indx].column_index;
//      SDL_Rect previous_area =
// 	  {
// 	       grid_area.x + (size * previous_column_index),
// 	       0,
// 	       size,
// 	       size
// 	  };

//      Vector2 velocity;
//      velocity.x = offset_x;
//      velocity.y = 0;

//      check_collision_area(mov_indx);

//      if(mov_bl[mov_indx].BlockQuad.x + \
// 	mov_bl[mov_indx].BlockQuad.w > moving_area.x + moving_area.w)
//      {
// 	  mov_bl[mov_indx].BlockQuad.x = (moving_area.x + moving_area.w) - \
// 	       mov_bl[mov_indx].BlockQuad.w;
//      }

//      if(mov_bl[mov_indx].BlockQuad.x >= previous_area.x + previous_area.w)
//      {
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index] = 0;
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index+1] = 2;

// 	  mov_bl[mov_indx].column_index += 1;
//      }
//      else if(mov_bl[mov_indx].BlockQuad.x <= previous_area.x)
//      {
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index] = 0;
// 	  bit_field[mov_bl[mov_indx].row_index][previous_column_index-1] = 2;

// 	  mov_bl[mov_indx].column_index -= 1;
//      }
// }

// void Grid_Manager::move_block_vertically(int offset_y)
// {
     
// }

int Grid_Manager::update()
{
     // print_grid(bit_field);

     bool was_action = false; 
     int amount = manager->get_figure_amount();
     
     if(amount == 0)
     {
	  return GRID_ZERO;
     }
     
     if(state == GRID_FULL)
     {
	  return GRID_FULL;
     }

     if(manager->is_being_restarted())
     {
	  return GRID_EMPTY;
     }

     for (int i = 0; i < stick_list.size(); ++i)
     {
	  if(!stick_list[i].is_sticked)
	  {
	       int index = stick_list[i].index;
	       std::shared_ptr<Figure> figure = manager->get_figure_at(index);
	       
	       SDL_Point cntr = figure->get_center();
	       SDL_Point targ_cntr = stick_list[i].targ_cntr;

	       const int max_vel = 5;

	       Vector2 vel;
	       vel.x = targ_cntr.x - cntr.x;
	       vel.y = targ_cntr.y - cntr.y;

	       vel.limit(max_vel);
	       figure->move_figure(round(vel.x), round(vel.y));

	       cntr = figure->get_center();
	       if(cntr.x == targ_cntr.x && cntr.y == targ_cntr.y)
	       {
		    Mix_PlayChannel(-1, piece_snap, 0);
		    stick_list[i].is_sticked = true;
		    was_action = true;
			 
		    for (int j = 0; j < 4; j++)
		    {
			 bit_field[stick_list[i].row[j]][stick_list[i].col[j]] = 1;
		    }
		    if(is_full())
		    {
			 state = GRID_FULL;
		    }
	       }
	  }
     }

     for (int k = 0; k < amount; k++)
     {
     	  std::shared_ptr<Figure> figure = manager->get_figure_at(k); // try O[i]
	  
	  if(figure->is_idle())
	  {
	       continue;
	  }
	  bool attached = manager->is_attached(k);
	  bool sticked  = figure->is_sticked();


	  // NOTE: there are basically 3 scenarions right here
	  //1.a If a figure is sticked and not attached then we skip it
	  //1.b If a figure is not sticked and attached then we skip it as well
	  
	  //2. If a figure is both sticked and attached then we unstick it from the bit_field
	  //3. If a figure is not sticked and not attached then we look if we can stick it
	  
	  if(!sticked != !attached)
	  {
	       continue;
	  }
	  else if(!sticked && !attached)
	  {
	       if(manager->is_being_rotated())
	       {
		    continue;
	       }
	       if(!is_figure_inside(figure))
	       {
		    continue;
	       }
	       else
	       {
		    if(manager->is_being_rotated())
		    {
			 continue;
		    }
		    
		    //check if we can stick it
		    float offset_x = 0;
		    float offset_y = 0;
		    int row_index[4] = {0};
		    int col_index[4] = {0};
		    SDL_Rect rect = {0, 0, size, size};
		    const SDL_Point *shell = figure->get_shell();

		    int count = 0; // this variable will count to 4
		    // and when it does we skip all the loops which is really helpful
		    for (int i = 0; i < row_amount && count != 4; ++i)
		    {
			 for (int j = 0; j < column_amount && count != 4; ++j)
			 {
			      rect.x = grid_area.x + (j*size);
			      rect.y = grid_area.y + (i*size);
			      for (int b = 0; b < 4; ++b)
			      {
				   if(shell[b].x <= rect.x)               continue;
				   else if(shell[b].y <= rect.y)          continue;
				   else if(shell[b].x > rect.x + rect.w)  continue;
				   else if(shell[b].y > rect.y + rect.h)  continue;
				   else
				   {
					if(count == 0)
					{
					     offset_x = rect.x+(rect.w/2) - shell[b].x;
					     offset_y = rect.y+(rect.h/2) - shell[b].y;
					}
					
					row_index[b] = i;
					col_index[b] = j;
					count++;
					break;
				   }

			      }
			 }
		    }

		    if(count != 4)
		    {
			 continue;
		    }
		    
		    bool free = true;
		    for (int s = 0; s < 4 && free == true; s++)
		    {
			 if(bit_field[row_index[s]][col_index[s]] > 0)
			 {
			      free = false;
			 }
		    }
		    // if place is free then we stick out figure
		    if(free)
		    {
			 //  set the state of a figure and playing a sound
			 figure->grid_stick();
			 was_action = true;
			 
			 // creating a unit that will hold the information about
			 // that figure in case we want to remove that figure from the grid 
			 Sticked_Unit unit;
			 unit.index = k;
			 for (int p = 0; p < 4; p++)
			 {
			      // bit_field[row_index[p]][col_index[p]] = 1;
			      unit.row[p] = row_index[p];
			      unit.col[p] = col_index[p];
			 }
			      
			 SDL_Point cntr = figure->get_center();
			 unit.targ_cntr.x = cntr.x + offset_x;
			 unit.targ_cntr.y = cntr.y + offset_y;
			      
			 if(offset_x == 0 && offset_y == 0)
			 {
			      Mix_PlayChannel(-1, piece_snap, 0);
			 }
			 stick_list.push_back(unit);

			 // this is for the figure to be drawn first
			 // and not on top of the others
			 manager->figure_low_priority(k);
			 if(is_full())
			 {
			      state = GRID_FULL;
			 }
			 else
			 {
			      return GRID_HIT;
			 }
		    }
	       } // end of else
	  }
	  else
	  {
	       // if we grab a figure that was sticked then we unstick it!
	       //looking for the given index in our vector
	       for (int i = 0; i < stick_list.size(); i++)
	       {
		    if(stick_list[i].index == k)
		    {
			 for (int j = 0; j < 4; j++)
			 {
			      bit_field[stick_list[i].row[j]][stick_list[i].col[j]] = 0;
			 }
			 
			 stick_list.erase(stick_list.begin() + i);
			 figure->grid_unstick();
			 break;
		    }
	       }
	  }
     }

     if(was_action)
     {
     	  if(block_grabbed)
     	  {
     	       mov_bl[mov_indx].check_horizontal_collision(bit_field);
     	  }
     }

     return GRID_EMPTY;
}

bool Grid_Manager::is_full()
{
     for (int i = 0; i < bit_field.size(); i++)
     {
	  for (int j = 0; j < bit_field[i].size(); j++)
	  {
	       if(bit_field[i][j] == 0)
	       {
		    return false;
	       }
	  }
     }

     return true;
}

int Grid_Manager::get_last_stick_index()
{
     if(stick_list.empty())
     {
	  return -1;
     }

     return stick_list[stick_list.size()-1].index;
}

bool Grid_Manager::unattach_figure(int index){
     for (int i = 0; i < stick_list.size(); i++)
     {
	  if(stick_list[i].index == index)
	  {
	       for (int j = 0; j < 4; j++)
	       {
		    bit_field[stick_list[i].row[j]][stick_list[i].col[j]] = 0;
	       }
			 
	       stick_list.erase(stick_list.begin() + i);
	       
	       return true;
	  }
     }
     return false;
}

void Grid_Manager::draw()
{
     if(!GridCellTexture)
     {
	  return;
     }
     
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();

     SDL_Rect rect;
     rect.w = size;
     rect.h = size;
     
     if(!start_animation)
     {
	  for (int i = 0; i < row_amount; i++)
	  {
	       rect.y = grid_area.y + (rect.w*i);
	       for (int j = 0; j < column_amount; j++)
	       {
		    uint8_t status = bit_field[i][j];
		    if(status != 1)
		    {
			 rect.x = grid_area.x + (rect.w*j);
			 if(status == 0 || status == 2 )
			 {
			      SDL_RenderCopy(RenderScreen, GridCellTexture, NULL, &rect);
			 }
			 // else if(status == 2)
			 // {
			 //      SDL_RenderCopy(RenderScreen, MovingBlockTexture, NULL, &rect);
			 // }
		    }
	       }
	  }
     }
     else
     {
	  if(animate_start())
	  {
	       start_animation = false;
	  }
     }

     for (int i = 0 ; i < mov_bl.size(); ++i)
     {
	  SDL_RenderCopy(RenderScreen, mov_bl[i].BlockTexture, NULL, &mov_bl[i].BlockQuad );
     }

}

bool Grid_Manager::animate_start()
{
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();

     SDL_Rect cur_rect;
     int x_start,y_start;
     
     bool res = true;
     bool play = false;
     int sound_loops = false;
     
     int index = 0;
     
     for (int i = 0; i < row_amount; i++)
     {
	  y_start = grid_area.y + (size*i) + (size/2);
	  for (int j = 0; j < column_amount; j++)
	  {
	       uint8_t status = bit_field[i][j];
	       x_start = grid_area.x + (size*j) + (size/2);

	       cur_rect.x = x_start - (ani_block[index]/2);
	       cur_rect.y = y_start - (ani_block[index]/2);
	       cur_rect.w = ani_block[index];
	       cur_rect.h = ani_block[index];
	       
	       if(status == 0)
	       {
		    SDL_RenderCopy(RenderScreen, GridCellTexture, NULL, &cur_rect);
	       }
	       else if(status == 2)
	       {
	       	    SDL_RenderCopy(RenderScreen, MovingBlockTexture, NULL, &cur_rect);
	       }
	       else
	       {
	       }
	       	       
	       if(ani_block[index] < active_block_size)
	       {
		    ani_block[index] += 1;
		    if(res)
		    {
			 res = false;
		    }
		    if(ani_block[index] == active_block_size)
		    {
			 if(!play)
			 {
			      play = true;
			 }
		    }
	       }
	       ++index;
	  }
     }
     if(res)
     {
	  ani_block.clear();
     }

     return res;
}

void Grid_Manager::load_grid_texture(SDL_Texture*& text, const char* path)
{
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     SDL_Surface* tmp_surface;
     SDL_Texture* tmp_texture;
     
     if(text)
     {
	  SDL_DestroyTexture(text);
     }

     tmp_surface = IMG_Load(path);
     if(tmp_surface)
     {
	  tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
	  if(tmp_texture)
	  {
	       text = tmp_texture;
	       SDL_FreeSurface(tmp_surface);
	  }
	  else
	  {
	       SDL_FreeSurface(tmp_surface);
	       printf("failed to load a texture from the surface! %s\n", SDL_GetError());
	       return;
	  }
     }
     else
     {
	  printf("failed to load a picture with a given path! - %s\n", IMG_GetError());
	  return;
     }

}

