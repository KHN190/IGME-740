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
 	unsigned int id;

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

    // ray & sphere intersection
    bool ray_intersect(vec3 orig, vec3 dir, float dist_i) {
      vec3 oc = orig - pos;
      float a = dot(dir, dir);
      float b = 2.0 * dot(oc, dir);
      float c = dot(oc,oc) - radius*radius;
      float discriminant = b*b - 4*a*c;
      // also return distance of hit
      if (discriminant > 0) {
        dist_i = (-b - sqrt(discriminant)) / (2.0 * a);
        return true;
      }
      return false;
    }
};

#endif
