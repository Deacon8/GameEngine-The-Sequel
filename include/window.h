#pragma once
#include <iostream>
#include "SDL2/SDL.h"
#include "glad/glad.h"

struct Window
{
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_GLContext context;
    int SCREEN_WIDTH = 800;
    int SCREEN_HEIGHT = 600;
    int flags;
    bool Running = 1;
    bool FullScreen = 0;
};typedef struct Window Window;

Window InitWindow()
{   
    Window window;
    //Initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n";
        std::cout << SDL_GetError();
    }

    //Setup Window
    window.flags = SDL_WINDOW_OPENGL;
    window.window = SDL_CreateWindow("3D Browser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window.SCREEN_WIDTH, window.SCREEN_HEIGHT, window.flags);
    window.context = SDL_GL_CreateContext(window.window);

    if(window.window == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: %s\n"; 
        std::cout << SDL_GetError();
    }

    //GL Attributes
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Creating GL Context
    window.context = SDL_GL_CreateContext( window.window );
    if(window.context == NULL)
    {
        std::cout << "OpenGL context could not be created! SDL Error: %s\n";
        std::cout << SDL_GetError();
    }

    //Loading Glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::cout << SDL_GetError();
    }    

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        std::cout << "Warning: Unable to set VSync! SDL Error: %s\n";
        std::cout << SDL_GetError();
    }
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    //Make window white
    window.screenSurface = SDL_GetWindowSurface(window.window);
    SDL_FillRect( window.screenSurface, NULL, SDL_MapRGB( window.screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    SDL_UpdateWindowSurface( window.window );
    return window;
}