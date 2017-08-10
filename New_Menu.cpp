#ifndef NEW_MENU
#define NEW_MENU

#include <SDL.h>
#include <SDL_image.h>

#include "New_Menu.h"
#include "Window.h"

#define round(x) ((x)>=0?(float)((x)+0.5):(float)((x)-0.5))

Layer::Layer(){
    
}
void Layer::init_layer(Select_Menu menu, SDL_Texture*& texture,
		       SDL_Texture*& texture2, TTF_Font*& font){
    
    int width = Window_Info::get_width();
    int height = Window_Info::get_height();
    
    // target_width = (width*2)/3;
    is_opening = false;
    is_closing = false;
    
    this->button_texture = texture;
    this->highlight_texture = texture2;
    this->font = font;

    speed = width / 19;

    page_info_texture = NULL;
    
    int w, h;
    switch(menu){
	case MAIN_PAGE:{
	    Button_Info but;
	    w = (width*2)/3;
	    h = width/4;
	    // Play button
	    {
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2);
		but.str_array.push_back("Play");
		// but.speed = 5;
		btn.push_back(but);
	    }
	    
	    // Settings button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2) + (h)+(h/4);
		but.str_array.push_back("Settings");
		// but.speed = 6;
		btn.push_back(but);
	    }
	} break;
	case DIFF_PAGE:{
	    Button_Info but;
	    w = (width*2)/3;
	    h = width/4;

	    // Normal button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2);
		but.str_array.push_back("Normal");
		btn.push_back(but);
	    }
	    
	    // Easy button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = btn[0].pos.y - (h/4) - h;
		but.str_array.push_back("Easy");
		btn.push_back(but);
	    }

	    // Hard button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = btn[0].pos.y + (h/4) + h;
		but.str_array.push_back("Hard");
		btn.push_back(but);
	    }
	    // Back button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = height - (h + h/4);
		but.str_array.push_back("Back");
		btn.push_back(but);
	    }
	    
	} break;
	case SETTINGS_PAGE:{
	    Button_Info but;
	    w = (width*2)/3;
	    h = width/4;

	    // Sound button
	    {
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2);
		but.str_array.push_back("Sound");
		// but.speed = 5;
		btn.push_back(but);
	    }
	    
	    // About button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2) + (h)+(h/4);
		but.str_array.push_back("About");
		// but.speed = 6;
		btn.push_back(but);
	    }
	    
	    // Back button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = height - (h + h/4);
		but.str_array.push_back("Back");
		btn.push_back(but);
	    }
	} break;
	case ABOUT_PAGE:{
	    Button_Info but;
	    w = (width*2)/3;
	    h = width/4;

	    // Sound button
	    {
		but.status = 1;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2);
		but.str_array.push_back("Demo by");
		// but.speed = 5;
		btn.push_back(but);
	    }

	    //About button
	    {
		but.str_array.clear();
		but.status = 1;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = (height/2) - (h/2) + (h)+(h/4);
		but.str_array.push_back("Maxim Sokolov");
		btn.push_back(but);
	    }
	    // Back button
	    {
		but.str_array.clear();
		but.status = 0;
		but.pos.w = 0; but.pos.h = h;
		but.pos.x = (width/2);
		but.pos.y = height - (h + h/4);
		but.str_array.push_back("Back");
		btn.push_back(but);
	    }
	}break;

	case EASY_PAGE:   init_buttons(EASY_PAGE);  break;
	case NORMAL_PAGE: init_buttons(NORMAL_PAGE); break;
	case HARD_PAGE:   init_buttons(HARD_PAGE); break;
    }
}

