#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <stdio.h>

#include "window.h"
#include "Grid_Manager.h"

Grid_Manager::Grid_Manager(){
     printf("Grid_Manager::Grid_Manager()\n");
#ifdef PC
     load_grid_texture(texture, "..\\data\\sprites\\grid_cell.png");
     load_grid_texture(texture2, "..\\data\\sprites\\grid_cell2.png");
     piece_snap = Mix_LoadWAV("..\\data\\sound\\piece_snap2.wav");
     // block_sound = Mix_LoadWAV("..\\data\\sound\\idle.wav");
#else
     load_grid_texture(texture, "..\\data\\sprites\\grid_cell.png");
     load_grid_texture(texture2, "..\\data\\sprites\\grid_cell2.png");
     piece_snap = Mix_LoadWAV("data/sound/piece_snap2.wav");
#endif
     Mix_VolumeChunk(piece_snap, 8);
     // Mix_VolumeChunk(block_sound, 20);
}



void print_grid(std::vector<std::vector<uint8_t>> &v){
     for (int i = 0 ; i < v.size(); i++) {
	  for (int j = 0; j < v[i].size(); j++) {
	       printf("%d ", v[i][j]);
	  }
	  printf("\n");
     }
}
Grid_Manager::~Grid_Manager(){
     printf("Grid_Manager::~Grid_Manager()\n");
     if(texture)
	  SDL_DestroyTexture(texture);
     if(piece_snap)
	  Mix_FreeChunk(piece_snap);
     if(block_sound)
	  Mix_FreeChunk(block_sound);
}


int Grid_Manager::get_block_size(){
     return size;
}

void Grid_Manager::update_grid(Figure_Manager *man,
			       int x, int y,
			       Level_Info& info){
     if(!stick_list.empty()) stick_list.clear();

     row_amount = info.row;
     column_amount = info.column;

     grid_area.w = (size) * info.column;
     grid_area.h = (size) * info.row;

     grid_area.x = x - (grid_area.w>>1);
     grid_area.y = y - (grid_area.h>>1);

     bit_field.resize(row_amount);
     for (int i = 0; i < row_amount; i++) {
	  bit_field[i].resize(column_amount);
	  for (int j = 0; j < column_amount; j++) {
	       bit_field[i][j] = 0;
	  }
     }
     if(!info.pos.empty() && (info.pos.size() == info.bit.size())){
	  for (int i = 0; i < info.pos.size(); i++){
	       int x = info.pos[i].first;
	       int y = info.pos[i].second;
	       bit_field[x][y] = info.bit[i];
	  }
     }
     state = GRID_EMPTY;
     manager = man;
     stick_list.reserve(manager->get_figure_amount());

     //TODO: do normal distibution here!
     start_animation = true;
     int block_amount = row_amount * column_amount;
     int def = active_block_size / block_amount;
     if(ani_block.empty()){
	  ani_block.reserve(block_amount);
	  for (int i = 0; i < block_amount; i++) {
	       ani_block.push_back(rand() % (default_block_size));
	  }
     }

}

bool Grid_Manager::is_figure_inside(std::shared_ptr<Figure> fig){
     const SDL_Point *shell = fig->get_shell();
     
     for (int j = 0; j < 4; j++) {
	  if(shell[j].x <= grid_area.x)                    return false;
	  else if(shell[j].y <= grid_area.y)               return false; 
	  else if(shell[j].x > grid_area.x + grid_area.w)  return false; 
	  else if(shell[j].y > grid_area.y + grid_area.h)  return false;
     }

     return true;
}

void Grid_Manager::restart_grid(){
     if(!stick_list.empty()){
	  for (int i = 0; i < bit_field.size(); i++) {
	       // std::fill(bit_field[i].begin(), bit_field[i].end(), 0);
	       for (int j = 0; j < bit_field[i].size(); j++) {
		    if(bit_field[i][j] == 1)
			 bit_field[i][j] = 0;
	       }
	  }
	  stick_list.clear();
     }
}

