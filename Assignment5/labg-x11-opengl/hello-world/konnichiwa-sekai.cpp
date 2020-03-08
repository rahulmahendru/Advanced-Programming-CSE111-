// $Id: konnichiwa-sekai.cpp,v 1.2 2019-05-15 17:16:28-07 - - $

// Draw line from (0,0) to (1,1).

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
   int width = 384;
   int height = 256;
} window;

const GLubyte WHITE[] {255, 255, 255};
const GLubyte CRIMSON_GLORY[] {188, 0, 45};
constexpr GLfloat aspect_ratio = 2.0 / 3.0;

void draw_japanese_flag() {
   glBegin (GL_POLYGON);
   glColor3ubv (WHITE);
   glVertex2f (0, 0);
   glVertex2f (window.width, 0);
   glVertex2f (window.width, window.height);
   glVertex2f (0, window.height);
   glEnd();
   glBegin (GL_POLYGON);
   glColor3ubv (CRIMSON_GLORY);
   const GLfloat delta = 2 * M_PI / 64;
   const GLfloat radius = window.height * 3.0 / 10.0;
   const GLfloat xoffset = window.width / 2.0;
   const GLfloat yoffset = window.height / 2.0;
   for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
      GLfloat xpos = radius * cos (theta) + xoffset;
      GLfloat ypos = radius * sin (theta) + yoffset;
      glVertex2f (xpos, ypos);
   }
   glEnd();
}

void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_japanese_flag();
   glutSwapBuffers();
}


void adjust_aspect (int width, int height) {
   if (window.width != width) {
      height = width * aspect_ratio;
   }else if (window.height != height) {
      width = height / aspect_ratio;
   }else { 
      return;
   }
   window.width = width;
   window.height = height; 
   glutReshapeWindow (window.width, window.height);
}

void reshape (int width, int height) {
   adjust_aspect (width, height);
   window.width = width;
   window.height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window.width, 0, window.height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window.width, window.height);
   glutPostRedisplay();
}

int main (int argc, char** argv) {
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf konnichiwa-sekai.cpp.ps konnichiwa-sekai.cpp*