void Layer::init_buttons(Select_Menu menu_choice){
    int width = Window_Info::get_width();
    int height = Window_Info::get_height();

    lvl_info = new Level_Mode();

    {
	SDL_Surface *tmp_surface = 0;
	tmp_surface = IMG_Load(level_button_texture_path);
	level_button_texture = SDL_CreateTextureFromSurface(Window_Info::get_renderer(), tmp_surface);
	SDL_FreeSurface(tmp_surface);
    }
    
    // Back button
    {
	Button_Info but;
	int w = (width*2)/3;
	int h = width/4;
	
	but.status = 0;
	but.pos.w = 0; but.pos.h = h;
	but.pos.x = (width/2);
	but.pos.y = height - (h + h/4);
	but.str_array.push_back("Back");
	btn.push_back(but);
    }
    
    level_page = true;

    std::string lvl_mode;
    switch(menu_choice){
	case EASY_PAGE: lvl_mode = "easy"; break;
	case NORMAL_PAGE: lvl_mode = "normal"; break;
	case HARD_PAGE: lvl_mode = "hard"; break;
    }
    
    int lvl_amount = 0; // maximum amount of levels to play
    
    GameData data;
    if(data.open_data())
    {
	int tmp_index = 0;
	int start_index = data.read_variable(tmp_index, lvl_mode, lvl_amount);
	if(start_index > 0)
	{
	    data.set_searching_point(start_index);
	}

	int block_size = 60;

	int start_x = (width >> 1) - ((block_size) * 4 + (30))/2 + (block_size/2);
	int start_y = (height / 3) - ((block_size) * 5)/2;
    
	int pitch_y = -1;
	int index = 0;
    
	lvl_info->scroll_bounds.x = 0;
	lvl_info->scroll_bounds.y = 0;
	lvl_info->scroll_bounds.w = width;
	lvl_info->scroll_bounds.h = btn[0].pos.y;

	int countdown = lvl_amount;
	//TODO: make io function that read all levels at once
	while(countdown != 0){
	    if(index % 20 == 0 && index != 0) start_x += width + (width/2);
	    if(index % 4 == 0) pitch_y++;
	    if(pitch_y >= 5) pitch_y = 0;

	    Level_Info lvl_info;
	    data.load_level(index+1, lvl_info);

	    Button_Info but;
	    but.str_array.push_back(std::to_string(index + 1));
	    but.status = lvl_info.status;
	    but.pos.w = 0; but.pos.h = block_size;
	    but.pos.x = start_x + ((index % 4) * (block_size+10));
	    printf("but.pos.x = %d\n",start_x);
	    but.pos.y = start_y + (pitch_y * (block_size+10));
	    but.is_button    = true;
	    btn.push_back(but);
	
	    index++;
	    countdown--;
	}

	//
	// Page info
	//
	lvl_info->page_number = 1;
	lvl_info->page_number_max = lvl_amount / 20 + 1;
	lvl_info->page_info   = std::to_string(lvl_info->page_number) + " / " + \
	    std::to_string(lvl_info->page_number_max);
	update_page_info_texture();
    }
    else
    {
	printf("Can't read levels!\n");
    }
}

bool Layer::is_level_screen(){
    return level_page;
}

void Layer::update_level_screen(int index){
    btn[index].status = 0;

    if(index > lvl_info->page_number * 20){
	int width = Window_Info::get_width();
	width += width/2;
	lvl_info->page_number++;
	for (int i = 1; i < btn.size(); i++) {
	    btn[i].pos.x -= width;
	}
    }
    
}

void Layer::open(){
    int width = Window_Info::get_width();
    is_opening = true;
    is_closing = false;
    for (int i = 0; i < btn.size(); i++)
	btn[i].pos.w = 0;
}

void Layer::close(){
    is_opening = false;
    is_closing = true;
    level_page = false;
}

Layer::~Layer()
{
    if(level_button_texture) SDL_DestroyTexture(level_button_texture);
}

