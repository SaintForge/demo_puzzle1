#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <stdio.h>
#include <math.h>

#include "Figure_Manager.h"

Figure_Manager::Figure_Manager()
{
     printf("Figure_Manager()\n");
     
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();

     default_zone.w = width;
     default_zone.h = block_width * 8;
     default_zone.y = height - (default_zone.h);
     default_zone.x = 0;

     printf("default_zone.h = %d\n", default_zone.h);

#ifdef _WIN32
     grab_sound     = Mix_LoadWAV("..\\data\\sound\\piece_grab_new.wav");
     idle_sound     = Mix_LoadWAV("..\\data\\sound\\endpillars_panel_scint_endpoint_cut.wav");
     return_sound   = Mix_LoadWAV("..\\data\\sound\\chunk.wav");
     release_sound  = Mix_LoadWAV("..\\data\\sound\\piece_snap_new.wav");
     rotation_sound = Mix_LoadWAV("..\\data\\sound\\piece_snap.wav");

     load_effect("..\\data\\sprites\\circle.png");
#else
     
     grab_sound     = Mix_LoadWAV("data/sound/piece_grab_new.wav");
     idle_sound     = Mix_LoadWAV("data/sound/endpillars_panel_scint_endpoint_cut.wav");
     return_sound   = Mix_LoadWAV("data/sound/piece_snap.wav");
     release_sound  = Mix_LoadWAV("data/sound/piece_snap_new.wav");
     rotation_sound = Mix_LoadWAV("data/sound/piece_snap.wav");

     load_effect("data/sprites/circle.png");
#endif
     Mix_VolumeChunk(return_sound, 16);
     Mix_VolumeChunk(rotation_sound, 4);
     Mix_VolumeChunk(release_sound, 1);
     Mix_VolumeChunk(grab_sound, 1);
     Mix_VolumeChunk(idle_sound, 1);
}
void Figure_Manager::align_vertically()
{
     int area = block_width * 2;

     for (int i = 0; i < figure_container.size(); i++)
     {
	  int pitch_x = i % 2;
	  int pitch_y = i / 2;

	  int center_x = (default_zone.x + (default_zone.w>>2)) + (default_zone.w/2)*pitch_x;
	  int width = figure_container[i]->get_width();
	  int x_target = center_x - (width/2);

	  int center_y = (default_zone.y + block_width>>1) + pitch_y*(area+(block_width>>1));
	  int height = figure_container[i]->get_height();
	  int y_target = center_y+(block_width<<1) - height/2;

	  figure_container[i]->set_default(x_target, y_target);
     }
}

void Figure_Manager::align_horisontally(std::vector<int>& angle)
{
     if(angle.empty()) return;

     for (int i = 0; i < figure_container.size(); i++)
     {
	  adjust_figure(i, angle[i]);
     }
}

void Figure_Manager::realign_horisontally()
{
     for (int i = 0; i < figure_container.size(); i++)
     {
	  adjust_figure(i, 0);
     }    
}

void Figure_Manager::adjust_figure(int index, int angle)
{
     int area = block_width * 2;

     int columns = (figure_container.size() / 2) + 1;
     int distance = (columns * area);

      
     int figure_width = figure_container[index]->get_width();
     int figure_height = figure_container[index]->get_height();
     if(figure_width > figure_height)
     {
     	  figure_container[index]->rotate_shell(angle * 90.0);
     	  figure_container[index]->update_angle(angle * 90.0);
     }

     SDL_Rect rect_area = figure_container[index]->get_area();

     int pitch_x = index / 2;
     int pitch_y = index % 2;
	  
     int center_y = default_zone.y  + ((block_width*4)*pitch_y) + ((block_width/6)*pitch_y);
     int height = rect_area.h;
     int y_target = center_y;

     int center_x = (default_zone.x + (default_zone.w>>1)) - (distance>>1) + (((area)*pitch_x) + ((block_width>>1)*pitch_x));
     int width = rect_area.w;
     int x_target = center_x;
     figure_container[index]->set_default(x_target, y_target);
}

