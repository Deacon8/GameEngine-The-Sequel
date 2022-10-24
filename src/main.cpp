#include <iostream>

#include "window.h"
#include "shader.h"
//Separate into src file
#include "camera.h"

//#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "gltf/tiny_gltf.h"
#include "renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDL2/SDL.h"
#include "glad/glad.h"

Window mainWindow;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Enable depth test??
void ProcessInput(float);

int main( int argc, char* args[] )
{   
    //Load Window
    mainWindow = InitWindow();
    //Load Shader
    Shader shader = LazyLoadShader("res/shaders/vertex.vert", "res/shaders/fragment.frag");

    tinygltf::Model duck;
    if (!loadModel(duck, "res/models/Duck.gltf")) return -1;

    std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos = bindModel(duck);

      // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 model_rot = glm::mat4(1.0f);
    glm::vec3 model_pos = glm::vec3(-3, 0, -3);
    glm::vec3 model_scale = glm::vec3(0.01f, 0.01f, 0.01f);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;

    //Event Loop
    while (mainWindow.Running)
    {   
        //Deltatime
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency() );

        //Input
        ProcessInput(deltaTime);

        //Prerender
        glViewport(0, 0, mainWindow.SCREEN_WIDTH, mainWindow.SCREEN_HEIGHT);
        glClearColor(0.3f, 0.2f, 0.8f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Render
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)mainWindow.SCREEN_WIDTH / (float)mainWindow.SCREEN_HEIGHT, 0.1f, 100.0f);
        shader.SetUniformMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.SetUniformMat4("view", view);

        //Per object Basis
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), model_pos);  // reposition model
        model_rot = glm::rotate(model_rot, glm::radians(0.8f), glm::vec3(0, 1, 0));  // rotate model on y axis
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), model_scale);
        model_mat = trans * model_rot * scale;

        glm::mat4 model = glm::mat4(1.0);
        shader.SetUniformMat4("model", model_mat);

        drawModel(vaoAndEbos, duck);

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