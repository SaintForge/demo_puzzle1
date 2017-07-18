#ifndef WINDOW
#define WINDOW

#include <SDL.h>

#define PC 1

class Window_Info{
public:
    static SDL_Renderer* get_renderer(){
	return get_window_info().renderer;
    }
    static SDL_Window* get_window(){
	return get_window_info().window;
    }
     
    static uint32_t get_height(){
	return get_window_info().window_height;
    }
    static uint32_t get_width(){
	return get_window_info().window_width;
    }
private:
     Window_Info();
     ~Window_Info();

     Window_Info(Window_Info const&);
     void operator=(Window_Info const&);

    static Window_Info& get_window_info(){
	static Window_Info window_info;
	return window_info;
    }
private:
     SDL_Window* window;
     SDL_Renderer* renderer;

     uint32_t window_width = 380;
     uint32_t window_height = 700;
};


#endif