void Figure_Manager::add_new_figure(Figure_Form form, Figure_Type type)
{
     O.push_back(figure_container.size());
     std::shared_ptr<Figure> tmp_figure(new Figure(form, type));
     figure_container.push_back(tmp_figure);
     
     realign_horisontally();
     
     SelectedFigure = figure_container.size() - 1;
}


/*
  This function deletes the specified figure from figure_continer and also looks
  for the given index in O array and adjust other elements if necessary  
*/ 
void Figure_Manager::delete_figure()
{
     printf("Figure_Manager::delete_figure()\n");
     if(SelectedFigure < 0 || SelectedFigure >= figure_container.size()) return;

     int O_index = O[SelectedFigure];
     figure_container.erase(figure_container.begin() + SelectedFigure);

     for (int i = 0 ; i < O.size(); ++i)
     {
	  if(O_index == O[i]) O.erase(O.begin() + i);
     }
     
     if(figure_container.size() != O_index)
     {
	  for (int i = 0; i < O.size(); ++i) if(O[i] > O_index) O[i]--;
     }

     printf("were SelectedFigure = %d\n", SelectedFigure);

     if(figure_container.size() == SelectedFigure) SelectedFigure--;
     else if(SelectedFigure < figure_container.size() - 1) SelectedFigure++;
     else{}

     printf("now SelectedFigure = %d\n", SelectedFigure);

     realign_horisontally();
}

void Figure_Manager::change_figures(
     std::vector<std::pair<Figure_Form, Figure_Type>> &fg, std::vector<int> &angles)
{
     if(!figure_container.empty())
     {
	  figure_container.clear();
     }
     
     if(!O.empty())
     {
	  O.clear();
     }
     
     if(fg.size() != angles.size())
     {
	  printf("Error! - Amount of figures doesn't match amount of angles!\n");
	  return;
     }

     int area = block_width * 2;

     for (int i = 0; i < fg.size(); i++)
     {
	  O.push_back(figure_container.size());
	  std::shared_ptr<Figure> tmp_figure(new Figure(fg[i].first, fg[i].second));
	  figure_container.push_back(tmp_figure);

	  /* 
	     The explanation to whoever reading this.(this will probably be Max himself)
	     This mess down here is basically the calculation 
	     of default places of figures.
	  
	     First off, our figures will be placed in a grid-like area,
	     having a specific row and column(depends on index of the figure).
	     So, we need 2 variables that will help us find its specific place 
	     on the grid.
	     pitch_x indicates the column number.
	     pitch_y indicates the row number.
	  */
	  
	  // int pitch_x = i % 2;
	  // int pitch_y = i / 2;

	  /* After we found our place in a grid we need to find out 
	     where exactly we're gonna place our figure.
	     First of all, we need to find the 'x' position of the figure,
	     and mind you when we're talking about finding the position 
	     we're talking about finding the up-left corner of a rectangle that
	     our figure will be drawn on.
	     1. center_x value indicates the middle point of either left column 
	     or right column(depends on pitch_x). 
	     2. width is the width of the figure.
	     3. x_target basically tells us the offset on 
	     where we should place our x-position of the up-left corner of the rectangle.
	  */
	  // int center_x = (default_zone.x + (default_zone.w>>2)) + (default_zone.w/2)*pitch_x;
	  // int width = tmp_figure->get_width();
	  // int x_target = center_x - (width/2);

	  /* Same goes for 'y' value.
	     1. center_y indicates the area on a given row 
	     and it also uses some offset so that we won't draw our figure on top 
	     of a frame.
	     2. height is the height of the figure.
	     3. y_target tells us where we should place our y-position of up-left corner
	     relative of the rectangle's height.
	  */
	  // int center_y = (default_zone.y + block_width>>1) + pitch_y*(area+(block_width>>1));
	  // int height = tmp_figure->get_height();
	  // int y_target = center_y+(block_width<<1) - height/2;
	  /* and then, with the given coordinates we tell the figure 
	     where to place it on a default state.*/
	  
	  // figure_container[i]->set_default(x_target, y_target);
	  // figure_container[i]->transparent_off();
     }

     align_horisontally(angles);
     
     r_target    = 0;
     r_index     = 0;
     current     = 0;
     alpha       = 255;
     grabbed     = false;
     is_idle     = false;
     is_rotating = false;
     start_ticks = SDL_GetTicks();
     
     hit = false;
     hit_index = 0;
     mouse_left_button  = false;
     mouse_right_button = false;
     restart_figures    = false;

     idle = false;
     idle_index = 0;
     adjust_move = false;

     // vel.x = 0;
     // vel.y = 0;
}

