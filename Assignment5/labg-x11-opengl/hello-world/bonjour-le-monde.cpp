// $Id: bonjour-le-monde.cpp,v 1.11 2019-05-15 17:12:35-07 - - $

// Draw line from (0,0) to (1,1).

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
   int width {384};
   int height {256};
} window;

const GLubyte BLEU[] {0, 85, 164};
const GLubyte BLANC[] {255, 255, 255};
const GLubyte ROUGE[] {239, 65, 53};
const vector<const GLubyte*> colors {BLEU, BLANC, ROUGE};
constexpr GLfloat aspect_ratio = 2.0 / 3.0;

void draw_french_flag() {
   for (size_t i = 0; i < 3; ++i) {
      glBegin (GL_POLYGON);
      glColor3ubv (colors[i]);
      glVertex2f (window.width * i / 3.0, 0);
      glVertex2f (window.width * (i + 1) / 3.0, 0);
      glVertex2f (window.width * (i + 1) / 3.0, window.height);
      glVertex2f (window.width * i / 3.0, window.height);
      glEnd();
   }
}

void display() {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear (GL_COLOR_BUFFER_BIT);
   draw_french_flag();
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
   glutInitWindowPosition (256, 256);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf bonjour-le-monde.cpp.ps bonjour-le-monde.cpp*

