/**
 * @file myglapp.cpp
 * @author Derek Tan
 * @brief Implements application setup code.
 * @version 0.1
 * @date 2023-12-18
 */

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <iostream>

/* Main Constants */

static constexpr const int32_t window_width = 480;
static constexpr const int32_t window_height = 480;
static constexpr const char* window_title = "GL Demo 2";

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

    uint32_t vshader_id;
    const char* vertex_shader_code = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    uint32_t fshader_id;
    const char* frag_shader_code = "#version 330 core\n"
        "out vec4 FragColor;\n\n"
        "void main() {\n"
        "  FragColor = vec4(0.78425f, 0.6275f, 0.8625f, 1.0f);"
        "}\0";

    // setup buffer
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shape_data), shape_data, GL_STATIC_DRAW);

    // setup shaders
    vshader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader_id, 1, &vertex_shader_code, NULL);
    glCompileShader(vshader_id);

    int shader_flag;
    char shader_debug_log[512];
    glGetShaderiv(vshader_id, GL_COMPILE_STATUS, &shader_flag);

    if (!shader_flag)
    {
        glGetShaderInfoLog(vshader_id, 512, NULL, shader_debug_log);
        std::cerr << "Error [Shader Compile]:\n" << shader_debug_log << '\n';
    }

    fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader_id, 1, &frag_shader_code, NULL);
    glCompileShader(fshader_id);
    glGetShaderiv(fshader_id, GL_COMPILE_STATUS, &shader_flag);

    if (!shader_flag)
    {
        glGetShaderInfoLog(fshader_id, 512, NULL, shader_debug_log);
        std::cerr << "Error [Shader Compile]:\n" << shader_debug_log << '\n';
    }

    // Link shaders into GPU program
    uint32_t shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vshader_id);
    glAttachShader(shader_program_id, fshader_id);
    glLinkProgram(shader_program_id);
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &shader_flag);

    if (!shader_flag)
    {
        glGetProgramInfoLog(shader_program_id, 512, NULL, shader_debug_log);
        std::cerr << "Error [Shader Compile]:\n" << shader_debug_log << '\n';
    }

    glUseProgram(shader_program_id);
    glDeleteShader(vshader_id);
    glDeleteShader(fshader_id);

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

    while (!glfwWindowShouldClose(window))
    {
        // process events
        handle_input(window);

        // render whitesmoke background
        glClearColor(0.9375f, 0.9375f, 0.9375f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render lilac triangle
        glUseProgram(shader_program_id);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // show new frame buffer
        glfwSwapBuffers(window);

        // lookup events
        glfwPollEvents();
    }

    // Dispose application resources
    glfwTerminate();

    return 0;
}