int Layer::do_event(SDL_Event& event){
    if(event.type == SDL_QUIT)
	return 0;
    int width = Window_Info::get_width();

    if(!is_opening){
	if(event.type == SDL_MOUSEBUTTONDOWN &&
	   event.button.button == SDL_BUTTON_LEFT){
	    if(!is_pressed){
		if(!scroll_animation){
		    // scroll_animation = false;
		    if(level_page){
			if(check_scroll_area(event.button.x, event.button.y)){
			    is_scrolling = true;
			    click_pos = event.button.x;
			}
			else
			    is_scrolling = false;

			lvl_info->time_ticks = SDL_GetTicks();
		    }
		    is_pressed = true;
		    is_moving = false;

		}
	    }
	}
	else if(event.type == SDL_MOUSEBUTTONUP &&
		event.button.button == SDL_BUTTON_LEFT){
	    if(is_pressed){
		is_pressed = false;
		if(!scroll_animation){
		    if(is_scrolling){
			float time_elapsed = (SDL_GetTicks() - lvl_info->time_ticks) / 1000.0f;
			lvl_info->time_ticks = 0;
						
			int distance = btn[1].pos.w;
			int new_x = event.button.x;
			
			printf("distance = %d\n",distance);
			
			if(level_page){
			    bool SlideMoves = ((time_elapsed < 0.2f) && is_moving) ||
				(abs(lvl_info->move_capacity) > btn[1].pos.w)
				? true : false;

			    if(SlideMoves)
			    {
				printf("forward!\n");
				if(new_x > click_pos)
				    prev_page(lvl_info->move_capacity, 0);
				else if(new_x < click_pos)
				    next_page(-lvl_info->move_capacity, 0);
			    }
			    else
			    {
				printf("back!\n");
				if(new_x > click_pos)
				    next_page(-lvl_info->move_capacity, 1);
				else if(new_x < click_pos)
				    prev_page(lvl_info->move_capacity, 1);
			    }
			}
			
			return (is_moving || (btn[0].status == 1)) ? -1 : click_index;
		    }
		    else{
			printf("return from not scrolling!\n");
			
			if(lvl_info)
			{
			    delete lvl_info;
			    lvl_info = 0;
			    printf("lvl_info is deleted!\n");
			}
			
			return click_index;
		    }
		}
	    }
	}
	else if(event.type == SDL_MOUSEMOTION){
	    if(is_pressed){
		if(level_page && !scroll_animation){
		    if(is_scrolling){
			for (int i = 1; i < btn.size(); i++)
			    btn[i].pos.x += event.motion.xrel;
			lvl_info->move_capacity += event.motion.xrel;
			if(!is_moving){
			    if(abs(lvl_info->move_capacity) > (0))
				is_moving = true;
			}
		    }
		}
	    }
	}
    }
    return -1;
}

void Layer::prev_page(int offset, bool is_default){
    if(!level_page) return;
    printf("prev_page()\n");
    
    int width = Window_Info::get_width();
    width += width/2;
    if(btn[1].pos.x < 0){
	if(!is_default){
	    lvl_info->scroll_target = btn[1].pos.x + (width - offset);
	    update_page_number(-1);
	    update_page_info_texture();
	}
	else
	    lvl_info->scroll_target = btn[1].pos.x - offset;
    }
    else
	lvl_info->scroll_target = btn[1].pos.x - offset;
    
    scroll_animation = true;
}

void Layer::next_page(int offset, bool is_default){
    if(!level_page) return;
    printf("next_page()\n");
    
    int width = Window_Info::get_width();

    if(btn[btn.size()-1].pos.x > width){
	width += width/2;
	printf("true!\n");
	if(!is_default){
	    lvl_info->scroll_target = btn[1].pos.x - (width - offset);
	    update_page_number(+1);
	    update_page_info_texture();

	}
	else
	    lvl_info->scroll_target = btn[1].pos.x + offset;
    }
    else{
	lvl_info->scroll_target = btn[1].pos.x + offset;
	printf("sas!\n");
    }
    
    scroll_animation = true;
}

