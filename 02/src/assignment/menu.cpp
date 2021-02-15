
void menu(int value) {
    switch (value) {
        // clear
        case 0:
            poly.clear();
            poly_color.clear();

            curr_poly.clear();
            n_vertices = 0;

            glutPostRedisplay();
            break;

        // exit
        case 1:
            exit(0);

        // color seetting
        case 2:  // red
            config.color[0] = 1.0f;
            config.color[1] = 0.0f;
            config.color[2] = 0.0f;
            glutPostRedisplay();
            break;
        case 3:  // green
            config.color[0] = 0.0f;
            config.color[1] = 1.0f;
            config.color[2] = 0.0f;
            glutPostRedisplay();
            break;
        case 4:  // blue
            config.color[0] = 0.0f;
            config.color[1] = 0.0f;
            config.color[2] = 1.0f;
            glutPostRedisplay();
            break;

        // brush setting
        case 5:
            if (n_vertices != 0) { std::cout << "must finish drawing before changing brush!\n"; }
            else { config.type = brush_type::dot; }
            break;
        case 6:
            config.type = brush_type::line;
            if (n_vertices != 0) { std::cout << "must finish drawing before changing brush!\n"; }
            else { config.type = brush_type::line; }
            break;
        case 7:
            if (n_vertices != 0) { std::cout << "must finish drawing before changing brush!\n"; }
            else { config.type = brush_type::tri; }
            break;
        case 8:
            if (n_vertices != 0) { std::cout << "must finish drawing before changing brush!\n"; }
            else { config.type = brush_type::quad; }
            break;
        case 9:
            if (n_vertices != 0) { std::cout << "must finish drawing before changing brush!\n"; }
            else { config.type = brush_type::poly; }
            break;

        // cursor
        case 10: // small
            config.cursor_size = 1;
            break;
        case 11: // medium
            config.cursor_size = 2;
            break;
        case 12: // large
            config.cursor_size = 3;
            break;

        default:
            break;
    }
}

void createMenu() {
    int color_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Blue", 4);

    int brush_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Dot", 5);
    glutAddMenuEntry("Line", 6);
    glutAddMenuEntry("Tri", 7);
    glutAddMenuEntry("Quad", 8);
    glutAddMenuEntry("Poly", 9);

    int cursor_menu = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 10);
    glutAddMenuEntry("Medium", 11);
    glutAddMenuEntry("Large", 12);

    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddSubMenu("Colors", color_menu);
    glutAddSubMenu("Brush", brush_menu);
    glutAddSubMenu("Cursor", cursor_menu);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
