#include <iostream>

#include "window.h"
#include "shader.h"
//Separate into src file
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDL2/SDL.h"
#include "glad/glad.h"

Window mainWindow;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Enable depth test??

int main( int argc, char* args[] )
{   
    //Load Window
    mainWindow = InitWindow();
    //Load Shader
    Shader shader = LazyLoadShader("../res/shaders/vertex.vert", "../res/shaders/fragment.frag");
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    bool Running = 1;
    bool FullScreen = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;

    //Event Loop
    while (Running)
    {   
        //Deltatime
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency() );

        //Input
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch (Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    Running = 0;
                    break;
                case 'f':
                    FullScreen = !FullScreen;
                    if (FullScreen)
                    {
                    SDL_SetWindowFullscreen(mainWindow.window, mainWindow.flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    else
                    {
                    SDL_SetWindowFullscreen(mainWindow.window, mainWindow.flags);
                    }
                    break;
                case 'w': camera.ProcessKeyboard(FORWARD, deltaTime);
                    break;
                case 'a': camera.ProcessKeyboard(LEFT, deltaTime);
                    break;
                case 's': camera.ProcessKeyboard(BACKWARD, deltaTime);
                    break;
                case 'd': camera.ProcessKeyboard(RIGHT, deltaTime);
                    break;
                default:
                    break;
                }
            }
            else if (Event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }

    //Prerender
    glViewport(0, 0, mainWindow.SCREEN_WIDTH, mainWindow.SCREEN_HEIGHT);
    glClearColor(0.3f, 0.2f, 0.8f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)mainWindow.SCREEN_WIDTH / (float)mainWindow.SCREEN_HEIGHT, 0.1f, 100.0f);
    shader.SetUniformMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader.SetUniformMat4("view", view);

    //Per object Basis
    glm::mat4 model = glm::mat4(1.0);
    shader.SetUniformMat4("model", model);

    // Draw Objects
    glUseProgram(shader.ShaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(mainWindow.window);
  }
    
    //Cleanup
    SDL_DestroyWindow(mainWindow.window);
    SDL_Quit();

    return 0;
}