void Layer::check_input(){
    int x,y;
    SDL_GetMouseState(&x, &y);
    
    for (int i = 0; i < btn.size(); i++) {
	if(x < btn[i].pos.x)                     continue;
	else if(x > btn[i].pos.x + btn[i].pos.w) continue;
	else if(y < btn[i].pos.y)                continue;
	else if(y > btn[i].pos.y + btn[i].pos.h) continue;
	else{
	    if(!btn[i].status){
		click_index = i;
		return;
	    }
	}
    }
    click_index = -1;
}

bool Layer::check_scroll_area(int x, int y){
	if(x < lvl_info->scroll_bounds.x)                        return false;
	else if(x > lvl_info->scroll_bounds.x + lvl_info->scroll_bounds.w) return false;
	else if(y < lvl_info->scroll_bounds.y)                   return false;
	else if(y > lvl_info->scroll_bounds.y + lvl_info->scroll_bounds.h) return false;
	else return true;
}

void Layer::render_text(SDL_Rect& pos, std::vector<std::string>& str_array, int status){
    if(str_array[0].empty())
	return;
    if(pos.w <= 10)
	return;
    SDL_Renderer *RenderScreen = Window_Info::get_renderer();
    
    SDL_Color color;
    if(status)
    {
	color = {0, 0, 0};
    }
    else
    {
	color = {255, 255, 255};
    }
    
    SDL_Rect rect;
    for (int i = 0; i < str_array.size(); i++) {
	const char *c_string = str_array[i].c_str();
	SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, c_string, color);
	if(!tmp_surface){
	    printf("Failed at Menu::draw_button() - %s\n", TTF_GetError());
	    return;
	}
	rect.w = tmp_surface->w;
	rect.h = tmp_surface->h;
	rect.x = pos.x + (pos.w>>1) - (rect.w>>1);
	rect.y = pos.y + (pos.h>>1) - (rect.h>>1);
	if(i!=0)
	    rect.y += rect.h;
    
	SDL_Texture *tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
	if(!tmp_texture){
	    printf("Failed at Menu::draw_button() - %s\n", IMG_GetError());
	    SDL_FreeSurface(tmp_surface);
	    return;
	}
	
	SDL_RenderCopy(RenderScreen, tmp_texture, 0, &rect);
	SDL_FreeSurface(tmp_surface);
	SDL_DestroyTexture(tmp_texture);
    }
}

void Layer::update_page_number(int num){
    if(num > 0 && lvl_info->page_number < 5)
	lvl_info->page_number++;
    else if(num < 0 && lvl_info->page_number > 1)
	lvl_info->page_number--;

    lvl_info->page_info = std::to_string(lvl_info->page_number) + " / " + std::to_string(lvl_info->page_number_max);
}

void Layer::update_page_info_texture(){
    if(page_info_texture)
	SDL_DestroyTexture(page_info_texture);
    
    SDL_Renderer *RenderScreen = Window_Info::get_renderer();
    
    SDL_Color color = {255, 255, 255};
    const char *c_string = lvl_info->page_info.c_str();
    
    SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, c_string, color);
    if(!tmp_surface){
	printf("Failed at Menu::draw_button() - %s\n", TTF_GetError());
	return;
    }
    
    lvl_info->page_info_rect.w = tmp_surface->w;
    lvl_info->page_info_rect.h = tmp_surface->h;
    lvl_info->page_info_rect.x = btn[0].pos.x + (btn[0].pos.w>>1) - (lvl_info->page_info_rect.w>>1);
    lvl_info->page_info_rect.y = btn[0].pos.y - (lvl_info->page_info_rect.h*2);

    page_info_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
    if(!page_info_texture){
	printf("Failed at Menu::draw_button() - %s\n", IMG_GetError());
	SDL_FreeSurface(tmp_surface);
	return;
    }
    SDL_FreeSurface(tmp_surface);
}

void Layer::delete_page_info(){
    SDL_DestroyTexture(page_info_texture);
    page_info_texture = nullptr;
}

