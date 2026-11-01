// Textures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*

    DISPLAYS AN IMAGE AS A TEXTURE

    CRATE IMAGE 



*/

#include <iostream>
#include <sstream>

// Includes for OpenGL - open a window on Windows
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Texture2D.h"

// Constants for creating a window
const char* APP_TITLE = "Introduction to Modern OpenGL - Textures";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = NULL;
// wireframe mode
bool gWireframe = false;
const char* textureFile = "textures/wooden_crate.jpg";

// Keyboard callback
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
// fps
void showFPS(GLFWwindow* window);
// claen up initialization
bool initOpenGL();

int main()
{
    if (!initOpenGL())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    // vertices for triangle
    GLfloat vertices[] = {
        // position         //tex cords
	   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right     
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f  // bottom left  
    };

    // indices for triangle
    GLuint indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    //vbo - vertex buffer object
    GLuint vbo, ibo, vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //vao - vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // set vertex attributes pointer - position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    // tex cords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //ibo - index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("basic.vert", "basic.frag");

	Texture2D texture;
	texture.loadTexture(textureFile);

    // main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // get fps
        showFPS(gWindow);

        glfwPollEvents();

        // clear color
        glClear(GL_COLOR_BUFFER_BIT);

        texture.bind();

        shaderProgram.use();

        // draw triangle
        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // buffer on screen - buffer we are currently drawing on
        glfwSwapBuffers(gWindow);
    }

    // clean up vbo and vao and ibo
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    // clean up glfw Init
    glfwTerminate();

    return 0;

}

// clean up initialization
bool initOpenGL()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW Initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);


    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);


    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        // opposite of Init - destroy object
        glfwTerminate();
        return false;
    }

    // make window focus
    glfwMakeContextCurrent(gWindow);

    // set callback function for keyboard
    glfwSetKeyCallback(gWindow, glfw_onKey);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW Initialization failed" << std::endl;

        return false;
    }

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    return true;
}

// implement callback Key function
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        gWireframe = !gWireframe;
        if (gWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

//show fps
void showFPS(GLFWwindow* window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds GLFW started

    elapsedSeconds = currentSeconds - previousSeconds;

    // limit text 4 time per second
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << APP_TITLE << "    "
            << "FPS: " << fps << "     "
            << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());
        frameCount = 0;

    }

    frameCount++;
}

