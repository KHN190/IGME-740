
void mouse(int button, int state, int x, int y) {
    // finish drawing
    if (isDrawingFinished()) { endDrawing(); }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // update mouse position
        mouse_pos[0] = static_cast<float>(x) / raster_size[0] * canvas_size[0];
        mouse_pos[1] = static_cast<float>(raster_size[1] - y) / raster_size[1] * canvas_size[1];
        // update vertices
        curr_poly.push_back(mouse_pos);

        n_vertices++;
        glutPostRedisplay();
    }
}

void motion(int x, int y) {
    // Mouse events are handled by OS, eventually.
    // When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mouse_pos[0] = static_cast<float>(x) / raster_size[0] * canvas_size[0];
    mouse_pos[1] = static_cast<float>(raster_size[1] - y) / raster_size[1] * canvas_size[1];

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Esc
        case 27:
            exit(0);
            break;
        // Enter or Space
        case '\r':
        case 32:
            if (config.type == brush_type::poly) { endDrawing(); }
            break;
    }
}
