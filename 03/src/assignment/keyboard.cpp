#include "main.h"

void specialKeys(int key, int x, int y)
{
    // space
    if (keyStates[32])
    {
        std::cout<< "space key locks special keys\n";
        return;
    }

    switch(key)
    {
        case GLUT_KEY_UP:
        case GLUT_KEY_LEFT:
            curr_part++;
            curr_part %= poly.size();
            break; 

        case GLUT_KEY_DOWN:
        case GLUT_KEY_RIGHT:
            curr_part--;
            if (curr_part < 0) curr_part += poly.size();
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    // esc
    if (key == 27)
        exit(0);
    // space
    if (key == 32)
    {
        keyStates[(int)key] = true;
        if (isclear(last_color))
            last_color = poly_color[curr_part];
        poly_color[curr_part] = default_color;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    // space
    if (key == 32)
    {
        keyStates[(int)key] = false;
        poly_color[curr_part] = last_color;
        last_color = clear_color;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        buttonState = button;
        curr_mouse =
        {
            ((float)x / window_size[0] - 0.5f) * canvas_size[0],
            ((float)(window_size[1] - y) - 0.5f) / window_size[1] * canvas_size[1]
        };
        prev_mouse =
        {
            ((float)x / window_size[0] - 0.5f) * canvas_size[0],
            ((float)(window_size[1] - y) - 0.5f) / window_size[1] * canvas_size[1]
        };
    }
    else if (state == GLUT_UP)
        button = -1;
}

void motion(int x, int y)
{
    curr_mouse =
    {
        ((float)x / window_size[0] - 0.5f) * canvas_size[0],
        ((float)(window_size[1] - y) - 0.5f) / window_size[1] * canvas_size[1]
    };

    if (buttonState == GLUT_RIGHT_BUTTON || buttonState == GLUT_LEFT_BUTTON)
    {
        // rotate
        rotations[curr_part] += curr_mouse[0] - prev_mouse[0];
        glutPostRedisplay();
    }

    prev_mouse = curr_mouse;
}
