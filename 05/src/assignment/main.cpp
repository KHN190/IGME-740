#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"
#include "ParticleSystem.h"
#include "Text.h"

#include <iostream>
using namespace std;
using namespace glm;

int max_width = 8192;
int min_width = 16;

int res_width = 1024 * 2;
int res_height = 512 * 2;

int g_winWidth = 1024;
int g_winHeight = 512;

Camera g_cam;
Text g_text;

unsigned char g_keyStates[256];

unsigned int curTime = 0; // the milliseconds since the start
unsigned int preTime = 0;
ParticleSystem parSys;

char v_shader_file[] = "./shaders/v_shader.vert";
char f_shader_file[] = "./shaders/f_shader.frag";
char c_shader_file[] = "./shaders/c_shader.comp";

vec3 origin = vec3(0.0, 5.0, 20.0);
vec3 wire_pos = vec3(0.0, 5.0, -8.0);
float wire_radius = 2.0;

// requirement 5: adjust resolution
void redraw() {
	std::cout << "redraw particle system: " << res_width << ", " << res_height << '\n';
	parSys.recreate(res_width + 1, res_height + 1, vec3(-10.0f, 0.0f, -5.0f), vec3(10.0f, 10.0f, -5.0f),
                c_shader_file, v_shader_file, f_shader_file);
}

void initialization() {
	// requirement 1: particle system creation
  parSys.create(res_width + 1, res_height + 1, vec3(-10.0f, 0.0f, -5.0f), vec3(10.0f, 10.0f, -5.0f),
                c_shader_file, v_shader_file, f_shader_file);

  g_cam.set(38.0f, 13.0f, 4.0f, 0.0f, 0.0f, 0.0f, g_winWidth, g_winHeight,
            45.0f, 0.01f, 10000.0f);
  g_text.setColor(0.0f, 0.0f, 0.0f);
}

/****** GL callbacks ******/
void initialGL() {
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void idle() {
  // add any stuff to update at runtime ....
  curTime = glutGet(GLUT_ELAPSED_TIME);
  float deltaT = (float)(curTime - preTime) / 1000.0f; // in seconds
  parSys.update(deltaT, origin, wire_pos, wire_radius);

  g_cam.keyOperation(g_keyStates, g_winWidth, g_winHeight);

  glutPostRedisplay();

  preTime = curTime;
}

void display() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(0);
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
	float particle_size = 512.0f / g_winWidth;
  parSys.draw(particle_size, g_cam.viewMat, g_cam.projMat);

  g_cam.drawGrid();
  g_cam.drawCoordinateOnScreen(g_winWidth, g_winHeight);
  g_cam.drawCoordinate();

	// requirement 2: display ray origin
	glPushMatrix();
  glTranslated(origin[0], origin[1], origin[2]); // position of the sphere
	glColor3d(1,0,0); // red
  glutSolidSphere(0.3,50,50); // radius 0.3
  glPopMatrix();

	// requirement 2: draw lines to corners
	glBegin(GL_LINES);
	  glVertex3f(origin[0], origin[1], origin[2]);
	  glVertex3f(-10.0f, 0.0f, -5.0f);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(origin[0], origin[1], origin[2]);
		glVertex3f(-10.0f, 10.0f, -5.0f);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(origin[0], origin[1], origin[2]);
		glVertex3f(10.0f, 10.0f, -5.0f);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(origin[0], origin[1], origin[2]);
		glVertex3f(10.0f, 0.0f, -5.0f);
	glEnd();

	// requirement 3: display wireframe sphere
	glPushMatrix();
	glTranslated(wire_pos[0], wire_pos[1], wire_pos[2]); // position of the sphere
	glColor3d(0,1,0); // red
	glutWireSphere(wire_radius,50,50); // radius 2.0
	glPopMatrix();

  // display the text
  if (g_cam.isFocusMode()) {
    string str = "Cam mode: Focus";
    g_text.draw(10, 30, const_cast<char *>(str.c_str()), g_winWidth,
                g_winHeight);
  } else if (g_cam.isFPMode()) {
    string str = "Cam mode: FP";
    g_text.draw(10, 30, const_cast<char *>(str.c_str()), g_winWidth,
                g_winHeight);
  }

	{
		string str = "resolution: " + to_string(res_width) + ", " + to_string(res_height);
		g_text.draw(10, 60, const_cast<char *>(str.c_str()), g_winWidth,
								g_winHeight);
	}

  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int w, int h) {
  g_winWidth = w;
  g_winHeight = h;
  if (h == 0) {
    h = 1;
  }
  g_cam.setProjectionMatrix(g_winWidth, g_winHeight);
  g_cam.setViewMatrix();
  glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y) {
  g_cam.mouseClick(button, state, x, y, g_winWidth, g_winHeight);
}

void motion(int x, int y) { g_cam.mouseMotion(x, y, g_winWidth, g_winHeight); }

void keyup(unsigned char key, int x, int y) { g_keyStates[key] = false; }

void keyboard(unsigned char key, int x, int y) {
  g_keyStates[key] = true;
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'c': // switch cam control mode
    g_cam.switchCamMode();
    glutPostRedisplay();
    break;
  case ' ':
    g_cam.PrintProperty();
    break;
	// requirement 3: move wireframe sphere
	case 'w':
		wire_pos[2] += 0.1;
		break;
	case 's':
		wire_pos[2] -= 0.1;
		break;
	case 'a':
		wire_pos[1] += 0.1;
		break;
	case 'd':
		wire_pos[1] -= 0.1;
		break;
	case 'u':
		wire_pos[0] += 0.1;
		break;
	case 'j':
		wire_pos[0] -= 0.1;
		break;
	// adjust resolution
	// case '+':
	// 	if (res_width < max_width) { res_width *= 2; res_height *= 2; redraw(); }
	// 	break;
	// case '-':
	// 	if (res_width > min_width) { res_width /= 2; res_height /= 2; redraw(); }
	// 	break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(g_winWidth, g_winHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Compute Shader Example: A Simple particle System");

  glewInit();
  initialGL();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardUpFunc(keyup);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);

  initialization();

  glutMainLoop();
  return EXIT_SUCCESS;
}
