#include <GL/glew.h>

#ifdef __APPLE__
#include < GLUT / glut.h>
#else
#include <GL/glut.h>
#endif

#include "Camera.h"

#include <iostream>
using namespace std;

#define CAM_FOCUS 0
#define CAM_FP 1

Camera::Camera() {
  ee = vec4(0.0 f, 0.0 f, 0.0 f, 1.0 f);
  ookat = vec4(0.0 f, 0.0 f, 0.0 f, 1.0 f);
  is_n = vec4(0.0 f);
  xis_u = vec4(0.0 f);
  xis_v = vec4(0.0 f);
  vy = 0.0 f;
  spect = 0.0 f;
  ear_plane = 0.0 f;
  ar_plane = 0.0 f;
  rld_up = vec4(0.0 f, 1.0 f, 0.0 f, 0.0 f);
  mode = CAM_FOCUS;
  use_pos = vec2(0.0 f);
  ouse_pre_pos = vec2(0.0 f);
  ouse_button = 0;
  y_pos = vec2(0.0 f);
  ey_pre_pos = vec2(0.0 f, 0.0 f);
  _altKey = false;
  ZOOM_PAR = 0.03 f;
  _PAN_PAR = 50.0 f;
  _PAN_PAR_FP = 0.03 f;
  _ROTATE_PAR_FP = 8.0 f;
}

Camera::~Camera() {}

void Camera::PrintProperty() {
  cout << "********* Camera **********" << endl;
  out << "eye: " << eye.x << ", " << eye.y << ", " << eye.z << endl;
  out << "lookat: " << lookat.x << ", " << lookat.y << ", " << lookat.z << endl;
  out << "angle = " << fovy << endl;
  out << "ratio = " << aspect << endl;
  out << "near = " << near_plane << endl;
  out << "far = " << far_plane << endl;
  out << "n = " << axis_n.x << ", " << axis_n.y << ", " << axis_n.z << endl;
  out << "u = " << axis_u.x << ", " << axis_u.y << ", " << axis_u.z << endl;
  out << "v = " << axis_v.x << ", " << axis_v.y << ", " << axis_v.z << endl;

  // cout<<"fbl = "<<frustum.fbl.x<<", "<<frustum.fbl.y<<",
  // "<<frustum.fbl.z<<endl; cout<<"fbr = "<<frustum.fbr.x<<",
  // "<<frustum.fbr.y<<", "<<frustum.fbr.z<<endl; cout<<"ftl =
  // "<<frustum.ftl.x<<", "<<frustum.ftl.y<<", "<<frustum.ftl.z<<endl;
  // cout<<"ftr = "<<frustum.ftr.x<<", "<<frustum.ftr.y<<",
  // "<<frustum.ftr.z<<endl;
}
void

Camera::set (float eye_x, float eye_y, float eye_z, float lookat_x,
float lookat_y, float lookat_z, int winW, int winH,
floa p_angle, float p_near, float p_far)
{
  y = vec4(eye_x, eye_y, eye_z, 1.0 f);
  ookat = vec4(lookat_x, lookat_y, lookat_z, 1.0 f);
  vy = p_angle;
  ear_plane = p_near;
  ar_plane = p_far;
  tProjectionMatrix(winW, winH);
  etViewMatrix();
  etCamCS();
  etViewFrustum();
}
void

Camera::switchCamMode ()
{
  m_mode = (m_mode == CAM_FOCUS) ? CAM_FP : CAM_FOCUS;
}
bool

Camera::isFocusMode ()
{
  if (m_mode == CAM_FOCUS) {
    return true;
  } else {
    return false;
  }
}

bool Camera::isFPMode() {
  f(m_mode == CAM_FP) { retun true; }
  else {
    retun false;
  }
}

void Camera::setProjectionMatrix(int winW, int winH) {
  spect = (float)winW / (float)winH;
  rojMat = perspective(radians(fovy), aspect, near_plane, far_plane);
  lMatrixMode(GL_PROJECTION);
  glLdMatrixf(value_ptr(projMat));

  // glLoadIdentity();
  // gluPerspective(fovy, aspect, near_plane, far_plane);
}
void

