// $Id: color-wheel.cpp,v 1.25 2019-02-22 15:36:02-08 - - $

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

// Characteristics of the window.
struct window {
   string name;
   int width {640};
   int height {480};
} window;

const GLubyte WHITE[]   {255, 255, 255};
const GLubyte BLACK[]   {  0,   0,   0};
const GLubyte RED[]     {255,   0,   0};
const GLubyte GREEN[]   {  0, 255,   0};
const GLubyte BLUE[]    {  0,   0, 255};
const GLubyte CYAN[]    {  0, 255, 255};
const GLubyte MAGENTA[] {255,   0, 255};
const GLubyte YELLOW[]  {255, 255,   0};

vector<const GLubyte*> color_wheel {
   YELLOW, RED, MAGENTA, BLUE, CYAN, GREEN,
};

void draw_6th_wedge (GLenum mode, const GLubyte* color,
                     const GLfloat angle, const GLfloat radius) {
   const GLfloat max_angle = angle + M_PI / 3.0;
   const GLfloat delta = M_PI / 30.0;
   glBegin (mode);
   glColor3ubv (color);
   glVertex2f (0.0, 0.0);
   for (GLfloat theta = angle; theta <= max_angle; theta += delta) {
      GLfloat xpos = radius * cos (theta);
      GLfloat ypos = radius * sin (theta);
      glVertex2f (xpos, ypos);
   }
   glEnd();
}

void draw_6th_pie (const GLubyte* color,
                   const GLfloat angle, const GLfloat radius) {
   draw_6th_wedge (GL_POLYGON, color, angle, radius);
   glLineWidth (5.0);
   draw_6th_wedge (GL_LINE_LOOP, BLACK, angle, radius);
}


void display() {
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glClear (GL_COLOR_BUFFER_BIT);

   for (size_t color = 0; color != color_wheel.size(); ++color) {
      draw_6th_pie (color_wheel[color], M_PI / 3.0 * color,
                    min (window.width, window.height) * 0.4);
   }

   glutSwapBuffers();
}

void reshape (int width, int height) {
   window.width = width;
   window.height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (- window.width / 2.0, + window.width / 2.0,
               - window.height / 2.0, + window.height / 2.0);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window.width, window.height);
   glutPostRedisplay();
}

int main (int argc, char** argv) {
   window.name = basename (argv[0]);
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (window.name.c_str());
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf color-wheel.ps color-wheel.cpp*

