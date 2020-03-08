// $Id: teapot.cpp,v 1.2 2019-02-22 15:37:19-08 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

bool wantsolid = true;

void display() {

    /* clear window */
    glClear (GL_COLOR_BUFFER_BIT);

    /* draw scene */
    if (wantsolid) glutSolidTeapot (.5);
              else glutWireTeapot (.5);

    /* flush drawing routines to the window */
    glFlush();

}

void mouse (int button, int state, int x, int y) {
   if (state == GLUT_DOWN) wantsolid = not wantsolid;
   cout << boolalpha;
   cout << "button=" << button << ", state=" << state
        << ", x=" << x << ", y=" << y
        << ", wantsolid=" << wantsolid << endl;
   glutPostRedisplay();
}

int main (int argc, char** argv) {

    // Initialize GLUT, using any commandline parameters passed.
    glutInit (&argc, argv);

    // Setup the size, position, and display mode for new windows.
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (0, 0);
    glutInitDisplayMode (GLUT_RGB);

    // Create and set up a window.
    glutCreateWindow (basename (argv[0]));
    glutDisplayFunc (display);
    glutMouseFunc (mouse);

    // Tell GLUT to wait for events.
    glutMainLoop();
}

//TEST// mkpspdf teapot.ps teapot.cpp*