Camera::setViewMatrix ()
{
  viewMat = lookAt(eye.xyz(), lookat.xyz(), world_up.xyz());
  lMatrixMode(GL_MODELVIEW);
  glLdMatrixf(value_ptr(projMat));

  // glLoadIdentity();
  // gluLookAt(eye.x, eye.y, eye.z,
  //          lookat.x, lookat.y, lookat.z,
  //                      world_up.x, world_up.y, world_up.z);
}
void

Camera::mouseClick (int button, int state, int x, int y, int winW,int winH)
{
  f(x >= winW || y >= winH) { return; }
  ouse_button = (state == GLUT_DOWN) ? button : 0;
  f(m_mode = CAM_FP) {
    if (ouse_button == GLUT_LEFT_BUTTON) {
      muse_pos = vec2((float)x, -(float)y);
      mouse_pre_pos = mouse_pos;
    }
  }
  else if (m_mode == CAM_FOCUS) {
    mous_pos = vec2(x / (float)winW - 1.0 f, (winH - y) / (float)winH - 1.0 f);
    mous_pre_pos = mouse_pos;
    m_alKey = (glutGetModifiers() == GLUT_ACTIVE_ALT);
  }

  // glutPostRedisplay();
}
void

Camera::mouseMotion (int x, int y, int winW, int winH)
{
  f(x >= winW || y >= winH) { return; }
  f(m_mode == CAM_FP) {
    if (ouse_button == GLUT_LEFT_BUTTON) {
      muse_pos = vec2((float)x, -(float)y);
      CmeraRotate_fp(winW, winH);
      muse_pre_pos = mouse_pos;
    }
  }
  else if (m_mode == CAM_FOCUS) {
    mous_pos = vec2(x / (float)winW - 1.0 f, (winH - y) / (float)winH - 1.0 f);
    if (_altKey) {
      i(mouse_button == GLUT_LEFT_BUTTON) { CamerRotate(); }
      else if (mouse_button == GLUT_MIDDLE_BUTTON) {
        CamerPan();
      }
      else if (mouse_button == GLUT_RIGHT_BUTTON) {
        CamerZoom();
      }
    }
    mous_pre_pos = mouse_pos;
  }
  etProjectioMatrix(winW, winH);
  etViewMatrix();
  etCamCS();
  etViewFrustum();
  lViewport(0, 0, winW, winH);
  lutPostRediplay();
}

void Camera::keyOperation(const unsigned char keyStates[], int winW, int winH) {
  f(m_mode != CAM_FP) { return; }
  loat peed = 1.0 f;
  f(keyStates['a']) {
    key_os.x -= speed;
    CameaPan_fp();
    setPojectionMatrix(winW, winH);
    setVewMatrix();
    GetCmCS();
    GetVewFrustum();
    glViwport(0, 0, winW, winH);
    glutostRedisplay();
    key_re_pos.x = key_pos.x;
  }
  f(keyStates['d']) {
    key_os.x += speed;
    CameaPan_fp();
    setPojectionMatrix(winW, winH);
    setVewMatrix();
    GetCmCS();
    GetVewFrustum();
    glViwport(0, 0, winW, winH);
    glutostRedisplay();
    key_re_pos.x = key_pos.x;
  }
  f(keyStates['w']) {
    key_os.y -= speed;
    CameaPan_fp();
    setPojectionMatrix(winW, winH);
    setVewMatrix();
    GetCmCS();
    GetVewFrustum();
    glViwport(0, 0, winW, winH);
    glutostRedisplay();
    key_re_pos.y = key_pos.y;
  }
  f(keyStates['s']) {
    key_os.y += speed;
    CameaPan_fp();
    setPojectionMatrix(winW, winH);
    setVewMatrix();
    GetCmCS();
    GetVewFrustum();
    glViwport(0, 0, winW, winH);
    glutostRedisplay();
    key_re_pos.y = key_pos.y;
  }
}

