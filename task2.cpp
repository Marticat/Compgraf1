
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 vColor;
out vec3 color;
void main() {
    color = vColor;
    gl_Position = vec4(vPosition, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;
void main() {
    FragColor = vec4(color, 1.0);
}
)";

// hlpr for shader
GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        std::cerr << "Shader error: " << info << std::endl;
    }
    return shader;
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Assignment1 Task2", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n"; return -1;
    }

    // shdr program
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs); glDeleteShader(fs);

    // triangle
    float tri[] = {
        0.0f,  0.7f,   1.0f, 0.0f, 0.0f,  
       -0.3f,  0.2f,   0.0f, 1.0f, 0.0f,  
        0.3f,  0.2f,   0.0f, 0.0f, 1.0f  
    };


    GLuint VAO_tri, VBO_tri;
    glGenVertexArrays(1, &VAO_tri);
    glGenBuffers(1, &VBO_tri);
    glBindVertexArray(VAO_tri);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tri);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ellipse
    std::vector<float> ellipse;
    float ex = -0.6f, ey = 0.6f, rx = 0.2f, ry = 0.12f;
    int seg = 80;
    ellipse.push_back(ex); ellipse.push_back(ey); ellipse.push_back(1.0f); ellipse.push_back(0.0f); ellipse.push_back(0.0f);
    for (int i = 0; i <= seg; i++) {
        float a = (float)i / seg * 2.0f * (float)M_PI;
        ellipse.push_back(ex + rx * cosf(a));
        ellipse.push_back(ey + ry * sinf(a));
        ellipse.push_back(1.0f); ellipse.push_back(0.0f); ellipse.push_back(0.0f);
    }
    GLuint VAO_e, VBO_e;
    glGenVertexArrays(1, &VAO_e);
    glGenBuffers(1, &VBO_e);
    glBindVertexArray(VAO_e);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_e);
    glBufferData(GL_ARRAY_BUFFER, ellipse.size() * sizeof(float), ellipse.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // circle with the gradient
    std::vector<float> circle;
    float cx = 0.6f, cy = 0.6f, r = 0.2f;
    circle.push_back(cx); circle.push_back(cy); circle.push_back(1.0f); circle.push_back(0.0f); circle.push_back(0.0f);
    for (int i = 0; i <= seg; i++) {
        float a = (float)i / seg * 2.0f * (float)M_PI;
        float red = 0.3f + 0.7f * (0.5f * (1.0f - cosf(a)));

        circle.push_back(cx + r * cosf(a));
        circle.push_back(cy + r * sinf(a));
        circle.push_back(red); circle.push_back(0.0f); circle.push_back(0.0f);
    }
    GLuint VAO_c, VBO_c;
    glGenVertexArrays(1, &VAO_c);
    glGenBuffers(1, &VBO_c);
    glBindVertexArray(VAO_c);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
    glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(float), circle.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

 
    // sqaures
    const int N = 6; // count of sq
    std::vector<GLuint> VAO_sq(N), VBO_sq(N);
    glGenVertexArrays(N, VAO_sq.data());
    glGenBuffers(N, VBO_sq.data());

    for (int k = 0; k < N; k++) {
        float size = 0.6f - k * 0.1f;   
        float y_offset = -0.3f;         //fix: for lift

        
        float col = (k % 2 == 0 ? 1.0f : 0.0f);

        float verts[] = {
            -size, y_offset - size, col, col, col, 
             size, y_offset - size, col, col, col,
            -size, y_offset + size, col, col, col, 
             size, y_offset + size, col, col, col  
        };

        glBindVertexArray(VAO_sq[k]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_sq[k]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }



    // rendr
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        glBindVertexArray(VAO_tri);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO_e);
        glDrawArrays(GL_TRIANGLE_FAN, 0, seg + 2);

        glBindVertexArray(VAO_c);
        glDrawArrays(GL_TRIANGLE_FAN, 0, seg + 2);

        for (int k = 0;k < N;k++) {
            glBindVertexArray(VAO_sq[k]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
