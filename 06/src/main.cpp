#include <GL/glew.h>
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

#include "Sphere.h"
#include "Box.h"
#include "Light.h"
#include "Ray.h"

#include "Text.h"

#include <fstream>
#include <iostream>
using namespace std;
using namespace glm;

int g_winWidth  = 640;
int g_winHeight = 480;

// plane vertices
float vertices[4 * 2] = {
  0,   0,
  640, 0,
  640, 480,
  0,   480
};
// plane texture
float texCoords[4*2] = {
  0, 0,
  1, 0,
  1, 1,
  0, 1
};

Camera g_cam;
Text g_text;
unsigned char g_keyStates[256];

GLfloat light0_pos[] = {0.0f, 5.0f, 5.0f, 0.0f};
GLfloat light0_Amb[] = {0.4f, 0.3f, 0.3f, 1.0f};
GLfloat light0_Diff[] = {0.8f, 0.8f, 0.7f, 1.0f};
GLfloat light0_Spec[] = {0.9f, 0.9f, 0.9f, 1.0f};

const char dataFile[128] = "data/geo.txt";

unsigned int g_box_num;
Box* g_boxes;

unsigned int g_sphere_num;
Sphere* g_spheres;

GLuint glTexID = -1;
unsigned int* imagedata;

Light g_light;


char float_to_char(const float val) {
  return (char)round(val * 255);
}

// todo return hit info including color
bool scene_intersect(const vec3 &orig, const vec3 &dir, vec3 &hit, vec3 &N) {
    float distance = std::numeric_limits<float>::max();
    // hit spheres
    for (int i = 0; i < g_sphere_num; i++) {
        float dist_i;
        // a sphere is hit
        if (g_spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < distance) {
            distance = dist_i;
            hit = orig + dir*dist_i;
            N = normalize(hit - g_spheres[i].pos);
        }
    }
    // hit boxes
    for (int i = 0; i < g_sphere_num; i++) {
        float dist_i;
        // a box is hit
        if (g_boxes[i].ray_intersect(orig, dir, dist_i) && dist_i < distance) {
            distance = dist_i;
            hit = orig + dir*dist_i;
            N = normalize(hit - g_boxes[i].pos);
        }
    }
    return distance < 1000;
}

// todo
vec3 cast_ray(const vec3 &orig, const vec3 &dir, int depth=0) {
    vec3 point, N;

    if (depth>1000 || !scene_intersect(orig, dir, point, N)) {
      // background color
      return vec3(0.0, 0.0, 0.5);
    }

    // todo object color: light + shadow

    // float diffuse_light_intensity = 0, specular_light_intensity = 0;
    // for (int i=0; i<lights.size(); i++) {
    //     vec3 light_dir      = (lights[i].position - point).normalize();
    //     float light_distance = (lights[i].position - point).norm();
    //
    //     vec3 shadow_orig = light_dir*N < 0 ? point - N*1e-3 : point + N*1e-3; // checking if the point lies in the shadow of the lights[i]
    //     vec3 shadow_pt, shadow_N;
    //     Material tmpmaterial;
    //     if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt-shadow_orig).norm() < light_distance)
    //         continue;
    //
    //     diffuse_light_intensity  += lights[i].intensity * std::max(0.f, light_dir*N);
    //     specular_light_intensity += 0.f;
    // }
    // return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + vec3(1., 1., 1.)*specular_light_intensity * material.albedo[1];

    // object color
    return vec3(0.5, 0.5, 0.5);
}

// cast ray from camera eye position
// if hits an object, render the color (todo with light + base color)
// hit closest object, return normal, color in hit info
//
// todo update imagedata
void trace() {
  vec3 buffer[g_winHeight * g_winWidth];

  // cast all rays on each pixel
  for (int j = 0; j < g_winHeight; j++) {
        for (int i = 0; i < g_winWidth; i++) {
            float dir_x =  (i + 0.5) -  g_winWidth/2.;
            float dir_y = -(j + 0.5) + g_winHeight/2.;
            float dir_z = -g_winHeight/(2.*tan(g_cam.fovy/2.));

            vec3 orig = g_cam.getEyeVec3();
            vec3 dir = normalize(vec3(dir_x, dir_y, dir_z));

            // todo store hit info
            vec3 pixel = cast_ray(orig, dir, 0);
            buffer[i + j * g_winWidth] = pixel;

            // todo index is wrong
            imagedata[i * j * 3 + 0] = float_to_char(pixel[0]);
            imagedata[i * j * 3 + 1] = float_to_char(pixel[1]);
            imagedata[i * j * 3 + 2] = float_to_char(pixel[2]);
        }
    }
    // update texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_winWidth, g_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
}

