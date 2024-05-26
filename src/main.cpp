#include <iostream>

#include "window.h"
#include "shader.h"
#include "camera.h"
#include "renderer.h"
#include "transform.h"

#include "entity.h"

/*#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "gltf/tiny_gltf.h"*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDL2/SDL.h"
#include "glad/glad.h"

//Scene Stuff
Window mainWindow;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Input
void ProcessInput(float);

int main( int argc, char* args[] )
{   
    //Load Window
    mainWindow = InitWindow();

    //Test Entity
    Entity duck = entityManager.CreateEntity();
    models[duck] = loadModel("res/models/duck.gltf");
    transforms[duck].scale = glm::vec3(0.01f, 0.01f, 0.01f);
    
    //Shader
    glm::vec3 sun_position = glm::vec3(3.0, 10.0, -5.0);
    glm::vec3 sun_color = glm::vec3(1.0, 1.0, 1.0);
    shaders[duck] = LoadShader("res/shaders/sun.vert", "res/shaders/sun.frag", 2);
    shaders[duck].uniforms = (Uniform*)malloc(sizeof(Uniform) * 2);
    shaders[duck].uniforms[0] = Uniform(UVec3, "sun_position", (float*)&sun_position, shaders[duck].ShaderProgram);
    shaders[duck].uniforms[1] = Uniform(UVec3, "sun_color", (float*)&sun_color, shaders[duck].ShaderProgram);
    
    while (mainWindow.Running)
    {   
        //Window stuff, ie deltatime
        mainWindow.Tick();

        //Input
        ProcessInput(mainWindow.deltaTime);

        //Prerender
        glViewport(0, 0, mainWindow.SCREEN_WIDTH, mainWindow.SCREEN_HEIGHT);
        glClearColor(0.3f, 0.2f, 0.8f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Camera Stuff
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)mainWindow.SCREEN_WIDTH / (float)mainWindow.SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //Draw each object
        for(int i = 0; i < MAX_ENTITIES; i++)
        {   
            if(entityManager.isAlive[i])
            {
                glm::mat4 mvp = projection * view * transforms[i].GetMatrix();
                shaders[duck].UseShader(mvp);
                DrawObject(models[duck]);
            }
        }

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