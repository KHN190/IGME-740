## Assignment

You are required to write a 2D drawing program using C++ and OpenGL. The program should have a right-click menu, from which users can choose specifications they want to draw with. 

The program should allow users to:

* Draw 2D objects including points, lines, triangles, quads and polygons.
* They must be interactively created using GLUT mouse, motion and keyboard callbacks. 
* The right-click menu must contain at least three predefined sizes. A chosen
size is to specify the rasterized diameter of the point.
* The menu must contain at least three predefined line widths. A chosen
width is to specify the width of rasterized lines.
* Your program must use an appropriate keyboard callback. Consider to glutKeyboardFunc(). For example, hitting a key on the keyboard completes the creation
of a polygon.

See example code in src to familiarize the use of callback functions, primitive drawing, and GLUT menu.

* `idle.cpp`
* `interact.cpp`

Build examples:

```bash
mkdir build && cd build && cmake .. && make
```