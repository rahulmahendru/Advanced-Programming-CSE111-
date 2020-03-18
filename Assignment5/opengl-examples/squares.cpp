// $Id: squares.cpp,v 1.17 2019-02-22 17:38:45-08 - - $

// Draw a square.

#include <iostream>
#include <string>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
   string name;
   int width {256};
   int height {256};
} window;

const GLubyte RED[] = {0xFF, 0x00, 0x00};
const GLubyte GREEN[] = {0x00, 0xFF, 0x00};
const GLubyte BLUE[] = {0x00, 0x00, 0xFF};
const GLubyte* colors[] = {RED, GREEN, BLUE};
constexpr size_t colors_size = sizeof colors / sizeof *colors;

void draw_square (GLfloat xpos, GLfloat ypos,
                  GLfloat width, const GLubyte* color) {
   glBegin (GL_POLYGON);
   glColor3ubv (color);
   glVertex2f (xpos, ypos);
   glVertex2f (xpos + width, ypos);
   glVertex2f (xpos + width, ypos + width);
   glVertex2f (xpos, ypos + width);
   glEnd();
}

void display() {
   glClearColor (0.5, 0.5, 0.5, 1.0);
   glClear (GL_COLOR_BUFFER_BIT);
   for (size_t index = 0; index < colors_size; ++index) {
      GLfloat offset = (index + 1) * 0.10;
      GLfloat width = 1 - 2 * offset;
      draw_square (window.width * offset, window.height * offset,
                window.width * width, colors[index]);
   }
   glutSwapBuffers();
}


void reshape (int width, int height) {
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
   glutInitWindowPosition (128, 128);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf squares.ps squares.cpp*

