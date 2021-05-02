#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "Shader.h"

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

	// shader
	Shader shader = Shader("../shaders/basic.vert", "../shaders/basic.frag");

public:
    Sphere() {};
    ~Sphere(){};

		/// lightPos, camera
    void Draw(const GLfloat* lightPosf, const Camera &camera)
    {
			// vec3 lightPos = vec3(lightPosf[0], lightPosf[1], lightPosf[2]);

			shader.use();
			shader.setVec3("objectColor", color.x, color.y, color.z);
			shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			shader.setVec3("lightPos", lightPos);
			shader.setVec3("viewPos", camera.eye);

			// view/projection transformations
			glm::mat4 projection = camera.projMat;
			glm::mat4 view = camera.mvMat;
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);

			// world transformation
			glm::mat4 model = glm::mat4(1.0f);
			shader.setMat4("model", model);






        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glColor3f(color.x,color.y, color.z);
        // glutWireSphere(radius, 10, 10);
				glutSolidSphere(radius, 10, 10);

        glPopMatrix();
    };
};

#endif
