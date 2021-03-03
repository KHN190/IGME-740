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

Point operator+(const Point& a, const Point& b)
{
    return { a[0] + b[0], a[1] + b[1] };
}

Point operator-(const Point& a, const Point& b)
{
    return { a[0] - b[0], a[1] - b[1] };
}

Point operator*(const Point& a, const float b)
{
    return { a[0] * b, a[1] * b };
}

////////////////////////////////////////////////

enum BodyPart
{
    Start, Inside, End, Single
};

////////////////////////////////////////////////

bool isclear(Color color)
{
    return color[0] == 0 && color[1] == 0 && color[2] == 0;
}

const Color default_color = {1.0f, 0.0f, 0.0f};
const Color clear_color = {0.0f, 0.0f, 0.0f};

Color last_color;

////////////////////////////////////////////////

const float canvas_size[] = {30.0f, 30.0f};
int window_size[] = {600, 600};
int raster_size[] = {2048, 1152};

// storing 2D vertices
int n_vertices = 0;

////////////////////////////////////////////////

Point curr_mouse = {0.0f, 0.0f};
Point prev_mouse = {0.0f, 0.0f};

////////////////////////////////////////////////

std::vector<Polygon> poly;
std::vector<Color> poly_color;
Polygon curr_poly;

////////////////////////////////////////////////

#define MAX_NUM_PARTS 7
#define CIRCLE_RADIUM 2.0

bool keyStates[256];
int buttonState;

std::vector<Point> translations;
std::vector<float> rotations;
std::vector<BodyPart> parts;

int curr_part = 0;

const unsigned char asciiOffset = 49;

#endif