bool Layer::update_button_animation(){
    int width = Window_Info::get_width();
    int t_w  = 0;
    bool done = true;
    for (int i = 0; i < btn.size(); i++) {
	if(is_opening){
	    if(btn[i].is_button) t_w = 60;
	    else t_w = (width*2)/3;
	    
	    if(btn[i].pos.w < t_w){
		btn[i].pos.w += speed;
		btn[i].pos.x -= speed/2;
		done = false;
	    }
	}
	else if(is_closing){
	    if(btn[i].pos.w > t_w){
		btn[i].pos.w -= speed;
		btn[i].pos.x += speed/2;
		done = false;
	    }
	}
    }
    if(is_opening && done){
	is_opening = false;
    }
    else if(is_closing && done){
	is_closing = false;
	if(page_info_texture)
	    delete_page_info();
	return 1;
    }
    return 0;
}

void Layer::animate_scroll(){
    if(scroll_animation){
	const float max_vel = 30.0; //TODO: calculate this according the screen width
	
	Vector2 desired;
	desired.x = lvl_info->scroll_target - btn[1].pos.x;
	
	float abs_distance = abs(desired.x);
	if(abs_distance < max_vel){
	    for (int i = 1 ; i < btn.size(); i++)
		btn[i].pos.x += desired.x;
	    
	    scroll_animation = false;
	    lvl_info->move_capacity = 0;
	    lvl_info->velocity.mult(0);
	    return;
	}

	lvl_info->velocity.add(desired);
	lvl_info->velocity.limit(max_vel);
	for (int i = 1 ; i < btn.size(); i++)
	    btn[i].pos.x += lvl_info->velocity.x;
    }
}

bool Layer::update(){
    animate_scroll();
    check_input();
    if(update_button_animation())
	return 1;

    return 0;
}

void Layer::render(){
    SDL_Renderer *RenderScreen = Window_Info::get_renderer();
    SDL_SetRenderDrawColor(RenderScreen, 0, 0, 0, 255);
    for (int i = 0; i < btn.size(); i++)
    {
	if(level_page)
	{
	    // if(click_index != i && btn[i].status != 1)
	    // {
		SDL_RenderCopy(RenderScreen, level_button_texture, 0, &btn[i].pos);
	    // }
	    // else
	    // {
	    // 	SDL_RenderCopy(RenderScreen, highlight_texture, 0, &btn[i].pos);
	    // }
	
	    render_text(btn[i].pos, btn[i].str_array, btn[i].status);
	}
	else
	{
	    if(click_index != i)
	    {
		SDL_RenderCopy(RenderScreen, button_texture, 0, &btn[i].pos);
	    }
	    else
	    {
		SDL_RenderCopy(RenderScreen, highlight_texture, 0, &btn[i].pos);
	    }
	    
	    render_text(btn[i].pos, btn[i].str_array, btn[i].status);
	}
    }
    if(lvl_info)
	SDL_RenderCopy(RenderScreen, page_info_texture, 0, &lvl_info->page_info_rect);
}

Menu::~Menu(){
    if(button_texture) SDL_DestroyTexture(button_texture);
    if(font) TTF_CloseFont(font);
    
    button_texture = 0;
    font = 0;
}

void Menu::init_texture(SDL_Texture *& texture, const char* path){
    SDL_Renderer* RenderScreen = Window_Info::get_renderer();

    SDL_Texture *tmp_texture = 0;
    SDL_Surface *tmp_surface = 0;
     
    tmp_surface = IMG_Load(path);
    if(!tmp_surface){
	printf("Failed loading tmp_surface! - %s\n ",SDL_GetError());
	return;
    }
     
    tmp_texture = SDL_CreateTextureFromSurface(RenderScreen, tmp_surface);
    if(!tmp_texture){
	printf("Failed loading tmp_texture! - %s\n ", SDL_GetError());
	SDL_Log("Failed loading tmp_surface! - %s\n ");
	SDL_FreeSurface(tmp_surface);
	return;
    }

    SDL_FreeSurface(tmp_surface);
    
    texture = tmp_texture;
}

