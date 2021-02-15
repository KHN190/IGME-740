// Copyright 2021 Lingxiao

////////////////////////////////////////////////

#include "main.h"
#include "render.cpp"
#include "menu.cpp"
#include "input.cpp"

////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(raster_size[0], raster_size[1]);
    glutCreateWindow("Mouse Event - draw a triangle");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    createMenu();
    glutMainLoop();
    return 0;
}
