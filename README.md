## Compgraf1

# Assignment 1 – OpenGL Basics
# Task 1. 
## Program Descriptions
red_triangle.cpp
This program creates an OpenGL window (800x600) and displays a single red triangle in the center of the screen. 

blue_square.cpp (modified from red_triangle.cpp)
This program creates an OpenGL window (800x600) and displays a single blue square that fits inside the window.
Changes made to red_triangle.cpp:

1:Modified fragment shader to use blue instead of red.

FragColor = vec4(0.0, 0.0, 1.0, 1.0);

2:Replace triangle vertices with square vertices (two triangles)
FROM:

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

TO:

float vertices[] = {

    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f
};

Update draw call to use 6 vertices instead of 3

glDrawArrays(GL_TRIANGLES, 0, 6);


## Task 2. Drawing 2D Polygons with Color
This program creates a 500×500 window and draws the following objects:

Triangle – with per-vertex colors (interpolated red, green, blue).
Ellipse – centered at (-0.6, 0.6), drawn with GL_TRIANGLE_FAN, scaled y-radius to 60%.
Circle with gradient shading – centered at (0.6, 0.6), red intensity varies smoothly as a function of angle.
Stacked Squares – multiple squares drawn on top of each other using GL_TRIANGLE_STRIP. Colors alternate from black to white, giving a layered effect.
All shapes are drawn using modern OpenGL (Core Profile, no deprecated functions).

# Attributes:

vPosition – vertex position (x, y)
vColor – vertex color (r, g, b)


Task2part2.cpp

A creative extension of Part 1, adding more variation: 
Triangle constructed using angle-based coordinates (2π/3 increments) Ellipse with center (-0.6, 0.6), red shading, Y scaled 60% 
Circle with smooth red gradient based on parametric angle function Stacked concentric squares drawn using angle-based corner placement (at π/4 and increments of π/2). 
Colors form a grayscale gradient from black to white This part demonstrates: 
Use of parametric equations (cos, sin) for shape generation Color variation as functions of angle 
Procedural construction of vertices instead of hardcoding

Build Instructions

Requirements

C++ compiler with C++11 (or newer)
OpenGL 3.3+
GLFW
GLAD
Make utility

# Compilation

Unzip the project.
Enter the directory 
In VS please make sure that your chosen file is not Excluded From Build
Go to properties -> write "No" on Excluded From Build field
Be sure that only 1 file is not excluded (other files must be excluded(write "yes" on the field))
Run the project

P.S if there are any problems with libraries
download glad and write commands in cmd

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install glfw3 glew glm
vcpkg install glew
## Usage

### Tested Environment
OS: Win11
Compiler: g++ 11.4.0
OpenGL: 3.3 Core Profile
GLFW: 3.3.8
GLAD: generated for OpenGL 3.3 Core