int Figure_Manager::check_mouse_click(int x, int y)
{
     int size = figure_container.size();
     for(int i = 0; i < size; ++i)
     {
	  SDL_Rect area = figure_container[i]->get_area();
	  if(x < area.x)
	       continue;
	  else if(x > area.x + area.w)
	       continue;
	  else if(y < area.y)
	       continue;
	  else if(y > area.y + area.h)
	       continue;
	  else
	  {
	       return i;
	  }
     }
     return -1;
}

void Figure_Manager::select_figure(int index)
{
     if(index < 0 || index >= figure_container.size())
     {
	  return;
     }

     SelectedFigure = index;
}

void Figure_Manager::change_figure_form()
{
     if(figure_container.empty()) return;
     if(SelectedFigure < 0 || SelectedFigure >= figure_container.size()) return;

     int figure_angle = figure_container[SelectedFigure]->get_angle();
     Figure_Type type = figure_container[SelectedFigure]->get_type();
     Figure_Form form = figure_container[SelectedFigure]->get_form();
     if(form != T_figure)
     {
	  int form_index = ((int) form) + 1;
	  form = static_cast<Figure_Form>(form_index);
     }
     else
     {
	  form = O_figure;
     }

     std::shared_ptr<Figure> tmp_figure(new Figure(form, type));

     figure_container.insert(figure_container.begin() + SelectedFigure + 1, tmp_figure);
     figure_container.erase(figure_container.begin() + SelectedFigure);
     
     realign_horisontally();
}

void Figure_Manager::change_figure_type()
{
     if(figure_container.empty()) return;
     if(SelectedFigure < 0 || SelectedFigure >= figure_container.size()) return;

     int figure_angle = figure_container[SelectedFigure]->get_angle();
     Figure_Type type = figure_container[SelectedFigure]->get_type();
     Figure_Form form = figure_container[SelectedFigure]->get_form();
     
     if(type != mirror)
     {
	  int type_index = ((int) type) + 1;
	  type = static_cast<Figure_Type>(type_index);
     }
     else
     {
	  type = classic;
     }

     std::shared_ptr<Figure> tmp_figure(new Figure(form, type));
     figure_container.insert(figure_container.begin() + SelectedFigure + 1, tmp_figure);
     figure_container.erase(figure_container.begin() + SelectedFigure);
     adjust_figure(SelectedFigure, figure_angle / 90);
}

void Figure_Manager::change_figure_angle()
{
     printf("Figure_Manager::change_figure_angle()\n");
     if(SelectedFigure < 0 || SelectedFigure >= figure_container.size()) return;
     
     figure_container[SelectedFigure]->rotate_shell(90);
     figure_container[SelectedFigure]->update_angle(90);

     adjust_figure(SelectedFigure, 0);
}

void Figure_Manager::change_figure_flip()
{
     printf("change_figure_flip()\n");
     if(SelectedFigure < 0 || SelectedFigure >= figure_container.size()) return;

     figure_container[SelectedFigure]->flip_figure();
     adjust_figure(SelectedFigure, 0);
}

