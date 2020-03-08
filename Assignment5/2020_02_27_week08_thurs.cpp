/*
Example/opengl-examples/glclock.cpp
   * CodeEx A
Examples/opengl-examples/???
   * CodeEx B
Examples/opengl-examples/mousetrace.cpp
   * CodeEx C
./translate.cpp
   * CodeEx D
./spincolors.cpp
   * CodeEx E
Assignments/asg5-oop-opengl/asg5-oop-opengl.pdf
   * CodeEx F
Assignments/asg5-oop-opengl/code/mk-colors.perl
Assignments/asg5-oop-opengl/code/Makefile
   * CodeEx G
Assignments/asg5-oop-opengl/code/main.cpp
   * CodeEx H
Assignments/asg5-oop-opengl/code/shape.h
   * CodeEx I
Assignments/asg5-oop-opengl/code/shape.cpp
   * CodeEx J
Assignments/asg5-oop-opengl/code/interp.h
   * CodeEx K
*/

{
   // Code example A:
   glutTimerFunc(frequency_msec, timer, 100);   // timer callback
   // wake up timer function every 100 milliseconds to perform timer()
   //  Within timer(), we'll update the clock and cause a redraw
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // should make lines smooth?
   glClearColor(0x2Fp0/255, 0x9Fp0/255, 0x2Fp0/255);
   //  0x2Fp0: special way to write floating point in hex.
   // ... 
   glRotatef(-clock*6, 0, 0, 1); // rotate clock and draw hand UPWARDS
   /* Remember: GLubyte is an unsigned char (single unsigned byte)
      => compared to char (which is a signed byte) */
   
   // Code example B:
   glutEntryFunc(entry);   // every time you move your mouse inside or
   // outside the window, we will call "entry", and the mouse state will
   // be passed into the arguments of entry:
   void entry(int state) {
      // state: entered or left? it will tell you
      // GLUT_LEFT or GLUT_ENTERED
   }
   // GLUT_KEY*: programs can recognize key presses from user.
   
   // Code example C:
   void mouse_func(int /*button*/, int /*state*/, int /*mouse_x*/,
                   int /*mouse_y*/) {
      // These are pretty self-explanatory
      // NOTE: These coordinates start 0,0 from TOPLEFT. These coords
      //  are ALWAYS per pixel!
      // button states: GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, 
      //    GLUT_MIDDLE_BUTTON
      /* GLUT_DOWN, GLUT_UP: Determine button state: pressed or not */
   )
   
   // Code example D:
   glutSetWindowTitle(...);   // change window title
   union {
      int x;
      char c[4];
   ]; // little endian: c[0]: lowest order byte, big end: highest order
   cout << hex << someNumber << endl;  // "hex" will manipulate what
   //                                     gets printed out...
   glPushMatrix();   // saves the world's coordinates
   glTranslatef(xcenter, ycenter, 0); // also glRotatef(...);
   glVertex2f(-delta_x, -delta_x);
   // more glVertex2f(...);
   //    These will be copy-pasted to where we specify...
   
   // Code example E:
   // Rotating triangle that acts like a clock.
   //    How to rotate world: glRotatef(...);
   // "As we go from 0 to 10 seconds, we want to increase the amount of
   //    green.
   
   // Code example F:
   // When the program begins, you start out in the TERMINAL state.
   //    -> In this state, you will provide objects to define, then you
   //       will draw these objects with "draw".
   /* Two major commands (there are some other; read PDF):
      $ define myObj ...    # defines an object and names it
      $ draw myObj ...      # draws myObj to the screen after Ctrl+D
      $ draw myObj ...      # draws another instance of myObj         */
   // Note, if you draw an object, except for text, that's partially
   //  outside of the window, then that partial portion will show on
   //  screen. With text, if a part of it is outside, then it won't draw
   //  at all.
   // === www.opengl.org/documentation === or just use a websearch
   
   // Code example G:
   // DO NOT SUBMIT colors.cppgen TO YOUR SUBMISSION FOLDER!!!
   //  Be sure to instead submit mk-colors.perl
   // As part of the Makefile compile, we will generate colors.cppgen
   //  using the perl script:
   "\
   colors.cppgen: mk-colors.perl     \
         perl mk-colors.perl         " // within Makefile
   
   // Code example H:
   // This program does cmd line argument parsing for you, but YOU
   //  need to parse individual commands yourself!!!
   interpreter::shape_map shapemap; // map of all generated shapes
   struct rgbcolor {
      // ...
      explicit rgbcolor (const string&);  // prevent implicit conversion
   };
   void some_function(rgbcolor that) { /**/ }
   /*example*/ {
      string myColor = "0x00FF00"; // green
      some_function(myColor);     // COMPILE ERROR
      some_function(rgbcolor(myColor));   // fine
   }
   
   // Code example I:
   // shape is the BASE class that all shapes derive from!
   //  1st level derived: text, ellipse, and polygon
   //    circle derives from ellipse
   //    rectangle, diamond, and triangle derive from polygon.
   // shape defines the prototype of "draw()", other funcs must
   //  implement their versions of draw. Note that this function must
   //  specify two arguments: (1) center of object, and (2) color
   // NOTE: When you specify an object's center with DRAW, be sure to
   //  take an object's average point as its center, meaning: average
   //  out the points of a polygon.
   {  // some stuff on inheritance...
      class Base { int _x = 5; /**/ };
      class Derived: public base { int _y = 6; /**/ };
      Base myB;
      Derived myD;
      myB = myD;     // this is BAD, because we will be doing SPLICING!
   }
   
   // Code example J:
   // rectangle ctor:
   rectangle::rectangle(GLfloat width, GLfloat height): polygon({}) { }
   // you need to modify the polygon initializer list with the polygon's
   //  vertices
   // it would look like:
   polygon({{0, 0}, {0, width}, {height, width}, {height, 0}})
   // if you want :)
   //  You could apply this procedure to some of the other derived ctors
   
   // Code example K:
   class interpreter {
      // Interpreter class
   };
   // Factory functions: function that makes a particular object, then
   //  we dispatch to a particular function by looking up a table.
   
   //
}