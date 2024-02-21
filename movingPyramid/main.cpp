#include <chrono>
#include <iostream>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <stdio.h>


#define ANGLE_STEP 10

int pir_x, pir_y, pir_z;
int cam_x, cam_y, cam_z;
float cam_step;

void initValues() 
{
    pir_x = pir_y = pir_z = 0;
    cam_x = 0;
    cam_y = cam_z = 5;
    cam_step= 0.0f;
}

void changeSize(int w, int h) 
{
  if (h == 0)
    h = 1;

  float ratio = w * 1.0f / h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
}

void truncAngle(float* angle)
{
    if (*angle< 0)
    { 
        while (aux< 0)
        {
            aux+= 360.0f;
        }
        
    }
    else 
    {
        while (*angle> 360.0f)
        {
            *angle-= 360.0f;
        }
    }
}

float updateAngle ()
{
    static auto last_frame_time= std::chrono::steady_clock::now();
    static float angle= 0.0f;

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed_time= now- last_frame_time;
    last_frame_time= std::chrono::steady_clock::now();

    angle+= elapsed_time.count()* cam_step;
    truncAngle(&angle);

    return angle;
}

void renderScene(void) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(cam_x, cam_y, cam_z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(pir_x, pir_y, pir_z);

    glRotatef(updateAngle(), 0, 1.0f, 0);
    glutWireTeapot(1);
    glutSwapBuffers();
}

void handleKeyboard(unsigned char key, int x, int y) 
{
    switch (key) 
    {
        case 'w': 
        {
            pir_z--;
            break;
        } 
        case 's': 
        {
            pir_z++;
            break;
        }
        case 'a': 
        {
            pir_x--;
            break;
        }
        case 'd': 
        {
            pir_x++;
            break;
        }
        case 'j': 
        {
            cam_y++;
            break;
        }
        case 'k': 
        {
            cam_y--;
            break;
        }
        case 'h': 
        {
            cam_x--;
            break;
        }
        case 'l': 
        {
            cam_x++;
            break;
        }
        case '+':
        {
            if (cam_step< 1080)
                cam_step+= ANGLE_STEP; 
            break; 
        } 
        case '-': 
        { 
            if (cam_step> -1080) 
                cam_step-= ANGLE_STEP;
            break; 
        } 
    } 
}

void printInfo() {

  printf("Vendor: %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("Version: %s\n", glGetString(GL_VERSION));
}

int main(int argc, char **argv) {
  // init: global vars
  initValues();

  // put GLUT init here
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 800);
  glutCreateWindow("MovingPot");

  // put callback registry here
  glutReshapeFunc(changeSize);
  glutIdleFunc(renderScene);
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(handleKeyboard);

  // some OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // enter GLUTs main cycle
  glutMainLoop();

  return 1;
}