void createTexture()
{
	imagedata = new unsigned int[g_winWidth * g_winHeight * 3];

	// assign red color (255, 0 , 0) to each pixel
	for (int i = 0; i < g_winWidth * g_winHeight; i++)
	{
		imagedata[i * 3 + 0] = 125; // R
		imagedata[i * 3 + 1] = 0;   // G
		imagedata[i * 3 + 2] = 0;   // B
	}

  glGenTextures(1, &glTexID);
  glBindTexture(GL_TEXTURE_2D, glTexID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // send the imagedata (on CPU) to the GPU memory at glTexID (glTexID is a GPU memory location index)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_winWidth, g_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
}

void drawPlane()
{
  glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, glTexID); // use the texture on the quad
	glBegin(GL_QUADS);
	    glTexCoord2fv(texCoords + 2*0); glVertex2fv(vertices + 2*0);
      glTexCoord2fv(texCoords + 2*1); glVertex2fv(vertices + 2*1);
      glTexCoord2fv(texCoords + 2*2); glVertex2fv(vertices + 2*2);
      glTexCoord2fv(texCoords + 2*3); glVertex2fv(vertices + 2*3);
	glEnd();
  glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void drawObjects() {
  glEnable(GL_LIGHTING);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos); // commenting out this line to make object always lit up in front of the cam.

  // draw sphere and box
  for (int i=0; i<g_sphere_num; i++)
  {
      g_spheres[i].Draw();
      // g_spheres_mesh[i].draw(g_cam.mvMat, g_cam.projMat, g_light.pos, g_cam.getEyeVec3(), false);
  }
  for (int i=0; i<g_box_num; i++)
  {
      g_boxes[i].Draw();
      // g_boxes_mesh[i].draw(g_cam.mvMat, g_cam.projMat, g_light.pos, g_cam.getEyeVec3(), false);
  }

  // displaying the camera
  g_cam.drawGrid();
  g_cam.drawCoordinateOnScreen(g_winWidth, g_winHeight);
  g_cam.drawCoordinate();

  // displaying the text
  if(g_cam.isFocusMode()) {
      string str = "Cam mode: Focus";
      g_text.draw(10, 30, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
  } else if(g_cam.isFPMode()) {
      string str = "Cam mode: FP";
      g_text.draw(10, 30, const_cast<char*>(str.c_str()), g_winWidth, g_winHeight);
  }

  char s[128];
  g_text.draw(10, 50, s, g_winWidth, g_winHeight);
}

void LoadConfigFile(const char* pFilePath)
{
    const unsigned int CAMERA= 0;
    const unsigned int LIGHT= 1;
    const unsigned int SPHERE= 2;
    const unsigned int BOX = 3;

    vec3 rot;
    fstream filestr;
    filestr.open (pFilePath, fstream::in);
    if(!filestr.is_open()){
        cout<<"'"<<pFilePath<<"'"<<", geo file does not exsit!"<<endl;
        filestr.close();
        return;
    }

    char objType[80];
    char attrType[80];
    unsigned int objState = -1;

    bool loop = true;
    while(loop){
        filestr>>objType;
        if(!strcmp(objType, "Camera:")) objState=CAMERA;
        else if(!strcmp(objType, "Light:")) objState = LIGHT;
        else if(!strcmp(objType, "Sphere:")) objState = SPHERE;
        else if(!strcmp(objType, "Box:")) objState = BOX;
        switch(objState){
            case CAMERA:
                float e_x, e_y, e_z;
                float l_x, l_y, l_z;
                float near_plane;

                for(int i=0; i<3; i++){
                    filestr>>attrType;
                    if(!strcmp(attrType, "eye:")){
                        filestr>>e_x;
                        filestr>>e_y;
                        filestr>>e_z;
                    }
                    else if(!strcmp(attrType, "lookat:")){
                        filestr>>l_x;
                        filestr>>l_y;
                        filestr>>l_z;
                    }
                    else if(!strcmp(attrType, "near_plane:")) filestr>>near_plane;
                }
                g_cam.set(e_x, e_y, e_z, l_x, l_y, l_z, g_winWidth, g_winHeight, 45.0f, near_plane, 1000.0f);
                break;
            case LIGHT:
                filestr>>attrType;
                if(!strcmp(attrType, "position:")){
                    filestr>>g_light.pos.x;
                    filestr>>g_light.pos.y;
                    filestr>>g_light.pos.z;
                }
                filestr>>attrType;
                if(!strcmp(attrType, "color:")){
                    filestr>>g_light.color.x;
                    filestr>>g_light.color.y;
                    filestr>>g_light.color.z;
                }
                filestr>>attrType;
                if(!strcmp(attrType, "intensity:")){
                    filestr>>g_light.intensity;
                }
                break;
            case SPHERE:
                filestr>>attrType;
                if(!strcmp(attrType, "num:"))
                    filestr>>g_sphere_num;
                if(g_sphere_num > 0) {
                    g_spheres = new Sphere[g_sphere_num];
                    // g_spheres_mesh = new Mesh[g_sphere_num];

                    for(int i=0; i<g_sphere_num; i++){
                      Sphere mesh = Sphere();
                      // Mesh sphere;

                      filestr>>attrType;
                      if(!strcmp(attrType, "position:")){
                          filestr>>mesh.pos.x; filestr>>mesh.pos.y;  filestr>>mesh.pos.z;
                      }
                      filestr>>attrType;
                      if(!strcmp(attrType, "radius:")){
                          filestr>>mesh.radius;
                      }
                      filestr>>attrType;
                      if(!strcmp(attrType, "color:")){
                          filestr>>mesh.color.x; filestr>>mesh.color.y;  filestr>>mesh.color.z;
                      }
                      filestr>>attrType;
                      if(!strcmp(attrType, "ambient:"))    filestr>>mesh.ambient;
                      filestr>>attrType;
                      if(!strcmp(attrType, "diffuse:"))    filestr>>mesh.diffuse;
                      filestr>>attrType;
                      if(!strcmp(attrType, "phong:"))     filestr>>mesh.phong;

                      // sphere to mesh
                      // sphere.create_sphere(mesh, "shaders/basic.vert", "shaders/basic.frag");

                      g_spheres[i] = mesh;
                      // g_spheres_mesh[i] = sphere;
                  }
                }
                break;
            case BOX:
                filestr>>attrType;
                if(!strcmp(attrType, "num:"))
                    filestr>>g_box_num;

                if(g_box_num > 0) {
                    g_boxes = new Box[g_box_num];
                    // g_boxes_mesh = new Mesh[g_box_num];

                    for(int i=0; i<g_box_num; i++){
                        Box mesh = Box();
                        // Mesh box_mesh;

                        filestr>>attrType;
                        if(!strcmp(attrType, "conner_position:")){
                            filestr>>mesh.minPos.x;
                            filestr>>mesh.minPos.y;
                            filestr>>mesh.minPos.z;
                            filestr>>mesh.maxPos.x;
                            filestr>>mesh.maxPos.y;
                            filestr>>mesh.maxPos.z;
                        }

                        filestr>>attrType;
                        if(!strcmp(attrType, "color:")){
                            filestr>>mesh.color.x;
                            filestr>>mesh.color.y;
                            filestr>>mesh.color.z;
                        }
                        filestr>>attrType;
                        if(!strcmp(attrType, "rotate:")){
                            filestr>>rot.x;
                            filestr>>rot.y;
                            filestr>>rot.z;
                            //Convert to radians
                            rot.x *= 3.14159265f / 180.0f;
                            rot.y *= 3.14159265f / 180.0f;
                            rot.z *= 3.14159265f / 180.0f;

                            mat4 m (1.0f);

                            // rotation order is zyx
                            m = rotate(m, rot.z, vec3(0, 0, 1));
                            m = rotate(m, rot.y, vec3(0, 1, 0));
                            m = rotate(m, rot.x, vec3(1, 0, 0));

                            //cout<<glm::to_string(m)<<endl;

                            mesh.rotMat = m;
                            mesh.invRotMat = inverse(m);
                        }
                        filestr>>attrType;
                        if(!strcmp(attrType, "ambient:")) filestr>>mesh.ambient;
                        filestr>>attrType;
                        if(!strcmp(attrType, "diffuse:")) filestr>>mesh.diffuse;
                        filestr>>attrType;
                        if(!strcmp(attrType, "phong:")) filestr>>mesh.phong;

                        // error: it modifies sphere meshes??
                        // error: it modifies existing boxes properties too!!
                        // box_mesh.create_box(mesh, "shaders/basic.vert", "shaders/basic.frag");

                        g_boxes[i] = mesh;
                        // g_boxes_mesh[i] = box_mesh;
                    }
                    loop = false;
                }
                break;
        }
    }
}

void initialization()
{
    //g_cam.set(3.0f, 4.0f, 3.0f, 0.0f, 0.0f, 0.0f, g_winWidth, g_winHeight);

    LoadConfigFile(dataFile);

    g_text.setColor(0.0f, 0.0f, 0.0f);
}

/****** GL callbacks ******/
void initialGL()
{
  createTexture();

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_Amb);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_Diff);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_Spec);


    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glPolygonMode(GL_FRONT, GL_FILL);


    glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

void idle()
{
    // adding stuff to update at runtime ....

    // todo update imagedata
    trace();

    g_cam.keyOperation(g_keyStates, g_winWidth, g_winHeight);
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // drawObjects();
    drawPlane();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    g_winWidth = w;
    g_winHeight = h;
    if (h == 0) {
        h = 1;
    }
    g_cam.setProj(g_winWidth, g_winHeight);
    g_cam.setModelView();
    glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y)
{
    g_cam.mouseClick(button, state, x, y, g_winWidth, g_winHeight);

}

void motion(int x, int y)
{
    g_cam.mouseMotion(x, y, g_winWidth, g_winHeight);
}

void keyup(unsigned char key, int x, int y)
{
    g_keyStates[key] = false;
}

void keyboard(unsigned char key, int x, int y)
{
    g_keyStates[key] = true;
    switch(key) {
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

    }
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(g_winWidth, g_winHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Ray Casting");

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
