#include <iostream>

#include "SDL2/SDL.h"
//#include "SDL2/SDL_opengl.h"
#include "glad/glad.h"

#include "window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* mainWindow = NULL;
SDL_Surface* screenSurface = NULL;
SDL_GLContext mainContext;

int main( int argc, char* args[] )
{
    //Initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n";
        std::cout << SDL_GetError();
        return -1;
    }

    //Setup Window
    mainWindow = SDL_CreateWindow("3D Browser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(mainWindow == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: %s\n"; 
        std::cout << SDL_GetError();
        return -1;
    }

    //GL Attributes
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Creating GL Context
    mainContext = SDL_GL_CreateContext( mainWindow );
    if(mainContext == NULL)
    {
        std::cout << "OpenGL context could not be created! SDL Error: %s\n";
        std::cout << SDL_GetError();
        return -1;
    }

    //Loading Glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::cout << SDL_GetError();
        return -1;
    }    

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        std::cout << "Warning: Unable to set VSync! SDL Error: %s\n";
        std::cout << SDL_GetError();
    }

    //Make window white
    screenSurface = SDL_GetWindowSurface(mainWindow);
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    SDL_UpdateWindowSurface( mainWindow );

    //Event Loop
    SDL_Event e; bool quit = false; 
    while(SDL_PollEvent( &e ) || quit == false)
    { 
        if(e.type == SDL_QUIT) { quit = true; } 
    } 
    
    //Cleanup
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}