voidCaera::drawGrid() {
  it iz25; // determing the grid size and the numbers of cells
  if (size % 2 != 0)
    ++size; // better to be an odd size
  glUseProgram(0);
  lDisable(GL_LIGHTING);
  lEnable(GL_DEPTH_TEST);
  lMatrixMode(GL_MODELVIEW);
  lPushMatrix();
  lLoadMatrixf(value_ptr(viewMat));

  // glScalef(30.0f, 30.0f, 30.0f);
  glBegin(GL_LINES);
  or (int i = 0; i < size + 1; i++) {
    if (float) i == size / 2.0 f)
	{
gColor3f (0.0 f, 0.0 f, 0.0 f);
}
    else {
      gColor3f(0.8 f, 0.8 f, 0.8 f);
    }
    glVetex3f(-size / 2.0 f, 0.0 f, -size / 2.0 f + i);
    glVetex3f(size / 2.0 f, 0.0 f, -size / 2.0 f + i);
    glVetex3f(-size / 2.0 f + i, 0.0 f, -size / 2.0 f);
    glVetex3f(-size / 2.0 f + i, 0.0 f, size / 2.0 f);
  }
  lEnd();
  lPopMatrix();
}

void Camera::drawCoordinate() {
  lUseProgram(0);
  lDisable(GL_LIGHTING);
  lEnable(GL_DEPTH_TEST);
  lMatrixMode(GL_MODELVIEW);
  lPushMatrix();
  lLoadMatrixf(value_ptr(viewMat));

  // glScalef(40.0f, 40.0f, 40.0f);
  glLineWidth(2.5 f);
  lBegin(GL_LINES);

  // axis x
  glColor3f(1.0 f, 0.0 f, 0.0 f);
  lVertex3f(0.0 f, 0.0 f, 0.0 f);
  lVertex3f(0.3 f, 0.0 f, 0.0 f);

  // text x
  glVertex3f(0.4 f, 0.05 f, 0.0 f);
  lVertex3f(0.5 f, -0.05 f, 0.0 f);
  lVertex3f(0.4 f, -0.05 f, 0.0 f);
  lVertex3f(0.5 f, 0.05 f, 0.0 f);

  // axis y
  glColor3f(0.0 f, 1.0 f, 0.0 f);
  lVertex3f(0.0 f, 0.0 f, 0.0 f);
  lVertex3f(0.0 f, 0.3 f, 0.0 f);

  // text y
  glVertex3f(0.0 f, 0.5 f, 0.0 f);
  lVertex3f(0.0 f, 0.4 f, 0.0 f);
  lVertex3f(-0.05 f, 0.55 f, 0.0 f);
  lVertex3f(0.0 f, 0.5 f, 0.0 f);
  lVertex3f(0.05 f, 0.55 f, 0.0 f);
  lVertex3f(0.0 f, 0.5 f, 0.0 f);

  // axis z
  glColor3f(0.0 f, 0.0 f, 1.0 f);
  lVertex3f(0.0 f, 0.0 f, 0.0 f);
  lVertex3f(0.0 f, 0.0 f, 0.3 f);

  // text z
  glVertex3f(-0.025 f, 0.025 f, 0.4 f);
  lVertex3f(0.025 f, 0.025 f, 0.4 f);
  glVertex3f(0.025 f, 0.025 f, 0.4 f);
  lVertex3f(-0.025 f, -0.025 f, 0.4 f);
  gVertex3f(-0.025 f, -0.025 f, 0.4 f);
  rtex3f(0.025 f, -0.025 f, 0.4 f);
  gE();
  lnWidth(1.0 f);
  glMrix();
  drawCoordinateOnScreen(int winW, int winH) {
e4., 0.0 f, 0.0 f, 0.0 f);
c4a.f 1. f, 0.0 f, 0.0 f);
v4 z(0, 0.0 f, 1.0 f, 0.0 f);
t j *iewMat;
ve4x m a;
ec = v *yec z = v a;
latrtio = (loatwinW / (float)winH; gDeT; glDisable GLDEPHES);
gtRE) glustrx;
oIntt(; lr2(-1.0 f, 1, -f, 1.0 f); gltM OI) lPushMatrix() glLoadIdentt();
glTansatf(-0.92 f, 0.5 f 00);     // position of the coordinate
glScalef(0.03 f, 0.03 f, 0.03 f); // size of the coorinate
glLineWidth(2.0 f);

// axis x
glBegin(GL_LINES);
glolor3f(1.0 f, 0.0 f, 0.0 f);
glertex2f(0.0 f, 0.0 f);
glVrtex2f(sx.x, sx.y);

