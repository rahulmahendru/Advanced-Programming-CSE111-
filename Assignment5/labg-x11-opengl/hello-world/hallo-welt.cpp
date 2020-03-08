// $Id: hallo-welt.cpp,v 1.9 2019-05-15 17:12:55-07 - - $

// Draw line from (0,0) to (1,1).

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
   int width = 385;
   int height = 231;
} window;

const GLubyte SCHWARZ[] {0x00, 0x00, 0x00};
const GLubyte ROT[] {0xFF, 0x00, 0x00};
const GLubyte GOLD[] {0xFF, 0xCC, 0x00};
const vector<const GLubyte*> colors {GOLD, ROT, SCHWARZ};
constexpr GLfloat aspect_ratio = 3.0 / 5.0;

void draw_german_flag() {
   for (size_t i = 0; i < 3; ++i) {
      glBegin (GL_POLYGON);
      glColor3ubv (colors[i]);
      glVertex2f (0, window.height * i / 3.0);
      glVertex2f (0, window.height * (i + 1) / 3.0);
      glVertex2f (window.width, window.height * (i + 1) / 3.0);
      glVertex2f (window.width, window.height * i / 3.0);
      glEnd();
   }
}

void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_german_flag();
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
   window.height = height;
   window.width = width;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window.width, 0, window.height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window.width, window.height);
   glutPostRedisplay();
}

int main (int argc, char** argv) {
   glutInit (&argc, argv);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf hallo-welt.cpp.ps hallo-welt.cpp*

