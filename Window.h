#ifndef WINDOW
#define WINDOW

#include <SDL.h>

#define PC 1

class Window_Info{
public:
     static Window_Info& get_window_info(){
	  static Window_Info window_info;
	  return window_info;
     }
     
     SDL_Renderer* get_renderer();
     SDL_Window* get_window();
     
     uint32_t get_height();     
     uint32_t get_width();
     
private:
     Window_Info();
     ~Window_Info();

     Window_Info(Window_Info const&);
     void operator=(Window_Info const&);
     
private:
     SDL_Window* window;
     SDL_Renderer* renderer;

     uint32_t window_width = 380;
     uint32_t window_height = 700;
};


#endif