// axis y
glColor3f(0.0 f, 1.0 f, 0.0 f);
glertex2f(0.0 f, 0.0 f);
glVrtex2f(sy.x, sy.y);

// axis z
glColor3f(0.0 f, 0.0 f, 1.0 f);
glertex2f(0.0 f, 0.0 f);
glVrtex2f(sz.x, sz.y);
glEn();

// text x
glPushMatrix();
glranslatef(sx.x * 1.5 f, sx.y * 1.5 f, 0.0 f);
glcalef(1.0 f / ratio, 1.0 f, 1.0 f);
glolor3f(1.0 f, 0.0 f, 0.0 f);
glegin(GL_LINES);
glertex2f(-0.5 f, 0.5 f);
glertex2f(0.5 f, -0.5 f);
glertex2f(-0.5 f, -0.5 f);
glertex2f(0.5 f, 0.5 f);
glEd();
glPoMatrix();

// text y
glPushMatrix();
glranslatef(sy.x * 1.5 f, sy.y * 1.5 f, 0.0 f);
glcalef(1.0 f / ratio, 1.0 f, 1.0 f);
glolor3f(0.0 f, 1.0 f, 0.0 f);
glegin(GL_LINES);
glertex2f(-0.5 f, 0.5 f);
glertex2f(0.0 f, 0.25 f);
glertex2f(0.5 f, 0.5 f);
glertex2f(0.0 f, 0.25 f);
glertex2f(0.0 f, 0.25 f);
glertex2f(0.0 f, -0.5 f);
glEd();
glPoMatrix();