// TODO: we can absolutely sure make this algorithm work better
int Grid_Manager::update(){ // TODO:: make 1 functions that check rectangle collisions
     if(state == GRID_FULL && stick_effect == false){
	  return GRID_FULL;
     }

     if(manager->is_being_restarted())
	  return GRID_EMPTY;
     int amount = manager->get_figure_amount();
     if(amount == 0)
	  return GRID_ZERO;


     if(stick_effect){
	  bool done = true;
	  for (int i = 0; i < stick_list.size(); i++) {
	       int index = stick_list[i].index;
	       std::shared_ptr<Figure> figure = manager->get_figure_at(index);
	       SDL_Point cntr = figure->get_center();
	       SDL_Point targ_cntr = stick_list[i].targ_cntr;
	       if(cntr.x != targ_cntr.x || cntr.y != targ_cntr.y){
		    const int max_vel = 5;

		    Vector2 vel;
		    vel.x = targ_cntr.x - cntr.x;
		    vel.y = targ_cntr.y - cntr.y;

		    vel.limit(max_vel);
		    figure->move_figure(round(vel.x), round(vel.y));
		    done = false;
		    
		    cntr = figure->get_center();
		    if(cntr.x == targ_cntr.x && cntr.y == targ_cntr.y){
			 Mix_PlayChannel(-1, piece_snap, 0);
			 for (int j = 0; j < 4; j++) 
			      bit_field[stick_list[i].row[j]][stick_list[i].col[j]] = 1;
			 if(is_full()){
			      state = GRID_FULL;
			 }
		    }
	       }
	  }
	  if(done == true)
	       stick_effect = false;
     }

     for (int k = 0; k < amount; k++) {
     	  std::shared_ptr<Figure> figure = manager->get_figure_at(k); // try O[i]
	  
	  // if(manager->is_being_rotated()) continue;
	  if(figure->is_idle()) continue;
	  bool attached = manager->is_attached(k);
	  bool sticked  = figure->is_sticked();


	  //there are basically 3 scenarions right here
	  //1.a If a figure is sticked and not attached then we skip it
	  //1.b If a figure is not sticked and attached then we skip it as well
	  
	  //2. If a figure is both sticked and attached then we unstick it from the bit_field
	  //3. If a figure is not sticked and not attached then we look if we can stick it
	  
	  if(!sticked != !attached)
	       continue;
	  else if(!sticked && !attached){
	       if(manager->is_being_rotated()) continue;
	       if(!is_figure_inside(figure))
		    continue;
	       else{
		    if(manager->is_being_rotated()) continue;
		    //check if we can stick it
		    float offset_x = 0;
		    float offset_y = 0;
		    int row_index[4] = {0};
		    int col_index[4] = {0};
		    SDL_Rect rect = {0, 0, size, size};
		    const SDL_Point *shell = figure->get_shell();

		    int count = 0; // this variable will count to 4
		    // and when it does we skip all the loops which is really helpful
		    for (int i = 0; i < row_amount && count != 4; ++i) {
			 for (int j = 0; j < column_amount && count != 4; ++j) {
			      rect.x = grid_area.x + (j*size);
			      rect.y = grid_area.y + (i*size);
			      for (int b = 0; b < 4; ++b) {
				   if(shell[b].x <= rect.x)               continue;
				   else if(shell[b].y <= rect.y)          continue;
				   else if(shell[b].x > rect.x + rect.w)  continue;
				   else if(shell[b].y > rect.y + rect.h)  continue;
				   else{
					if(count == 0){
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
			 continue;
		    
		    bool free = true;
		    for (int s = 0; s < 4 && free == true; s++) {
			 if(bit_field[row_index[s]][col_index[s]] > 0) free = false;
		    }
		    // if place is free then we stick out figure
		    if(free){
			 //set the state of a figure and playing a sound
			 figure->grid_stick(); // TODO:!!!!!!!!!!!!!!!!!!!!!!!!!!!
			 stick_effect = true;
			 
			 
			 //creating a unit that will hold the information about
			 //that figure in case we want to remove that figure from the grid 
			 {
			      Sticked_Unit unit;
			      unit.index = k;
			      for (int p = 0; p < 4; p++) {
				   // bit_field[row_index[p]][col_index[p]] = 1;
				   unit.row[p] = row_index[p];
				   unit.col[p] = col_index[p];
			      }
			      SDL_Point cntr = figure->get_center();
			      unit.targ_cntr.x = cntr.x + offset_x;
			      unit.targ_cntr.y = cntr.y + offset_y;
			      if(offset_x == 0 && offset_y == 0)
				   Mix_PlayChannel(-1, piece_snap, 0);
			      stick_list.push_back(unit);
			 }


			 //this is for the figure to be drawn first
			 //and not on top of the others
			 manager->figure_low_priority(k);
			 if(is_full()){
			      state = GRID_FULL;
			 }
			 else
			      return GRID_HIT;
		    }
	       } // end of else
	  }
	  else{
	       // if we grab a figure that was sticked then we unstick it!
	       //looking for the given index in our vector
	       for (int i = 0; i < stick_list.size(); i++) {
		    if(stick_list[i].index == k){
			 for (int j = 0; j < 4; j++)
			      bit_field[stick_list[i].row[j]][stick_list[i].col[j]] = 0;
			 
			 stick_list.erase(stick_list.begin() + i);
			 figure->grid_stick();
			 // figure->transparent_off();
			 break;
		    }
	       }
	  }
     }

     

     return GRID_EMPTY;
}

bool Grid_Manager::is_full(){
     bool if_one = true;
     for (int i = 0; i < bit_field.size() && if_one == true; i++) {
	  for (int j = 0; j < bit_field[i].size(); j++) {
	       if(bit_field[i][j] == 0){
		    if_one = false;
		    break;
	       }
	  }
     }
     if(if_one){
	  return true;
     }
     return false;
}

int Grid_Manager::get_last_stick_index(){
     if(stick_list.empty()) return -1;

     return stick_list[stick_list.size()-1].index;
}

bool Grid_Manager::unattach_figure(int index){
     for (int i = 0; i < stick_list.size(); i++) {
	  if(stick_list[i].index == index){
	       for (int j = 0; j < 4; j++)
		    bit_field[stick_list[i].row[j]][stick_list[i].col[j]] = 0;
			 
	       stick_list.erase(stick_list.begin() + i);
	       
	       // manager->get_figure_at(index)->grid_stick();
	       return true;
	  }
     }
     return false;
}

void Grid_Manager::draw(){
     if(!texture)
	  return;
     SDL_Renderer *RenderScreen = Window_Info::get_window_info().get_renderer();

     SDL_Rect rect;
     rect.w = size;
     rect.h = size;
     
     if(!start_animation){
	  for (int i = 0; i < row_amount; i++) {
	       rect.y = grid_area.y + (rect.w*i);
	       for (int j = 0; j < column_amount; j++) {
		    uint8_t status = bit_field[i][j];
		    if(status != 1){
			 rect.x = grid_area.x + (rect.w*j);
			 if(status == 0)
			      SDL_RenderCopy(RenderScreen, texture, NULL, &rect);
			 else if(status == 2){
			      SDL_RenderCopy(RenderScreen, texture2, NULL, &rect);
			 }
		    }
	       }
	  }
     }
     else{
	  if(animate_start())
	       start_animation = false;
     }
}

bool Grid_Manager::animate_start(){
     SDL_Renderer *RenderScreen = Window_Info::get_window_info().get_renderer();

     SDL_Rect cur_rect;
     int x_start,y_start;
     
     bool res = true;
     bool play = false;
     int sound_loops = false;
     
     int index = 0;
     
     for (int i = 0; i < row_amount; i++) {
	  y_start = grid_area.y + (size*i) + (size/2);
	  for (int j = 0; j < column_amount; j++) {
	       uint8_t status = bit_field[i][j];
	       x_start = grid_area.x + (size*j) + (size/2);

	       cur_rect.x = x_start - (ani_block[index]/2);
	       cur_rect.y = y_start - (ani_block[index]/2);
	       cur_rect.w = ani_block[index];
	       cur_rect.h = ani_block[index];
	       
	       if(status == 0)
		    SDL_RenderCopy(RenderScreen, texture, NULL, &cur_rect);
	       else if(status == 2){
		    SDL_RenderCopy(RenderScreen, texture2, NULL, &cur_rect);
	       }
	       	       
	       if(ani_block[index] < active_block_size){
		    ani_block[index] += 1;
		    if(res)
			 res = false;
		    if(ani_block[index] == active_block_size){
			 if(!play){
			      // Mix_PlayChannel(-1, block_sound, 0);
			      play = true;
			 }
		    }
	       }
	       ++index;
	  }
     }
     if(res){
	  // printf("deleting!\n");
	  ani_block.clear();
     }

     

     return res;
}

void Grid_Manager::load_grid_texture(SDL_Texture*& text, const char* path){
     SDL_Renderer *RenderScreen = Window_Info::get_window_info().get_renderer();
     SDL_Surface* tmp_surface;
     SDL_Texture* tmp_texture;
     
     if(text)
	  SDL_DestroyTexture(texture);

     tmp_surface = IMG_Load(path);
     if(tmp_surface){
	  tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
	  if(tmp_texture){
	       text = tmp_texture;
	       SDL_FreeSurface(tmp_surface);
	  }
	  else{
	       SDL_FreeSurface(tmp_surface);
	       printf("failed to load a texture from the surface! %s\n", SDL_GetError());
	       return;
	  }
     }
     else{
	  printf("failed to load a picture with a given path! - %s\n", IMG_GetError());
	  return;
     }

}

