#include <iostream>

#include "window.h"
#include "shader.h"
//Separate into src file
#include "camera.h"

/*#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "gltf/tiny_gltf.h"*/
#include "renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "transform.h"

#include "SDL2/SDL.h"
#include "glad/glad.h"

//Big stuff
Window mainWindow;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void ProcessInput(float);

int main( int argc, char* args[] )
{   
    //Load Window
    mainWindow = InitWindow();
    //Load Shader
    Shader shader = LazyLoadShader("res/shaders/sun.vert", "res/shaders/sun.frag");
    //Load Model
    Model duck = loadModel("res/models/Duck.gltf");
    //Create Transform
    Transform transform;

    glm::vec3 sun_position = glm::vec3(3.0, 10.0, -5.0);
    glm::vec3 sun_color = glm::vec3(1.0);

    //Event Loop
    while (mainWindow.Running)
    {   
        mainWindow.Tick();

        //Input
        ProcessInput(mainWindow.deltaTime);

        //Prerender
        glViewport(0, 0, mainWindow.SCREEN_WIDTH, mainWindow.SCREEN_HEIGHT);
        glClearColor(0.3f, 0.2f, 0.8f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)mainWindow.SCREEN_WIDTH / (float)mainWindow.SCREEN_HEIGHT, 0.1f, 100.0f);
        shader.SetUniformMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.SetUniformMat4("view", view);

        glm::mat4 model = glm::mat4(1.0);
        shader.SetUniformMat4("model", transform.GetMatrix());

        //TODO - Shader Debugging not working???
        //Finding position every frame??
        shader.SetUniformVec3("sun_color", sun_color);
        shader.SetUniformVec3("sun_position", sun_position);

        drawModel(duck);

        //Last Rendering thingy
        SDL_GL_SwapWindow(mainWindow.window);
    }
    
    //Cleanup
    SDL_DestroyWindow(mainWindow.window);
    SDL_Quit();

    return 0;
}


void ProcessInput(float deltaTime)
{       
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {   
            switch(Event.type)
            {
                case SDL_KEYDOWN: 
                    switch (Event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            mainWindow.Running = 0;
                            break;
                        case 'f':
                            mainWindow.FullScreen = !mainWindow.FullScreen;
                            if (mainWindow.FullScreen)
                            {
                                SDL_SetWindowFullscreen(mainWindow.window, mainWindow.flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                            }
                            else
                            {
                                SDL_SetWindowFullscreen(mainWindow.window, mainWindow.flags);
                            }
                            break;
                        default:
                            break;
                    }
                break;
                case SDL_QUIT: mainWindow.Running = 0;
                break;
                case SDL_MOUSEMOTION: camera.ProcessMouseMovement((float)Event.motion.xrel, (float)-Event.motion.yrel); //mouseMovement(Event.motion.x, Event.motion.y);
                break;

            }
        }
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_D])
        {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if(keystate[SDL_SCANCODE_A])
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if(keystate[SDL_SCANCODE_W])
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if(keystate[SDL_SCANCODE_S])
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if(keystate[SDL_SCANCODE_SPACE])
        {
            camera.ProcessKeyboard(UP, deltaTime);
        }
        if(keystate[SDL_SCANCODE_LSHIFT])
        {
            camera.ProcessKeyboard(DOWN, deltaTime);
        }
}