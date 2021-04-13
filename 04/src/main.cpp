// This OpenGL project demonstrates a simple interactive camera implementation
// and a few vertex and fragment shaders. Two modes of camera controls (press c
// to switch between two modes):
// 1. Focus Mode: Holding ALT and Left Mouse Button (LMB) and moving the mouse
// will rotate the camera about the LookAt Position
//                Holding ALT and MMB and moving the mouse will pan the camera.
//                Holding ALT and RMB and moving the mouse will zoom the camera.
// 2. First-Person Mode: Pressing W, A, S, or D will move the camera
//                       Holding LMB and moving the mouse will roate the camera.
// Basic shader - demonstrate the basic use of vertex and fragment shader
// Displacement shader - a special fireball visual effects with Perlin noise
// function Toon shading shader - catoonish rendering effects Per-vertex shading
// v.s. per-fragment shading = visual comparison between two types of shading

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Text.h"

#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

int g_winWidth = 1024;
int g_winHeight = 512;

Camera g_cam;
Text g_text;

unsigned char g_keyStates[256];

#ifdef __APPLE__
char v_basic[] = "./shaders/basic.vert";
char f_basic[] = "./shaders/basic.frag";
char v_shader_per_vertex[] = "./shaders/perVert_phong.vert";
char f_shader_per_vertex[] = "./shaders/perVert_phong.frag";
char v_shader_per_frag[] = "./shaders/perFrag_phong.vert";
char f_shader_per_frag[] = "./shaders/perFrag_phong.frag";
#endif

#ifdef __linux__
char v_basic[] = "./shaders/basic.vert";
char f_basic[] = "./shaders/basic.frag";
char v_shader_per_vertex[] = "./shaders/perVert_phong.vert";
char f_shader_per_vertex[] = "./shaders/perVert_phong.frag";
char v_shader_per_frag[] = "./shaders/perFrag_phong.vert";
char f_shader_per_frag[] = "./shaders/perFrag_phong.frag";
#endif

#ifdef _WIN32
char v_basic[] = ".\\shaders\\basic.vert";
char f_basic[] = ".\\shaders\\basic.frag";
char v_shader_per_vertex[] = ".\\shaders\\perVert_phong.vert";
char f_shader_per_vertex[] = ".\\shaders\\perVert_phong.frag";
char v_shader_per_frag[] = ".\\shaders\\perFrag_phong.vert";
char f_shader_per_frag[] = ".\\shaders\\perFrag_phong.frag";
#endif

const char meshFile[] = "Mesh/teapot.obj";
const char lightMesh[] = "Mesh/sphere.obj";

// obj mesh
Mesh g_mesh_0;
Mesh g_mesh_1;

// light mesh
Mesh l_mesh_0;
Mesh l_mesh_1;

vec3 g_lightPos0 = vec3(3, 3, 3);
vec3 g_lightPos1 = vec3(1, 0, -2);
float g_time = 0.0f;

// select light
bool l0_select = true;
bool l1_select = false;

void initialization() {
  g_cam.set(3.0f, 4.0f, 14.0f, 0.0f, 1.0f, -0.5f, g_winWidth, g_winHeight);
  g_text.setColor(0.0f, 0.0f, 0.0f);

  g_mesh_0.create(meshFile, v_shader_per_vertex, f_shader_per_vertex);
  g_mesh_1.create(meshFile, v_shader_per_frag, f_shader_per_frag);

  l_mesh_0.create(meshFile, v_basic, f_basic);
  l_mesh_1.create(meshFile, v_basic, f_basic);
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
  g_lightPos0.x = 3.0 * std::sin(g_time);
  g_lightPos0.z = 3.0 * std::cos(g_time);

  g_lightPos1.x = 1.0 * std::sin(g_time * 2.0);
  g_lightPos1.z = -2.0 * std::cos(g_time * 2.0);

  glutPostRedisplay();
}

void display() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // add any stuff you'd like to draw

  glUseProgram(0);
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  g_cam.drawGrid();
  g_cam.drawCoordinateOnScreen(g_winWidth, g_winHeight);
  g_cam.drawCoordinate();

  // display the text
  string str;
  if (g_cam.isFocusMode()) {
    str = "Cam mode: Focus";
    g_text.draw(10, 30, const_cast<char *>(str.c_str()), g_winWidth, g_winHeight);
  } else if (g_cam.isFPMode()) {
    str = "Cam mode: FP";
    g_text.draw(10, 30, const_cast<char *>(str.c_str()), g_winWidth, g_winHeight);
  }
  str = "vertex count: " + std::to_string(g_mesh_0.vert_num);
  g_text.draw(10, 45, const_cast<char *>(str.c_str()), g_winWidth, g_winHeight);
  str = "triangle count: " + std::to_string(g_mesh_0.tri_num);
  g_text.draw(10, 60, const_cast<char *>(str.c_str()), g_winWidth, g_winHeight);

  g_time = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
  g_mesh_0.draw(g_cam.viewMat, g_cam.projMat, vec3(0.0, 2.0, 0.0), g_lightPos0, g_lightPos1, g_cam.getEyeVec3(), g_time, false, 0.5f);
  g_mesh_1.draw(g_cam.viewMat, g_cam.projMat, vec3(3.0, 2.0, 0.0), g_lightPos0, g_lightPos1, g_cam.getEyeVec3(), g_time, false, 0.5f);

  l_mesh_0.draw(g_cam.viewMat, g_cam.projMat, g_lightPos0, g_lightPos0, g_lightPos1, g_cam.getEyeVec3(), g_time, l0_select, 0.1f);
  l_mesh_1.draw(g_cam.viewMat, g_cam.projMat, g_lightPos1, g_lightPos0, g_lightPos1, g_cam.getEyeVec3(), g_time, l1_select, 0.1f);

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
  case 's':
    l0_select = !l0_select;
    l1_select = !l1_select;
    break;
  case '+':
    g_mesh_0.normal_offset += 0.01;
    break;
  case '-':
    g_mesh_0.normal_offset -= 0.01;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
#ifdef __APPLE__
  glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
#else
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
#endif
  glutInitWindowSize(g_winWidth, g_winHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("VertFrag Shader Example");

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

  cout << "initialization finished\n";
  glutMainLoop();
  return EXIT_SUCCESS;
}
