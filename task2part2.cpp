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

    // shaders
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs); glDeleteShader(fs);

    // ========== Triangle (angle-based) ==========
    // Make a triangle using angle approach (e.g., center near top)
    std::vector<float> tri;
    float t_cx = 0.0f, t_cy = 0.65f, t_r = 0.18f;
    // three vertices at baseAngle, baseAngle + 2pi/3, baseAngle + 4pi/3
    float baseAngle = -M_PI / 2.0f; // pointing up
    for (int i = 0; i < 3; ++i) {
        float a = baseAngle + i * (2.0f * M_PI / 3.0f);
        float x = t_cx + t_r * cosf(a);
        float y = t_cy + t_r * sinf(a);
        // color: vary each vertex a bit
        float r = 0.9f - 0.3f * i;
        float g = 0.1f + 0.4f * i;
        float b = 0.2f + 0.2f * i;
        tri.push_back(x); tri.push_back(y); tri.push_back(r); tri.push_back(g); tri.push_back(b);
    }
    GLuint VAO_tri, VBO_tri;
    glGenVertexArrays(1, &VAO_tri);
    glGenBuffers(1, &VBO_tri);
    glBindVertexArray(VAO_tri);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tri);
    glBufferData(GL_ARRAY_BUFFER, tri.size() * sizeof(float), tri.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ========== Ellipse (60% scaling on y) ==========
    std::vector<float> ellipse;
    float ex = -0.6f, ey = 0.6f, rx = 0.2f, ry = 0.2f * 0.6f; // y scaled to 60%
    int seg = 80;
    // center vertex for fan (color red)
    ellipse.push_back(ex); ellipse.push_back(ey); ellipse.push_back(1.0f); ellipse.push_back(0.2f); ellipse.push_back(0.2f);
    for (int i = 0; i <= seg; ++i) {
        float a = (float)i / seg * 2.0f * M_PI;
        float x = ex + rx * cosf(a);
        float y = ey + ry * sinf(a);
        ellipse.push_back(x); ellipse.push_back(y);
        // perimeter color (light red)
        ellipse.push_back(0.9f); ellipse.push_back(0.3f); ellipse.push_back(0.3f);
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

    // ========== Circle (triangle fan, red varies with angle) ==========
    std::vector<float> circle;
    float cx = 0.6f, cy = 0.6f, r = 0.2f;
    circle.push_back(cx); circle.push_back(cy); circle.push_back(0.9f); circle.push_back(0.0f); circle.push_back(0.0f); // center
    for (int i = 0; i <= seg; ++i) {
        float a = (float)i / seg * 2.0f * M_PI;
        float red = 0.3f + 0.7f * (0.5f * (1.0f - cosf(a))); // varies with angle (same idea as you had)
        float x = cx + r * cosf(a);
        float y = cy + r * sinf(a);
        circle.push_back(x); circle.push_back(y); circle.push_back(red); circle.push_back(0.0f); circle.push_back(0.0f);
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

    // ========== Squares (stacked, angle-based corners, grayscale gradient) ==========
    const int N = 6; // number of squares
    std::vector<GLuint> VAO_sq(N), VBO_sq(N);
    glGenVertexArrays(N, VAO_sq.data());
    glGenBuffers(N, VBO_sq.data());

    // center and base size: we'll draw concentric rotated squares centered lower on screen
    float sq_cx = 0.0f, sq_cy = -0.25f;
    float maxHalf = 0.45f; // largest square half-size
    for (int k = 0; k < N; ++k) {
        float t = (N == 1) ? 0.0f : (float)k / (float)(N - 1); // 0..1
        float half = maxHalf * (1.0f - 0.15f * k); // shrink per layer
        float gray = t; // black->white across k

        // angle base at pi/4; corners every pi/2
        float base = M_PI / 4.0f;
        // We will create a triangle strip with 4 vertices: bottom-left, bottom-right, top-left, top-right (ordering for TRIANGLE_STRIP)
        // But using angle-based corners:
        float corners[4][2];
        for (int c = 0; c < 4; ++c) {
            float a = base + c * (M_PI / 2.0f);
            corners[c][0] = sq_cx + half * cosf(a);
            corners[c][1] = sq_cy + half * sinf(a);
        }
        // Tri-strip order: 0,1,2,3 will produce a strip that covers the square (two triangles)
        float verts[4 * 5];
        for (int i = 0; i < 4; ++i) {
            verts[i * 5 + 0] = corners[i][0];
            verts[i * 5 + 1] = corners[i][1];
            verts[i * 5 + 2] = gray;
            verts[i * 5 + 3] = gray;
            verts[i * 5 + 4] = gray;
        }

        glBindVertexArray(VAO_sq[k]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_sq[k]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        // triangle
        glBindVertexArray(VAO_tri);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ellipse
        glBindVertexArray(VAO_e);
        glDrawArrays(GL_TRIANGLE_FAN, 0, seg + 2);

        // circle
        glBindVertexArray(VAO_c);
        glDrawArrays(GL_TRIANGLE_FAN, 0, seg + 2);

        // squares
        for (int k = 0; k < N; ++k) {
            glBindVertexArray(VAO_sq[k]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
