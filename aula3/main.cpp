#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define N_SLICES 100

void changeSize(int w, int h) {
  // Prevent a divide by zero, when window is too short
  // (you cant make a window with zero width).
  if (h == 0)
    h = 1;

  // compute window's aspect ratio
  float ratio = w * 1.0 / h;

  // Set the projection matrix as current
  glMatrixMode(GL_PROJECTION);
  // Load Identity Matrix
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set perspective
  gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

  // return to the model view matrix mode
  glMatrixMode(GL_MODELVIEW);
}

void drawSlice(float radius, float height, float angle, float alpha) {
  static bool stepper = true;
  float dark;
  if (stepper) {
    dark = 0.2f;
    stepper = false;
  } else {
    dark = 0;
    stepper = true;
  }

  // Calculate the coordinates of the four corners of the slice
  float x1 = radius * cos(angle);
  float y1 = radius * sin(angle);
  float x2 = radius * cos(angle + alpha);
  float y2 = radius * sin(angle + alpha);

  // Draw the slice
  // top triang
  glColor3f(1.0f - dark, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(x1, y1, 0.0f);
  glVertex3f(x2, y2, 0.0f);

  // bot triang
  glVertex3f(0.0f, 0.0f, height);
  glVertex3f(x1, y1, height);
  glVertex3f(x2, y2, height);
  glEnd();

  // side triangs
  glBegin(GL_QUADS);
  glColor3f(0.0f, 1.0f - dark, 0.0f);
  glVertex3f(x1, y1, 0.0f);
  glVertex3f(x2, y2, 0.0f);
  glVertex3f(x2, y2, height);
  glVertex3f(x1, y1, height);
  glEnd();
}

void drawCylinder(float radius, float height, int slices) {
  float alpha = 2 * M_PI / slices;

  for (int i = 0; i < slices; i++) {
    drawSlice(radius, height, (float)i * alpha, alpha);
  }
}

void renderScene(void) {

  // clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the camera
  glLoadIdentity();
  gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  drawCylinder(1, 2, N_SLICES);

  // End of frame
  glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {

  // put code to process regular keys in here
}

void processSpecialKeys(int key, int xx, int yy) {

  // put code to process special keys in here
}

int main(int argc, char **argv) {
  // init GLUT and the window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 800);
  glutCreateWindow("CG@DI-UM");

  // Required callback registry
  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);

  // Callback registration for keyboard processing
  glutKeyboardFunc(processKeys);
  glutSpecialFunc(processSpecialKeys);

  //  OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
