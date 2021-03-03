void drawPolygon(Polygon polygon, Color color)
{
    glColor3fv(color.data());

    int n_poly = polygon.size();
    switch (n_poly)
    {
    // tri
    case 3:
        glBegin(GL_TRIANGLES);
        break;
    // dot
    case 1:
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        break;
    // line
    case 2:
        glLineWidth(10.0f);
        glBegin(GL_LINES);
        break;
    // quad
    case 4:
        glBegin(GL_QUADS);
        break;
    // polygon
    default:
        glBegin(GL_POLYGON);
        break;
    }
    for (int i = 0; i < polygon.size(); i++)
    {
        glVertex2fv(polygon[i].data());
    }
    glEnd();
}

void display(void)
{
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int i = 0; i < poly.size(); i++)
    {
        if (isStart(parts[i])) glPushMatrix();
        glTranslatef(translations[i][0], translations[i][1], 0.0f);
        glRotatef(rotations[i], 0.0f, 0.0f, 1.0f);
        drawPolygon(poly[i], poly_color[i]);
        if (isEnd(parts[i])) glPopMatrix();
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    window_size[0] =  w;
    window_size[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvas_size[0] / 2.0f, canvas_size[0] / 2.0f, -canvas_size[1] / 2.0f, canvas_size[1] / 2.0f);
    glViewport(0, 0, (GLsizei) window_size[0], (GLsizei) window_size[1]);

    glutPostRedisplay();
}
