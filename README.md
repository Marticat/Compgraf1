## Compgraf1

# Assignment 1 – OpenGL Basics


## Task 1 – Getting Started

Compile a first OpenGL program.
Draw a red triangle.
Modify it to show a blue square (fitting the whole window).

## Task 2 – Drawing 2D Polygons with Color

Use modern OpenGL to draw shapes (triangle, ellipse, circle, stacked squares).
Pass attributes (vPosition, vColor) to shaders.
Use triangle-based primitives (GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP).
No deprecated OpenGL functions were used.

## Program Descriptions
red_triangle.cpp

Opens an OpenGL window 800×600.
Draws a red triangle in the center.
Uses shaders (vertexShaderSource, fragmentShaderSource) and buffer objects.

blue_square.cpp
Based on red_triangle.cpp, but modified.

Changes made:

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

Opens a 500×500 window.
Draws several 2D shapes:
Triangle – each vertex has its own color (red, green, blue), colors interpolate.
Ellipse – centered at (-0.6, 0.6), created with GL_TRIANGLE_FAN, Y-axis scaled 60%.
Circle with gradient – centered at (0.6, 0.6), red intensity changes with angle.
Stacked squares – drawn with GL_TRIANGLE_STRIP, colors alternate between black/white.

## Attributes:

vPosition – vertex position (x, y)
vColor – vertex color (r, g, b)

## Task2part2.cpp

A creative extension of Part 1, adding more variation: 
Triangle constructed using angle-based coordinates (2π/3 increments) Ellipse with center (-0.6, 0.6), red shading, Y scaled 60% 
Circle with smooth red gradient based on parametric angle function Stacked concentric squares drawn using angle-based corner placement (at π/4 and increments of π/2). 
Colors form a grayscale gradient from black to white This part demonstrates: 
Use of parametric equations (cos, sin) for shape generation Color variation as functions of angle 
Procedural construction of vertices instead of hardcoding



## Build Instructions
Requirements

C++11 (or newer)
OpenGL 3.3 Core Profile
GLFW
GLAD

Steps

Unzip the project.
Go into the folder.
Run one of the programs (for example):

./red_triangle
./blue_square
./task2
./task2part2


### VS Code / Visual Studio note:
If you are using Visual Studio, make sure that only one .cpp file is marked as "included in build". Go to:
Properties → Excluded From Build → set "No" for the file you want, and "Yes" for the rest.

## Library Setup (if needed)

If you don’t already have GLFW/GLAD, you can use vcpkg:

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install glfw3 glew glm

## Usage
Tested Environment
OS: Windows 11
Compiler: g++ 11.4.0
OpenGL: 3.3 Core Profile
GLFW: 3.3.8
GLAD: generated for OpenGL 3.3


Pictures with results are in pdf file
How to use parametric equations (cos, sin) to create circles and ellipses.

How small shader/vertex changes (like switching red → blue) completely change the result.
