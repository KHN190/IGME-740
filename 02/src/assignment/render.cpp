
void displayCursor() {
    glColor3fv(config.cursor_color.data());
    glPointSize(10.0f * config.cursor_size);
    glBegin(GL_POINTS);
    glVertex2fv(mouse_pos.data());
    glEnd();
    glPointSize(1.0f);
}

void displayMouse() {
    glColor3fv(config.color.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n_vertices; i++) {
        glVertex2fv(curr_poly[i].data());
    }
    glVertex2fv(mouse_pos.data());
    glEnd();
}

void displayPolygon(Polygon polygon, Color color, float size) {
    glColor3fv(color.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int n_poly = polygon.size();
    switch (n_poly) {
        // tri
        case 3:
            glBegin(GL_TRIANGLES);
            break;
        // dot
        case 1:
            glPointSize(10.0f * size);
            glBegin(GL_POINTS);
            break;
        // line
        case 2:
            glLineWidth(10.0f * size);
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
    for (int i = 0; i < polygon.size(); i++) {
        glVertex2fv(polygon[i].data());
    }
    glEnd();
}

void display(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    displayMouse();

    for (int i = 0; i < poly.size(); i++) {
        displayPolygon(poly[i], poly_color[i], poly_size[i]);
    }

    displayCursor();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    raster_size[0] = w;
    raster_size[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvas_size[0], 0.0, canvas_size[1]);
    glViewport(0, 0, raster_size[0], raster_size[1]);

    glutPostRedisplay();
}

bool isDrawingFinished() {
    switch (config.type) {
        case brush_type::dot:
            return n_vertices == 1;
        case brush_type::line:
            return n_vertices == 2;
        case brush_type::tri:
            return n_vertices == 3;
        case brush_type::quad:
            return n_vertices == 4;
        default:
            return false;
    }
}

void endDrawing() {
    poly.push_back(curr_poly);
    poly_color.push_back(config.color);
    poly_size.push_back(config.cursor_size);

    curr_poly.clear();
    n_vertices = 0;
}
