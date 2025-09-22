// red_triangle.cpp
// Draw a single red triangle using modern OpenGL (GLFW + GLAD)
// Build: see Makefile

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 vColor;

out vec3 color;

void main()
{
    color = vColor;
    gl_Position = vec4(vPosition, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;
void main()
{
    FragColor = vec4(color, 1.0);
}
)";

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return -1;

    // Request OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Red Triangle", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Compile shaders
    auto compile = [](GLenum type, const char* src) -> GLuint {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, NULL);
        glCompileShader(s);
        GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char buf[1024]; glGetShaderInfoLog(s, 1024, NULL, buf);
            std::cerr << "Shader compile error: " << buf << std::endl;
        }
        return s;
        };

    GLuint vs = compile(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    GLint linked; glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        char buf[1024]; glGetProgramInfoLog(program, 1024, NULL, buf);
        std::cerr << "Program link error: " << buf << std::endl;
    }
    glDeleteShader(vs); glDeleteShader(fs);

    // Triangle vertex data: positions (x,y) and color (r,g,b) - red
    float vertices[] = {
        // positions     // colors
         0.0f,  0.5f,     1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,     1.0f, 0.0f, 0.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