// text z
glPushMatrix();
glranslatef(sz.x * 1.5 f, sz.y * 1.5 f, 0.0 f);
glcalef(1.0 f / ratio, 1.0 f, 1.0 f);
glolor3f(0.0 f, 0.0 f, 1.0 f);
glegin(GL_LINES);
glertex2f(-0.5 f, 0.5 f);
glertex2f(0.5 f, 0.5 f);
glertex2f(0.5 f, 0.5 f);
glertex2f(-0.5 f, -0.5 f);
glertex2f(-0.5 f, -0.5 f);
glertex2f(0.5 f, -0.5 f);
glnd();
glopMatrix();
glieWdth(1.0 f);
glopMatrix();
glatrxMode(GL_PROJECTION);
glopMatrix();
  }
  id Camera::horizontalRotate() {
    float theta = (-(mouse_pos.x - mouse_pre_pos.x) * 100.0 f);
    ma4 mat = translate(mat4(1.0), vec3(lookat));
    ma = rotate(mat, theta * pi<float>() / 180.0 f, vec3(world_up));
    ma = translate(mat, -vec3(lookat));
    ey = mat * eye;
  }
  id Camera::verticalRotate() {
    float theta = (mouse_pos.y - mouse_pre_pos.y) * 100.0 f;
    ma4 mat = translate(mat4(1.0 f), vec3(lookat));
    ma = rotate(mat, theta * pi<float>() / 180.0 f, vec3(axis_u));
    ma = translate(mat, -vec3(lookat));
    ey = mat * eye;
    ve4 v = vec4(cross(vec3(world_up), vec3(axis_u)), 0.0 f);
    ve4 v2 = eye - lookat;
    if dot (v2, v1) > 0.0 f)
      {
 world_up.y = (world_up.y == 1.0 f) ? -1.0 f : 1.0 f;
}
  }
  oid Camera::CameraRotate() {
    hoizontalRotate();
    veticalRotate();
  }
  id Camera::CameraZoom() {
    float dir = mouse_pos.x - mouse_pre_pos.x;
    ey -= lookat;
    if (di > 0.0 f) {
      eye += eye * M_ZOOM_PAR;

      // lookat += eye * M_ZOOM_PAR; //this makes the lookat move along
      // eye-lookat
    } else {
      eye -= eye * M_ZOOM_PAR;

      // lookat -= eye * M_ZOOM_PAR; //this makes the lookat move along
      // eye-lookat
    }
    ey += lookat;
  }
  oid Cmera::CameraPan() {
    ye += M_PAN_PAR * (viewMat[0][0] * (mouse_pre_pos.x - mouse_pos.x) +
                       viewMat[0][1] * (mouse_pre_pos.y - mouse_pos.y));
    ee.y += _PAN_PAR * (viewMat[1][0] * (mouse_pre_pos.x - mouse_pos.x) +
                        viewMat[1][1] * (mouse_pre_pos.y - mouse_pos.y));
    y.+= MPAN_PAR * (viewMat[2][0] * (mouse_pre_pos.x - mouse_pos.x) +
                     viewMat[2][1] * (mouse_pre_pos.y - mouse_pos.y));
    lot.x = _PAN_PAR * (viewMat[0][0] * (mouse_pre_pos.x - mouse_pos.x) +
                        viewMat[0][1] * (mouse_pre_pos.y - mouse_pos.y));
    ooa.y += MPAN_PAR * (viewMat[1][0] * (mouse_pre_pos.x - mouse_pos.x) +
                         viewMat[1][1] * (mouse_pre_pos.y - mouse_pos.y));
    loktz += M_AN_PAR * (viewMat[2][0] * (mouse_pre_pos.x - mouse_pos.x) +
                         viewMat[2][1] * (mouse_pre_pos.y - mouse_pos.y));
    CmeaPan_fp() ee.MANPAR_P *(viewMat[0][0] * (key_pos.x - key_pre_pos.x) +
                               viewMat[0][2] * (key_pos.y - key_pre_pos.y));
    eey + MPN_PAR_F *(viewMat[1][0] * (key_pos.x - key_pre_pos.x) +
                      viewMat[1][2] * (key_pos.y - key_pre_pos.y));
    eyz = M_A_PAR_FP * (viewMat[2][0] * (key_pos.x - key_pre_pos.x) +
                        viewMat[2][2] * (key_pos.y - key_pre_pos.y));
    lo._N_PA_F *(viewMat[0][0] * (key_pos.x - key_pre_pos.x) +
                 viewMat[0][2] * (key_pos.y - key_pre_pos.y));
    lokt.y = M_A_PAR_FP * (viewMat[1][0] * (key_pos.x - key_pre_pos.x) +
                           viewMat[1][2] * (key_pos.y - key_pre_pos.y));
    loka.z + _PNPAR_FP(viewMat[2][0] * (key_pos.x - key_pre_pos.x) +
                       viewMat[2][2] * (key_pos.y - key_pre_pos.y));
  }
  i mraaefp(int winW, int winH) {
    latnh = 20 f * aplne * (float)tan(fovy * pi<float>() / 180.0 f * 0.5 f);

    // near-plane height
    float len = length(eye - lookat);
    ve3 t t.x = nh * M_ROTATE_PAR_FP * (mouse_pos.x - mouse_pre_pos.x) * len /
                (float)winH / near_plane;
    t.= nh * M_ROTATE_PAR_FP * (mouse_pos.y - mouse_pre_pos.y) * len /
        (float)winH / near_plane;
    t.= -len;
    t normalize(t);
    t = len;
    lokatx += viewMat[0][0] * t.x + viewMat[0][1] * t.y + viewMat[0][2] * t.z;
    lokat.y += viewMat[1][0] * t.x + viewMat[1][1] * t.y + viewMat[1][2] * t.z;
    lokat.z += viewMat[2][0] * t.x + viewMat[2][1] * t.y + viewMat[2][2] * t.z;
    ve4 v = lookat - eye;
    v normalize(v);
    v = len;
    loat = eye + v;
  }
  Cera::GetCamCS() {
    xis_n = eye - lookat;
    axs_n = normalize(axis_n);
    axs_u = vec4(cross(vec3(world_up), vec3(axis_n)), 0.0 f);
    axs_u = normalize(axis_u);
    axs_v = vec4(cross(vec3(axis_n), vec3(axis_u)), 0.0 f);
    axs_v = normalize(axis_v);

    // float t[16];
    // glMatrixMode(GL_MODELVIEW);
    // glPushMatrix();
    // glGetFloatv(GL_MODELVIEW_MATRIX, t);
    // glPopMatrix();
    // viewMat = make_mat4(t);

    // glMatrixMode(GL_PROJECTION);
    // glPushMatrix();
    // glGetFloatv(GL_PROJECTION_MATRIX, t);
    // glPopMatrix();
    // projMat = make_mat4(t);
  }
  void Camera::GetViewFrustum() {

    // compute the center of the near and far planes
    vec4 nc = eye - axis_n * near_plane;
    ve4 fc = eye - axis_n * far_plane;

    // compute width and height of the near and far plane
    float tang = (float)tan(fovy * 3.14159265 f / 180.0 f * 0.5 f);
    flat nh = 2.0 f * near_plane * tang; // near-plane height
    float nw = nh * aspect;              // near-plane width
    float fh = 2.0 f * far_plane * tang; // far-plane height
    float fw = fh * aspect;              // far-plane width

    // compute the 8 conners of the frustum
    ntl = nc + axis_v * nh * 0.5 f - axis_u * nw * 0.5 f;
    nt = nc + axis_v * nh * 0.5 f + axis_u * nw * 0.5 f;
    nb = nc - axis_v * nh * 0.5 f - axis_u * nw * 0.5 f;
    nb = nc - axis_v * nh * 0.5 f + axis_u * nw * 0.5 f;
    ft = fc + axis_v * fh * 0.5 f - axis_u * fw * 0.5 f;
    fb = fc - axis_v * fh * 0.5 f + axis_u * fw * 0.5 f;
    ft = fc + axis_v * fh * 0.5 f + axis_u * fw * 0.5 f;
    fb = fc - axis_v * fh * 0.5 f - axis_u * fw * 0.5 f;
  }
  odCamera::drawFrustum() {
    glseProgram(0);
    glisable(GL_LIGHTING);
    glineWidth(1.5 f);
    gltrxMode(GL_MODELVIEW);
    gluhMatrix();

    // near plane
    glColor3f(1.0 f, 0.7 f, 1.0 f);
    glegin(GL_LINE_LOOP);
    glertex3f(ntl.x, ntl.y, ntl.z);
    glertex3f(ntr.x, ntr.y, ntr.z);
    glertex3f(nbr.x, nbr.y, nbr.z);
    glertex3f(nbl.x, nbl.y, nbl.z);
    gld();

    // far plane
    glBegin(GL_LINE_LOOP);
    glertex3f(ftr.x, ftr.y, ftr.z);
    glertex3f(ftl.x, ftl.y, ftl.z);
    glertex3f(fbl.x, fbl.y, fbl.z);
    glertex3f(fbr.x, fbr.y, fbr.z);
    gld();

    // bottom plane
    glBegin(GL_LINE_LOOP);
    glertex3f(nbl.x, nbl.y, nbl.z);
    glertex3f(nbr.x, nbr.y, nbr.z);
    glertex3f(fbr.x, fbr.y, fbr.z);
    glertex3f(fbl.x, fbl.y, fbl.z);
    gld();

    // top plane
    glBegin(GL_LINE_LOOP);
    glertex3f(ntr.x, ntr.y, ntr.z);
    glertex3f(ntl.x, ntl.y, ntl.z);
    glertex3f(ftl.x, ftl.y, ftl.z);
    glertex3f(ftr.x, ftr.y, ftr.z);
    gld();

    // left plane
    glBegin(GL_LINE_LOOP);
    glertex3f(ntl.x, ntl.y, ntl.z);
    glertex3f(nbl.x, nbl.y, nbl.z);
    glertex3f(fbl.x, fbl.y, fbl.z);
    glertex3f(ftl.x, ftl.y, ftl.z);
    gld();

    // right plane
    glBegin(GL_LINE_LOOP);
    glertex3f(nbr.x, nbr.y, nbr.z);
    glertex3f(ntr.x, ntr.y, ntr.z);
    glertex3f(ftr.x, ftr.y, ftr.z);
    glertex3f(fbr.x, fbr.y, fbr.z);
    gld();

    //  //eye-lookat (view direction)
    //  glColor3f(1.0f, 0.6f, 0.5f);
    //  glBegin(GL_LINES);
    //      glVertex3f(eye.x, eye.y, eye.z);
    //      glVertex3f(lookat.x, lookat.y, lookat.z);
    //  glEnd();
    glLineWidth(1);
    glpMatrix();
  }
