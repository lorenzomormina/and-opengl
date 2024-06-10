#include <iostream>
#include "platform.h"
#include "shader.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Btn1.h"
#include "Btn2.h"
#include "BtnOp.h"
#include "BtnRes.h"
#include "text.h"
#include "callbacks.h"
#include "globals.h"


#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

void processInput(GLFWwindow *window);




int main()
{
    initWindow();

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSwapInterval(0);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable blending
    glEnable(GL_BLEND);

    // Set blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);

    // set up opengl machinery; (see inside initWindow()).

    buildShaders();

    // load assets

    loadFont();

    // set up objects

    andGate.load(textureShader);
    led1.load(ledTexShader, 1);
    led2.load(ledTexShader, 2);
    led3.load(ledTexShader, 3);
    expr.load(textShader);

    // render loop
    // -----------
    // NOTE: framerate limiter not implemented
    int prevFrame = 0;
    int frameCount = 0;
    double prevSecond = 0;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // NOTE: inconsistent api
        glUseProgram(rectangleShader);
        andGate.draw(textureShader);
        led1.draw(ledTexShader);
        led2.draw(ledTexShader);
        led3.draw(ledTexShader);
        expr.draw();


        RenderText(textShader, "AND OpenGL", 540.0f, 30.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        RenderText(textShader, std::to_string(prevFrame), 540.0f, 100.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        frameCount++;

        auto tempSecond = glfwGetTime();
        if (tempSecond - prevSecond > 1.0)
        {
            prevSecond = tempSecond;
            prevFrame = frameCount; 
            frameCount = 0;
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        
    }

    // TODO: unload assets
    // ...

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

