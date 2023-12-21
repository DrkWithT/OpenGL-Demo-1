/**
 * @file myglapp.cpp
 * @author Derek Tan
 * @brief Implements application setup code.
 * @todo Add FPS based render loop.
 * @version 0.1
 * @date 2023-12-18
 */

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <cmath>
#include <iostream>

#include "utils/resources.hpp"

/* Main Constants */

constexpr const char* basic_vert_shader_path = "./resources/basicvertex.glsl";
constexpr const char* basic_frag_shader_path = "./resources/basicfragment.glsl";

constexpr const int32_t window_width = 480;
constexpr const int32_t window_height = 480;
constexpr const char* window_title = "GL Demo 2";

/* Callbacks */

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void handle_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // Setup GLFW configs
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Try to create window
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);

    if (window == NULL)
    {
        std::cerr << __FILE__ << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to setup GLAD bindings.\n";
        glfwTerminate();
        return -1;
    }

    uint32_t vbo_id;
    float shape_data[] = {
        -0.5f, 0.5f, 0.0f, // top left
        0.5f, 0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f // bottom left
    };

    uint32_t draw_indices[] = {
        0, 1, 2, // triangle 1
        2, 3, 0  // triangle 2
    };

    // setup buffer
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shape_data), shape_data, GL_STATIC_DRAW);

    // setup GPU programs of shader code
    resources::GLShaderProgram program {basic_vert_shader_path, basic_frag_shader_path};

    if (!program.getSetupFlag())
    {
        std::cerr << "Error [Shader Check]:\n" << "Check for program setup failed.\n";
        glfwTerminate();
        return -1;
    }

    // Set formatting of vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create VAO to store multiple vertex buffer objects per simple shape... can be used to store all data of a complex drawable!
    uint32_t vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Copy vertex array into VBO for OGL
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shape_data), shape_data, GL_STATIC_DRAW);

    // Put EBO to VAO for vertex draw ordering
    uint32_t ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(draw_indices), draw_indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    glfwSwapInterval(1);

    // Begin window use and rendering
    glViewport(0, 0, window_width, window_height);

    // Dynamic uniform state:
    std::string yDeltaName {"yDelta"};
    float yDelta = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // process events
        handle_input(window);

        // render whitesmoke background
        glClearColor(0.9375f, 0.9375f, 0.9375f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render bobbing lilac box
        program.use();

        program.setUniformFloat(yDeltaName, yDelta);

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // show new frame buffer
        glfwSwapBuffers(window);

        // update uniform state
        yDelta = std::sinf(glfwGetTime()) / 2.0f;

        // lookup events
        glfwPollEvents();
    }

    // Dispose application resources
    glfwTerminate();

    return 0;
}