void Figure_Manager::load_effect(const char* path){
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     SDL_Surface* tmp_surface;
     SDL_Texture* tmp_texture;
     
     if(idle_effect)
	  SDL_DestroyTexture(idle_effect);

     tmp_surface = IMG_Load(path);
     if(tmp_surface)
     {
	  tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
	  if(tmp_texture)
	  {
	       idle_effect = tmp_texture;
	       SDL_SetTextureBlendMode(idle_effect, SDL_BLENDMODE_BLEND);
	       SDL_FreeSurface(tmp_surface);
	  }
	  else
	  {
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


Figure_Manager::~Figure_Manager(){
     printf("~Figure_Manager()\n");
     
     if(rotation_sound) Mix_FreeChunk(rotation_sound);
     if(release_sound) Mix_FreeChunk(release_sound);
     if(grab_sound) Mix_FreeChunk(grab_sound);

     if(idle_sound) Mix_FreeChunk(idle_sound);
     if(idle_effect) SDL_DestroyTexture(idle_effect);
     if(return_sound) Mix_FreeChunk(return_sound);
}

std::vector<std::shared_ptr<Figure>>*
Figure_Manager::get_container()
{
     std::vector<std::shared_ptr<Figure>>* tmp = &figure_container;
     return tmp;
}

std::shared_ptr<Figure>  Figure_Manager::get_figure_at(int index)
{
     if(index >= 0 && index < figure_container.size())
     {
	  return figure_container[index];
     }
     else
     {
	  return NULL;
     }
}

int Figure_Manager::get_figure_amount()
{
     return figure_container.size();
}

SDL_Rect Figure_Manager::get_idle_zone()
{
     return default_zone;
}

bool Figure_Manager::is_attached(int index){
     if(grabbed && (index == current)) return true; 
     return false;
}

void Figure_Manager::handle_event(SDL_Event &event){
     if(figure_container.empty())
	  return;
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();


#ifdef _WIN32
     switch(event.type){
     	  case SDL_MOUSEBUTTONDOWN:{
	       switch(event.button.button){
		    case SDL_BUTTON_LEFT:{
			 if(!mouse_left_button){
			      if(!grabbed){
				   if(is_idle) is_idle = false;
				   start_ticks = SDL_GetTicks();
		    
				   int x_button = event.button.x;
				   int y_button = event.button.y;
		    
				   int index = check_input(x_button, y_button);
				   if(index != -1){
					hit = true;
					hit_index = index;
					x_rel = x_button - figure_container[index]->get_center().x;
					y_rel = y_button - figure_container[index]->get_center().y;
					
					if(stick_effect){
					     stick_effect = false;
					     stick_effect_alpha = 150;
					}
				   }
			      }
			      else{
				   if(grabbed){
					release_figure();
				   }
			      }
			      mouse_left_button = true;
			 }
		    } break;
		    case SDL_BUTTON_RIGHT:{
			 if(!mouse_right_button){
			      if(grabbed){
				   Figure_Type type = figure_container[current]->get_type();
				   if(type == classic)
					rotate_figure(current);
				   else if(type == mirror){
					figure_container[current]->flip_figure();
					Mix_PlayChannel( -1, rotation_sound, 0 );
				   }
			      }
			      mouse_right_button = true;
			 }
		    } break;
	       }
     	  } break;
     	  case SDL_MOUSEBUTTONUP:{
	       switch(event.button.button){
		    case SDL_BUTTON_LEFT:{
			 if(mouse_left_button){
			      mouse_left_button = false;
			 }
		    } break;
		    case SDL_BUTTON_RIGHT:{
			 if(mouse_right_button){
			      mouse_right_button = false;
			 }
		    } break;
			 
	       } break;
	  }
	  case SDL_MOUSEMOTION:{
	       if(grabbed){
		    int x = event.motion.xrel;
		    int y = event.motion.yrel;
		    figure_container[current]->move_figure(x, y);
	       }
	  } break;
     }
#else
     switch(event.type){
     	  case SDL_FINGERDOWN:{
     	       if(!mouse_left_button && event.tfinger.fingerId == 0){
     		    start_ticks = SDL_GetTicks();
     		    int x = round(event.tfinger.x * width);
     		    int y = round(event.tfinger.y * height);
		    
     		    int index = check_input(x, y);
     		    if(index != -1){
     			 hit = true;
     			 hit_index = index;
			 
			 x_rel = x_button - figure_container[index]->get_center().x;
			 y_rel = y_button - figure_container[index]->get_center().y;

			 if(stick_effect){
			      stick_effect = false;
			      stick_effect_alpha = 150;
			 }
     		    }
     		    mouse_left_button = true;
     	       }
	       else if(mouse_left_button && event.tfinger.fingerId > 0){
		    if(grabbed)
			 rotate_figure(current);
	       }
     	  } break;
     	  case SDL_FINGERUP:{
     	       if(mouse_left_button && (event.tfinger.fingerId == 0)){
     		    if(grabbed){
     			 release_figure();
     		    }
     		    if(hit && !grabbed){
     			 if(!figure_container[hit_index]->is_idle()){
     			      rotate_figure(hit_index);
			      grabbed = true;
			      current = hit_index;
			      figure_high_priority(hit_index);
			 }
     			 hit = false;
     		    }
     		    mouse_left_button = false;
     	       }
     	  } break;
     	  case SDL_FINGERMOTION:{
     	       if(grabbed && event.tfinger.fingerId == 0){
     	  	    int x = round(event.tfinger.dx * width);
     	  	    int y = round(event.tfinger.dy * height);
     	  	    figure_container[current]->move_figure(x, y);
     	       }
     	  } break;
     }
#endif
}

int Figure_Manager::check_input(int x, int y){
     int size = figure_container.size();
     for (int i = size-1; i >= 0; --i)
     {
	  SDL_Rect area = figure_container[O[i]]->get_area();
	  if(x < area.x)
	       continue;
	  else if(x > area.x + area.w)
	       continue;
	  else if(y < area.y)
	       continue;
	  else if(y > area.y + area.h)
	       continue;
	  else
	  {
	       const SDL_Point *points = figure_container[O[i]]->get_shell();
	       int width = figure_container[O[i]]->get_size();
	       SDL_Rect tmp_rect = {0, 0, width, width};
	       for (int j = 0; j < 4; j++)
	       {
		    tmp_rect.x = points[j].x - (width >> 1);
		    tmp_rect.y = points[j].y - (width >> 1);
		    if(x < tmp_rect.x)
			 continue;
		    else if(x > tmp_rect.x + tmp_rect.w)
			 continue;
		    else if(y < tmp_rect.y)
			 continue;
		    else if(y > tmp_rect.y + tmp_rect.h)
			 continue;
		    else{
			 return O[i];
		    }
	       }
	  }
     }
     return -1;
}

bool Figure_Manager::is_grabbed(){
     return grabbed;
}

bool Figure_Manager::is_being_restarted(){
     return restart_figures;
}

bool Figure_Manager::is_being_rotated(){
     return is_rotating;
}

void Figure_Manager::grab_figure(int index){
     if(index < 0 || index > figure_container.size() - 1)
	  return;

     current = index;
     int size = figure_container.size();
     figure_container[current]->mouse_attach(true);
     
     figure_high_priority(index);

     if(is_idle){
	  is_idle = false;
	  Mix_HaltChannel(-1);
	  start_ticks = SDL_GetTicks();
     }
     Mix_PlayChannel( -1, grab_sound, 0);
     grabbed = true;
}


void Figure_Manager::figure_high_priority(int index){
     int size = figure_container.size();
     if(current != O[size - 1]){
	  int up_index = 0;
	  for (int i = 0; i < size; i++)
	       if(O[i] == index) up_index = i;

	  for (int i = up_index; i < size-1; ++i) 
	       O[i] = O[i+1];
	  O[size-1] = current;
     }
}

void Figure_Manager::figure_low_priority(int index){
     int size = figure_container.size();
     if(index < 0 || index > size - 1)
	  return;

     if(O[0] == index)
	  return;

     int start_index = 0;
     for (int i = 0; i < size; i++) {
	  if(O[i] == index){
	       start_index = i;
	       break;
	  }
     }
     

     for (int i = start_index; i > 0; i--)
	  O[i] = O[i-1];
     O[0] = index;
     
}

void Figure_Manager::release_figure(){
     if(!grabbed)
     {
	  return;
     }

     figure_container[current]->mouse_attach(false);
     
     if(check_edge_collision())
     {
	  figure_container[current]->set_default_state();
	  figure_container[current]->set_idle();
	  if(figure_container[current]->get_type() == Figure_Type::mirror)
	  {
	       if(figure_container[current]->is_flip())
	       {
		    figure_container[current]->flip_figure();
	       }
	  }
	  return_to_idle_zone(current);
	  idle = true;
	  idle_index = current;
     }

     grabbed = false;
     start_ticks = SDL_GetTicks();
     Mix_PlayChannel(-1, release_sound, 0);     
}


bool Figure_Manager::check_edge_collision(){
     SDL_Point fig_cntr = figure_container[current]->get_center();
     int width = Window_Info::get_width();
     int height = Window_Info::get_height();

     if(fig_cntr.x < 0 || fig_cntr.y < 0)           return true;
     if(fig_cntr.x > width || fig_cntr.y > height)  return true;

     SDL_Point *shell = figure_container[current]->get_shell();
     for (int i = 0; i < 4; i++)
     {
	  if(shell[i].x < default_zone.x){}
	  else if(shell[i].x > (default_zone.x + default_zone.w)){}
	  else if(shell[i].y < default_zone.y){}
	  else if(shell[i].y > (default_zone.y + default_zone.h)){}
	  else
	  {
	       return true;
	  }
     }
     return false;
}

void Figure_Manager::restart(){
     restart_figures = true;
     for (int i = 0; i < figure_container.size(); i++)
     {
	  if(!figure_container[i]->is_idle())
	  {
	       figure_container[i]->set_default_state();
	       figure_container[i]->set_idle();
	       if(figure_container[i]->get_type() == Figure_Type::mirror)
	       {
		    if(figure_container[i]->is_flip())
		    {
			 figure_container[i]->flip_figure();
		    }
	       }
	       
	       return_to_idle_zone(i);
	  }
     }
     
     if(is_rotating)
     {
	  is_rotating = false;
	  r_target = 0.0f;
     }
}

void Figure_Manager::rotate_figure(int index){
     if(!is_rotating)
     {
	  figure_container[index]->rotate_shell(90.0);
	  Mix_PlayChannel( -1, rotation_sound, 0 );
	  is_rotating = true;
	  r_index = index;
	  start_ticks = SDL_GetTicks();
     }
}


void Figure_Manager::return_to_idle_zone(int index){
     SDL_Point cntr = figure_container[index]->get_center();
     SDL_Point def_cntr = figure_container[index]->get_def_center();
     
     Vector2 vector;
     vector.x = def_cntr.x - cntr.x;
     vector.y = def_cntr.y - cntr.y;
     vector.limit(vector.mag() - active_block_size);
     
     figure_container[index]->move_figure(round(vector.x), round(vector.y));
}

void Figure_Manager::update(){
     float time_dx = (SDL_GetTicks() - start_ticks) / 1000.0f;
     int width  = Window_Info::get_width();
     int height = Window_Info::get_height();

     if(restart_figures && !idle)
     {
	  bool clear = true;
	  for (int i = 0; i < figure_container.size(); i++)
	  {
	       SDL_Point def_cntr = figure_container[i]->get_def_center();
	       if(!move_figure(i, def_cntr))
	       {
		    clear = false;
	       }
	       else{
		    SDL_Point* shell = figure_container[i]->get_shell();
		    SDL_Point* def_shell = figure_container[i]->get_def_shell();
		    for (int j = 0; j < 4; j++) {
			 shell[j].x = def_shell[j].x;
			 shell[j].y = def_shell[j].y;
		    }
	       }
	  }

	  if(clear)
	  {
	       Mix_PlayChannel( -1, return_sound, 0);
	       restart_figures = false;
	       printf("restarted!\n");
	  }
     }
     
     if(hit)
     {
	  bool fig_is_idle = figure_container[hit_index]->is_idle();

#ifdef _WIN32
	  if(fig_is_idle)
	  {
	       SDL_Rect def_rect = figure_container[hit_index]->get_area();
	       
	       grab_figure(hit_index);
	       
	       SDL_Rect rect = figure_container[current]->get_area();
	       int end_y = def_rect.y;// - (rect.h>>1);
	       int offset = end_y - (rect.y + (rect.h>>1));
	       
	       figure_container[current]->move_figure(0, offset);
	       hit = false;
	  }
	  else
	  {
	       grab_figure(hit_index);
	       
	       SDL_Point target;
	       SDL_Point cntr = figure_container[current]->get_center();
	       
	       SDL_GetMouseState(&target.x, &target.y);
	       target.x = (target.x - x_rel) - cntr.x;
	       target.y = (target.y - y_rel) - cntr.y;
	       figure_container[current]->move_figure(target.x, target.y);

	       hit = false;
	  }
#else
	  if(fig_is_idle)
	  {
	       SDL_Rect def_rect = figure_container[hit_index]->get_area();
	       
	       grab_figure(hit_index);
	       
	       SDL_Rect rect = figure_container[current]->get_area();
	       int end_y = def_rect.y;// - (rect.h>>1);
	       int offset = end_y - (rect.y + (rect.h>>1));
	       
	       figure_container[current]->move_figure(0, offset);
	       hit = false;
	  }
	  else if(time_dx > 0.1)
	  {
	       grab_figure(hit_index);
	       
	       SDL_Point target;
	       SDL_Point cntr = figure_container[current]->get_center();
	       
	       SDL_GetMouseState(&target.x, &target.y);
	       target.x = (target.x - x_rel) - cntr.x;
	       target.y = (target.y - y_rel) - cntr.y;
	       
	       figure_container[current]->move_figure(target.x, target.y);

	       hit = false;
	  }
#endif
     }

     if(idle)
     {
	  SDL_Point def_cntr = figure_container[idle_index]->get_def_center();
	  if(move_figure(idle_index, def_cntr))
	  {
	       SDL_Point* shell = figure_container[idle_index]->get_shell();
	       SDL_Point* def_shell = figure_container[idle_index]->get_def_shell();
	       for (int i = 0; i < 4; i++)
	       {
		    shell[i].x = def_shell[i].x;
		    shell[i].y = def_shell[i].y;
	       }
	       idle = false;
	  }
	  else
	  {
	       start_ticks = SDL_GetTicks();
	  }
     }

     // updating animation for rotating a texture
     if(is_rotating)
     {
     	  float angle_dt = time_dx * r_vel;
     	  if(r_target < 90 && !(r_target + angle_dt >= 90.0))
	  {
     	       figure_container[r_index]->update_angle(angle_dt);
     	       r_target += angle_dt;
     	       start_ticks = SDL_GetTicks();
     	  }
     	  else
	  {
     	       figure_container[r_index]->update_angle(90.0f - r_target);
     	       r_target = 0;
     	       is_rotating = false;
	       // TODO: don't forget that!!
	       #ifdef ANDROID
	       // if(grabbed && !mouse_left_button){
	       // 	    grabbed = false;
	       // 	    hit = false;
	       // }
	       #endif
     	  }
     }

     // idle effect algorithm in case if user didn't do anything for about 10 sec
     if(time_dx >= 10.0 && !(grabbed) && !(is_idle))
     {
	  is_idle = true;
	  start_ticks = SDL_GetTicks();
	  alpha = 250;
	  Mix_PlayChannel(-1, idle_sound, 0);
     }
     
     if(is_idle)
     {
	  float elapsed_dt = (SDL_GetTicks() - start_ticks) / 1000.0f;
	  float move_dt    = block_width + (elapsed_dt * ANIMATION_SPEED);
	  float move_alpha = elapsed_dt * ALPHA_SPEED;

	  if(elapsed_dt < 3.0)
	  {
	       idle_rect.w = (move_dt+0.5);
	       idle_rect.h = (move_dt+0.5);
	       alpha = move_alpha <= 255 ? -(move_alpha+0.5) : alpha = 0;
	  }
	  else
	  {
	       alpha       = 255;
	       start_ticks = SDL_GetTicks();
	       Mix_PlayChannel(-1, idle_sound, 0);
	  }
     }
}

bool Figure_Manager::move_figure(int index, SDL_Point &target)
{
     SDL_Point cntr = figure_container[index]->get_center();
     
     if(target.x == cntr.x && target.y == cntr.y)
     {
     	  return true;
     }
     
     const int max_vel = 10;

     Vector2 vel;
     vel.x = target.x - cntr.x;
     vel.y = target.y - cntr.y;

     vel.limit(max_vel);
     figure_container[index]->move_figure(round(vel.x), round(vel.y));

     return false;
}

void Figure_Manager::draw_shadow(int index){
     if(index == current && is_rotating)
     {
	  return;
     }
     if(figure_container[index]->is_sticked())
     {
	  return;
     }

     uint8_t alpha_shadow = 200;

     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     SDL_Point *shell = figure_container[index]->get_shell();
     int size = figure_container[index]->get_size();
     
     for (int i = 0; i < 4; i++)
     {
     	  SDL_Rect rect;
     	  rect.x = shell[i].x - (size >> 1) + 3;
     	  rect.y = shell[i].y - (size >> 1) + 2;
     	  rect.w = size;
     	  rect.h = size;
     	  if(grabbed && index == current)
	  {
	       rect.x += 1;
	       rect.y += 1;
	       alpha_shadow = 50;
	  }
	  
     	  SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, alpha_shadow);
     	  SDL_RenderFillRect(RenderScreen, &rect);
     }
}


void DrawFrame(SDL_Rect Frame, int thicknes)
{
     SDL_Renderer* RenderScreen = Window_Info::get_renderer();

     for (int i = 0; i < thicknes; ++i)
     {
	  SDL_RenderDrawRect(RenderScreen, &Frame);
	  Frame.x -= 1;
	  Frame.w += 2;
	  Frame.y -= 1;
	  Frame.h += 2;
     }
}
void Figure_Manager::draw(){
     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     
     for (int i = 0; i < figure_container.size(); i++)
     {
	  draw_shadow(O[i]);

	  figure_container[O[i]]->draw();
	  if(stick_effect && stick_effect_index == O[i]) animate_stick_effect();
	  if(is_idle && alpha != 0)
	  {
	       if(!figure_container[O[i]]->is_sticked())
	       {
		    SDL_Point cntr = figure_container[O[i]]->get_center();
		    
		    idle_rect.x = cntr.x - (idle_rect.w/2);
		    idle_rect.y = cntr.y - (idle_rect.h/2);
		    SDL_RenderCopy(RenderScreen, idle_effect, 0, &idle_rect);
	       }
	  }
     }

     if(DeveloperMode && !figure_container.empty())
     {
	  if(SelectedFigure >= 0 || SelectedFigure < figure_container.size())
	  {
	       SDL_SetRenderDrawColor(RenderScreen, 0, 255, 0, 255);
	       SDL_Rect figure_area = figure_container[SelectedFigure]->get_area();
	       DrawFrame(figure_area, 5);
	  }

	  for (int i = 0; i < figure_container.size(); ++i)
	  {
	       figure_container[i]->draw_shell();
	  }

     }

     if(is_idle) SDL_SetTextureAlphaMod(idle_effect, alpha);
}


void Figure_Manager::toogle_stick_effect(int index)
{
     if(stick_effect)
     {
	  stick_effect_alpha = 200;
     }
     
     stick_effect = true;
     stick_effect_index = index;
}

void Figure_Manager::animate_stick_effect()
{
     if(!stick_effect)
     {
	  return;
     }
     
     if(stick_effect_alpha <= 10)
     {
	  stick_effect = false;
	  stick_effect_alpha = 200;
	  return;
     }
     
     stick_effect_alpha -= 5;

     SDL_Renderer *RenderScreen = Window_Info::get_renderer();
     SDL_Point *shell = figure_container[stick_effect_index]->get_shell();
     int size = get_activeblock_size();
     
     for (int i = 0; i < 4; i++)
     {
	  SDL_Rect rect;

	  rect.x = shell[i].x - (size >> 1);
	  rect.y = shell[i].y - (size >> 1); 
	  rect.w = rect.h = size;

	  SDL_SetRenderDrawColor(RenderScreen, 255, 255, 255, stick_effect_alpha);
	  SDL_RenderFillRect(RenderScreen, &rect);
     }
}
