//Usage:
//Hold down the number keys , 1-7, to select one or multiple circles.
//While circle(s) are selected, use the left mouse button to translate and use the right mouse button to rotate.

#include "main.h"
#include "geometry.cpp"
#include "render.cpp"
#include "keyboard.cpp"

void init(void)
{
    for(int i = 0; i<256; i++)
    {
        keyStates[i] = false;
    }

    createBody();
    createArm(1);
    createArm(-1);
    createHead();
    createLeg(1);
    createLeg(-1);

    buttonState = -1;
}

int main(int argc, char *argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(window_size[0], window_size[1]);
    glutCreateWindow("Assignment #2");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;

}