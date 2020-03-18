// $Id: countdown-clock.cpp,v 1.1 2019-08-14 12:47:34-07 - - $

// Show a real-time analog clock.

#include <cmath>
#include <iostream>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>
#include <time.h>

struct {
   int width = 640;
   int height = 350;
   string progname;
   string end_time;
} window;

static const float RADIUS = 0.9;
static const GLubyte BLUE[] {0x00, 0x00, 0xFF};
static const GLubyte YELLOW[] {0xFF, 0xFF, 0x00};
static const GLubyte RED[] {0xFF, 0x00, 0x00};
static const GLubyte BLACK[] {0x00, 0x00, 0x00};

//void* Stroke_Roman = GLUT_STROKE_ROMAN;
//void* Stroke_Mono_Roman = GLUT_STROKE_MONO_ROMAN;

string time_now() {
   time_t clock = time (nullptr);
   struct tm localtime;
   localtime_r (&clock, &localtime);
   char stime[64];
   strftime (stime, sizeof stime, "%I:%M:%S", &localtime);
   return stime;
}

pair<const GLubyte*,string> time_to_stop (const string& stop_time_s) {
   time_t clock = time (nullptr);
   struct tm localtime;
   localtime_r (&clock, &localtime);
   int hour, min, sec;
   sscanf (stop_time_s.c_str(), "%d:%d:%d", &hour, &min, &sec);
   localtime.tm_hour = hour;
   localtime.tm_min = min;
   localtime.tm_sec = sec;
   time_t stop_time = timelocal (&localtime);
   time_t delta_time = stop_time - clock;
   const GLubyte* color = BLACK;
   if (delta_time < 0) {
      delta_time = -delta_time;
      color = RED;
   }
   sec = delta_time % 60;
   min = delta_time / 60 % 60;
   hour = delta_time / 60 / 60;
   char buffer[10];
   snprintf (buffer, sizeof buffer, "%02d:%02d:%02d", hour, min, sec);
   return {color, buffer};
}

const GLubyte* to_ubytes (const char* cstring) {
   return reinterpret_cast<const GLubyte*> (cstring);
}

void show_string (int xpos, int ypos, const string& time) {
   void* font = GLUT_STROKE_ROMAN;
   glPushMatrix();
   glLineWidth (20);
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_POLYGON_SMOOTH);
   glTranslatef (xpos, ypos, 0);
   for (char ch: time) {
      glutStrokeCharacter (font, ch);
      glTranslatef (12, 0, 0);
   }
   glPopMatrix();
}

void display() {
   glClearColor (0.75, 0.75, 0.75, 1.0);
   glClear (GL_COLOR_BUFFER_BIT);
   auto stop_info = time_to_stop (window.end_time);
   const GLubyte* time_color = stop_info.first == RED ? YELLOW : BLUE;
   glColor3ubv (time_color);
   show_string (20, 200, time_now());
   glColor3ubv (stop_info.first);
   show_string (20,  50, stop_info.second);
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
   gluOrtho2D (0, window.width, 0, window.height);
   glMatrixMode (GL_MODELVIEW);
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_POLYGON_SMOOTH);
   glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
   glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
   glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glViewport (0, 0, window.width, window.height);
}

void usage() {
   cerr << "Usage: " << window.progname << "stop_time" << endl;
   cerr << "where stop_time is HH:MM:SS (not am/pm)" << endl;
   exit (1);
}

int main (int argc, char** argv) {
   window.progname = basename (argv[0]);
   glutInit (&argc, argv);
   if (argc != 2) usage();
   window.end_time = argv[1];
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (window.progname.c_str());
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutTimerFunc (frequency_msec, timer, 100);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf glclock.ps glclock.cpp*

