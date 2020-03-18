// $Id: glclock.cpp,v 1.6 2019-03-21 17:00:11-07 - - $

// Show a real-time analog clock.

#include <cmath>
#include <iostream>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>
#include <time.h>

struct {
   int width = 256;
   int height = 256;
} window;

string program_name;
static const float RADIUS = 0.9;
static const GLubyte FOREGROUND[] {0x00, 0x00, 0xFF};

struct calend {
   time_t clock;
   struct tm localtime;
   char sdate[64];
   char stime[64];
   void set() {
      clock = time (nullptr);
      localtime_r (&clock, &localtime);
      strftime (sdate, sizeof sdate, "%a %b %e", &localtime);
      strftime (stime, sizeof stime, "%I:%M:%S %p", &localtime);
      if (stime[0] == '0') stime[0] = ' ';
      for (char* chr = stime; *chr != '\0'; ++chr) {
         if (isupper(*chr)) *chr = tolower (*chr);
      }
   }
} calend;

const GLubyte* to_ubytes (const char* cstring) {
   return reinterpret_cast<const GLubyte*> (cstring);
}

void show_time() {
   void* font = GLUT_BITMAP_HELVETICA_18;
   glRasterPos2f (-0.95, -0.95);
   glutBitmapString (font, to_ubytes (calend.sdate));
   float timewidth = glutBitmapLength (font, to_ubytes (calend.stime));
   float timexpos = 0.95 - 2 * timewidth / window.width;
   glRasterPos2f (timexpos, -.95);
   glutBitmapString (font, to_ubytes (calend.stime));
}


void draw_dots (int points, int count) {
   glEnable (GL_POINT_SMOOTH);
   glPointSize (points);
   glBegin(GL_POINTS);
   glColor3ubv (FOREGROUND);
   for (float theta = 0; theta < 2 * M_PI; theta += 2 * M_PI / count) {
      float xdot = 0.9 * RADIUS * cos (theta);
      float ydot = 0.9 * RADIUS * sin (theta);
      glVertex2f (xdot, ydot);
   }
   glEnd();
}

void draw_hand (GLfloat width, GLfloat length, GLfloat clock) {
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_POLYGON_SMOOTH);
   glPushMatrix();
   glRotatef (-clock * 6, 0, 0, 1);
   glColor3ubv (FOREGROUND);
   glBegin (GL_POLYGON);
   glVertex2f (-width / 2 * RADIUS, 0);
   glVertex2f (+width / 2 * RADIUS, 0);
   glVertex2f (+width / 8, length * RADIUS);
   glVertex2f (-width / 8, length * RADIUS);
   glEnd();
   glPopMatrix();
}

void display() {
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3ubv (FOREGROUND);
   draw_dots (4, 60);
   draw_dots (8, 12);
   calend.set();
   float second = calend.localtime.tm_sec;
   float minute = calend.localtime.tm_min + second / 60;
   float hour = calend.localtime.tm_hour + minute / 60;
   draw_hand (0.1, 0.6, hour * 5);
   draw_hand (0.075, 0.8, minute);
   draw_hand (0.05, 1.0, second);
   show_time();
   glutSwapBuffers();
}

const float frequency_msec = 500;
void timer (int) {
   glutTimerFunc (frequency_msec, timer, 100);
   glutPostRedisplay();
}


void reshape (int width, int height) {
   cout << "reshape(width=" << width << ", height=" << height << endl;
   window.width = width;
   window.height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (-1, +1, -1, +1);
   glMatrixMode (GL_MODELVIEW);
   glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
   glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
   glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glViewport (0, 0, window.width, window.height);
   glClearColor (0xFFp0/255, 0xFFp0/255, 0x0Fp0/255, 1.0);
}

int main (int argc, char** argv) {
   program_name = basename (argv[0]);
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (program_name.c_str());
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutTimerFunc (frequency_msec, timer, 100);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf glclock.ps glclock.cpp*

