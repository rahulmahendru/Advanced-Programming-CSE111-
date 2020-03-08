// $Id: glversion.cpp,v 1.16 2019-02-22 15:38:20-08 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#define SHOW(X) { \
           const char* value = \
              reinterpret_cast<const char*> (glGetString(X)); \
           cout << #X << ": " << (value ? value : "(nullptr)") \
                << endl; \
        }

void display() {
    SHOW (GL_VENDOR);
    SHOW (GL_RENDERER);
    SHOW (GL_VERSION);
    SHOW (GL_SHADING_LANGUAGE_VERSION);
    exit (0);
}

int main (int argc, char** argv) {
    glutInit (&argc, argv);
    glutCreateWindow (argv[0]);
    glutDisplayFunc (display);
    glutMainLoop();
    return 0;
}

//TEST// glversion >glversion.out 2>&1
//TEST// mkpspdf glversion.ps glversion.cpp* glversion.out

