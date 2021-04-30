#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

class Sphere{

public: // it would be better to have some kind of protection on members...

	vec3 pos;
  float radius;
  vec3 color;

	// material
	float ambient;
  float diffuse;
	float phong;

	// shader

public:
    Sphere(){};
    ~Sphere(){};

		/// lightPos, camera
    void Draw(const GLfloat* lightPos, const Camera &camera)
    {
			// lightingShader.use();
			// lightingShader.setVec3("objectColor", color.x, color.y, color.z);
			// lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			// lightingShader.setVec3("lightPos", lightPos);
			// lightingShader.setVec3("viewPos", camera.Position);
			//
			// // view/projection transformations
			// glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			// glm::mat4 view = camera.GetViewMatrix();
			// lightingShader.setMat4("projection", projection);
			// lightingShader.setMat4("view", view);
			//
			// // world transformation
			// glm::mat4 model = glm::mat4(1.0f);
			// lightingShader.setMat4("model", model);






        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
