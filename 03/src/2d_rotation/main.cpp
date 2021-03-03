//Usage:
//Click the left mouse button to rotate the quad about its center.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
using namespace std;

int win_width = 600, win_height = 600;
float canvas_width = 20.0f, canvas_height = 20.0f;

float v0[2], v1[2], v2[2], v3[2];
float quadCenter[2];
float color[3];
float angle = 0.0f;


void createQuad()
{
    v0[0] = 6;
    v0[1] = 6;
    v1[0] = 6;
    v1[1] = 10;
    v2[0] = 10;
    v2[1] = 10;
    v3[0] = 10;
    v3[1] = 6;

    quadCenter[0] = (v0[0] + v3[0]) / 2.0f;
    quadCenter[1] = (v0[1] + v1[1]) / 2.0f;

    color[0] = 0.5f;
    color[1] = 0.0f;
    color[2] = 0.5f;
}


void init(void)
{
    createQuad();
    angle = 0.0f;
}



void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // three lines of transformation code to roate the quad about its own center
    // read these three lines from bottom to top
    // 1. translate the quad center to the origin
    // 2. rotate the quad about the origin, which is now the center too. rotation angle is updated by mouse click
    // 3. translate the quad back to where the center was
    glTranslatef(quadCenter[0], quadCenter[1], 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f); // note: glrotatef() takes angles in degree, rather than radian.
    glTranslatef(-quadCenter[0], -quadCenter[1], 0.0f);

    // draw the quad
    // Note: the composed matrix implicitly generated from above three lines of code is multiplied with vertices autonatically
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2fv(v0);
    glVertex2fv(v1);
    glVertex2fv(v2);
    glVertex2fv(v3);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    win_width = w;
    win_height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 20, 0, 20);
    glViewport(0, 0, (GLsizei)win_width, (GLsizei)win_height);

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // 'esc' key
        exit(0);

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        angle += 1.5f;
    }
}


int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Quad rotation about its center");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;

}