#ifndef SPHERE_H
#define SPHERE_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "Camera.h"

using namespace glm;

class Sphere{

public:

	vec3 pos;
  float radius;
  vec3 color;

	// material
	float ambient;
  float diffuse;
	float phong;

public:
    Sphere() {};
    ~Sphere(){};

    void Draw()
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
      glDisable(GL_LIGHTING);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(pos.x, pos.y, pos.z);
      glColor3f(color.x,color.y, color.z);
      glutWireSphere(radius, 10, 10);
			// glutSolidSphere(radius, 10, 10);

      glPopMatrix();
    };
};

#endif