void Menu::init_font(){
    TTF_Font* tmp_font = TTF_OpenFont(font_path, font_size);
    if(!tmp_font){
	printf("Failed opening font! - %s\n",TTF_GetError());
	SDL_Log("Failed loading tmp_surface! - %s\n ");
	return;
    }
    font = tmp_font;
}

Menu::Menu(Select_Menu menu){
    menu_layer = menu;

    init_font();
    init_texture(button_texture, button_texture_path);
    init_texture(highlight_texture, button_highligth);
    
    Layer new_layer;
    layer.push(new_layer);
    layer.top().init_layer(menu, button_texture, highlight_texture, font);
    layer.top().open();
    is_loading = true;

}

// void Menu::switch_to(Select_Menu menu){



// }

int Menu::handle_event(SDL_Event& event){
    //TODO: elaborate that
    if(event.type == SDL_QUIT)
	return EXIT;
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
	return EXIT;
    
    int result = layer.top().do_event(event);
    Select_Menu menu_choice = menu_layer;
    if(result != -1){
	switch(menu_layer){
	    case MAIN_PAGE:{
		switch(result){
		    case 0: menu_choice = DIFF_PAGE; break;
		    case 1: menu_choice = SETTINGS_PAGE; break;
		}
	    } break;
	    case DIFF_PAGE:{
		switch(result){
		    case 0: menu_choice = NORMAL_PAGE; break;
		    case 1: menu_choice = EASY_PAGE;   break;
		    case 2: menu_choice = HARD_PAGE;   break;
		    case 3:{
			layer.top().close();
			menu_layer = MAIN_PAGE;
			return NOTHING;
		    }
		}
	    } break;
	    case EASY_PAGE:
	    case NORMAL_PAGE:
	    case HARD_PAGE:{
		switch(result){
		    case 0:{
			layer.top().close();
			menu_layer = DIFF_PAGE;
			return NOTHING;
		    }break;
		    default:{
			printf("result = %d\n",result);
			return result;
		    }
		}
	    } break;
	    case SETTINGS_PAGE:{
		switch(result){
		    case 0:{
		    	// layer.top().close();
		    	// menu_layer = MAIN_PAGE;
			printf("0\n");
		    	return NOTHING;
		    }
		    case 1:{
			// layer.top().close();
			menu_choice = ABOUT_PAGE;
			printf("about_page!\n");
			break;
		    } 
		    case 2:{
			layer.top().close();
			menu_layer = MAIN_PAGE;
			printf("back!\n");
			return NOTHING;
		    }
		}
	    } break;
	    case ABOUT_PAGE:{
		switch(result){
		    case 2:{
			layer.top().close();
			menu_layer = SETTINGS_PAGE;
			return NOTHING;
		    }break;
		}
	    }break;
	}


	is_loading = true;
	menu_layer = menu_choice;
	layer.top().close();
    }

    switch(menu_choice){
	case EASY_PAGE: {
	    lvl_mode = "easy";
	    return EASY;
	}
	case NORMAL_PAGE: {
	    lvl_mode = "normal";
	    return NORMAL;   
	}
	case HARD_PAGE:{
	    lvl_mode = "hard";
	    return HARD;
	}
    }
    return NOTHING;
}

const std::string& Menu::get_lvl_mode(){
    return lvl_mode;
}

void Menu::update(){
    int result = 0;
    result = layer.top().update();
    if(result){
	if(is_loading){
	    Layer new_layer;
	    layer.push(new_layer);
	    layer.top().init_layer(menu_layer, button_texture, highlight_texture, font);
	    layer.top().open();
	    is_loading = false;
	}
	else{
	    layer.pop();
	    layer.top().open();
	}
    }
}

void Menu::update_button(int index){
    if(layer.top().is_level_screen())
	layer.top().update_level_screen(index);
}

void Menu::render(){
    layer.top().render();
}



#endif
