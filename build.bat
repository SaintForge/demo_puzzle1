@echo off 
call "D:\Visual Studio\VC\vcvarsall.bat" x86
cl /EHsc -Zi /I"headers/" ^
demo_main.cpp Game_Engine.cpp Figure.cpp Figure_Manager.cpp Grid_Manager.cpp Window.cpp New_Menu.cpp Level_Manager.cpp Game_Data.cpp ^
/I"W:\SDL\sdl_dev_lib\include" ^
/link /LIBPATH:W:\SDL\sdl_dev_lib\lib\x86 ^
SDL2.lib SDL2main.lib SDL2_image.lib SDL2_ttf.lib SDL2_mixer.lib ^
/SUBSYSTEM:CONSOLE

move /y *.obj "build" >NUL
move /y *.ilk "build" >NUL
move /y *.pdb "build" >NUL
move /y *.exe "build" >NUL
