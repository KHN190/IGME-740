#ifndef MAIN_H
#define MAIN_H

////////////////////////////////////////////////
#ifdef __APPLE__

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <array>
#include <vector>
#include <string>
#include <iostream>

////////////////////////////////////////////////

typedef std::array<float, 2> Point;
typedef std::array<float, 3> Color;
typedef std::vector<Point> Polygon;

Point operator+(const Point& a, const Point& b) {
    return { a[0] + b[0], a[1] + b[1] };
}

Point operator-(const Point& a, const Point& b) {
    return { a[0] - b[0], a[1] - b[1] };
}

////////////////////////////////////////////////

const float canvas_size[] = {30.0f, 30.0f};
int raster_size[] = {2048, 1152};

// storing 2D vertices
int n_vertices = 0;

// defaults
const Color default_color = {1.0f, 0.0f, 0.0f};
const Color default_cursor_color = {1.0f, 0.0f, 1.0f};

////////////////////////////////////////////////

Point mouse_pos = {0.0f, 0.0f};

////////////////////////////////////////////////

std::vector<Polygon> poly;
std::vector<Color> poly_color;
std::vector<float> poly_size;
Polygon curr_poly;

////////////////////////////////////////////////

enum class brush_type {
    dot, line, tri, quad, poly
};

struct BrushConfig {
    // brush
    brush_type type = brush_type::tri;
    // polygon
    Color color = default_color;
    float line_width = 1;
    // cursor
    Color cursor_color = default_cursor_color;
    float cursor_size = 1;
};

BrushConfig config;

////////////////////////////////////////////////

#endif