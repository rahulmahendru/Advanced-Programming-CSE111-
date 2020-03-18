// $Id: graphics.cpp,v 1.6 2019-05-15 18:02:12-07 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
vector<object> window::objects;
size_t window::selected_obj = 0;
mouse window::mus;
int window::moveByPixels = 4;  // default move pixels
bool window::isSelected = false; // default check value

// Implementation of object functions.
object::object (shared_ptr<shape> pshape_, vertex center_,
                rgbcolor color_):
      pshape(pshape_), center(center_), color(color_) {
}

void object::draw(int objectPosition) {
   pshape->draw (center, color, objectPosition);
}

void object::move (GLfloat delta_x, GLfloat delta_y) {
   center.xpos += delta_x;
   center.ypos += delta_y;

   // Check to keep object in bounds
   center.xpos = center.xpos == 0 ? window::getwidth() : 
               static_cast<int>(center.xpos) % window::getwidth();
   center.ypos = center.ypos == 0 ? window::getheight() : 
               static_cast<int>(center.ypos) % window::getheight();
}

// Implementation of mouse functions.
void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      auto ubytes = reinterpret_cast<const GLubyte*>
                    (text.str().c_str());
      glutBitmapString (font, ubytes);
   }
}


// Executed when window system signals to shut down.
void window::close() {
   DEBUGF ('g', sys_info::execname() << ": exit ("
           << sys_info::exit_status() << ")");
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   DEBUGF ('g', "mouse_entered=" << mouse_entered);
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
           << ", height=" << window::height);
   }
   glutPostRedisplay();
}

// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   size_t selectCheck = 0;  // Counter to track objects
   size_t counter = 0;
   for (auto& object: window::objects){
      if (selectCheck++ == selected_obj){
         ++counter;
         continue;
      } 
      object.draw(counter++);
   }
   isSelected = true;
   if (objects.size() >=1) 
      window::objects[selected_obj].draw(selected_obj);
   isSelected = false;
   mus.draw();
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width_, int height_) {
   DEBUGF ('g', "width=" << width_ << ", height=" << height_);
   window::width = width_;
   window::height = height_;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}


// Executed when a regular keyboard key is pressed.
void window::keyboard (GLubyte key, int x, int y) {
   enum {BS = 8, TAB = 9, ESC = 27, SPACE = 32, DEL = 127};
   DEBUGF ('g', "key=" << unsigned (key) << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case 'Q': case 'q': case ESC:
         window::close();
         break;
      case 'H': case 'h':
         //move_selected_object left
         objects[selected_obj].move(-1 * moveByPixels, 0);
         break;
      case 'J': case 'j':
         //move_selected_object down
         objects[selected_obj].move(0, -1 * moveByPixels);
         break;
      case 'K': case 'k':
         //move_selected_object up
         objects[selected_obj].move(0, moveByPixels);
         break;
      case 'L': case 'l':
         //move_selected_object right
         objects[selected_obj].move(moveByPixels, 0);
         break;
      case 'N': case 'n': case SPACE: case TAB:
         if (++selected_obj == objects.size()) selected_obj = 0;
         break;
      case 'P': case 'p': case BS:
         if (selected_obj == 0) selected_obj = objects.size() - 1;
         else --selected_obj;
         break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
         //select_object (key - '0');
         if (unsigned(key - '0') < objects.size()) 
         selected_obj = key - '0';
         else cerr << unsigned (key) << ": invalid keystroke" << endl;
         break;
      default:
         cerr << unsigned (key) << ": invalid keystroke" << endl;
         break;
   }
   glutPostRedisplay();
}


// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   DEBUGF ('g', "key=" << key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case GLUT_KEY_LEFT: //move_selected_object (-1, 0); break;
         objects[selected_obj].move(-1 * moveByPixels, 0);
         break;
      case GLUT_KEY_DOWN: //move_selected_object (0, -1); break;
         objects[selected_obj].move(0, -1 * moveByPixels);
         break;
      case GLUT_KEY_UP: //move_selected_object (0, +1); break;
         objects[selected_obj].move(0, moveByPixels);
         break;
      case GLUT_KEY_RIGHT: //move_selected_object (+1, 0); break;
         objects[selected_obj].move(moveByPixels, 0);
         break;
      case GLUT_KEY_F1: //select_object (1); break;
         if (objects.size() > 0) selected_obj = 0;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F2: //select_object (2); break;
         if (objects.size() > 1) selected_obj = 1;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F3: //select_object (3); break;
         if (objects.size() > 2) selected_obj = 2;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F4: //select_object (4); break;
         if (objects.size() > 3) selected_obj = 3;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F5: //select_object (5); break;
         if (objects.size() > 4) selected_obj = 4;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F6: //select_object (6); break;
         if (objects.size() > 5) selected_obj = 5;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F7: //select_object (7); break;
         if (objects.size() > 6) selected_obj = 6;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F8: //select_object (8); break;
         if (objects.size() > 7) selected_obj = 7;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F9: //select_object (9); break;
         if (objects.size() > 8) selected_obj = 8;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F10: //select_object (10); break;
         if (objects.size() > 9) selected_obj = 9;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F11: //select_object (11); break;
         if (objects.size() > 10) selected_obj = 10;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      case GLUT_KEY_F12: //select_object (12); break;
         if (objects.size() > 11) selected_obj = 11;
         else cerr << unsigned (key) 
                   << ": invalid function key" << endl;
         break;
      default:
         cerr << unsigned (key) << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}


void window::motion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   DEBUGF ('g', "button=" << button << ", state=" << state
           << ", x=" << x << ", y=" << y);
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::main () {
   static int argc = 0;
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   glutCloseFunc (window::close);
   glutEntryFunc (window::entry);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   glutKeyboardFunc (window::keyboard);
   glutSpecialFunc (window::special);
   glutMotionFunc (window::motion);
   glutPassiveMotionFunc (window::passivemotion);
   glutMouseFunc (window::mousefn);
   DEBUGF ('g', "Calling glutMainLoop()");
   glutMainLoop();
}

