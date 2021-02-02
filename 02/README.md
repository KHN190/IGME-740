## Assignment

You are required to write a 2D drawing program using C++ and OpenGL. The program should have a right-click menu, from which users can choose specifications they want to draw with. 

The program should allow users to (and supports):

* 2D drawing:
	* Point, line, triangle, quad and polygon.
* Mouse
* Keyboard
* Settings:
	* 3 colors
	* 3 line widths
	* 3 point sizes

See example code in src to familiarize the use of callback functions, primitive drawing, and GLUT menu.

* `idle.cpp`
* `interact.cpp`

Build examples:

```bash
mkdir build && cd build && cmake .. && make
```