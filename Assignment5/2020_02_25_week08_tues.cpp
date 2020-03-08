/*
Assignments/labg-x11-opengl
   * CodeEx A
Assignments/labg-x11-opengl/other-tests/glversion.cpp
   * CodeEx B
Assignments/labg-x11-opengl/other-tests/teapot.cpp
   * CodeEx C
Assignments/labg-x11-opengl/other-tests/bonjour-monde.cpp
   * CodeEx D
Assignments/labg-x11-opengl/other-tests/konnichiwa-sekai.cpp
   * CodeEx E
Assignments/labg-x11-opengl/other-tests/hello-gl.cpp
-> mitadmissions.org/wp-content/uploads/2012/08/1265121755-Color31.jpeg
   * CodeEx F
Assignments/labg-x11-opengl/other-tests/???.cpp
   * CodeEx G
Assignments/labg-x11-opengl/other-tests/ellipses.cpp
   * CodeEx H
Example/opengl-examples/squares.cpp
Example/opengl-examples/color-wheel.cpp
   * CodeEx I
..
*/

{
   // Code example A:
   /* === OpenGL: ===
      * C-level functions and function calls only; 
   ====== Lab G ===
      * Ungraded lab, but do this lab to prepare for asg 5.
      * If working on your own machine, you have a variety of options to
         test and run the lab. Though, AS THE TA, I have the following
         things to tell you all:
         + When in the BE 105, please log into the machine and work
            directly off of it.
         + If working on your own machine, note that there's a
            significant amount of lag. X2Go would have the least effects
            of latency.
         + I would also suggest that you install an Ubuntu VM and test
            on that. That would eliminate a lot of issues.
      * Test out the "hello-world" program!
   */
   
   void display() {
      // Display callback function
      glClear(GL_COLOR_BUFFER_BIT); // clears OLD display
      /* Do some drawing here... */
      glColor3ubv(some_color);   // is a triplet of bytes
      glBegin(GL_POLYGON);
      /* Select for coordinates, then OpenGL will draw a polygon around
          those N coordinates, inclusive. Do not give the starting
          coordinate at the end. 
               . . .
          # Vertices = # edges */
      glVertex2f(vertex1.x, vertex1.y);
      glVertex2f(vertex2.x, vertex2.y);
      /* ... */
      glEnd();
      glFlush();  // flush display to screen
   }
   example_main() {
      // Code example B:
      glutInit(&argc, argv);     // initializes GL
      glutCreateWindow(...);     // creates GL window
      glutDisplayFunc(display);  // Set up the callback routine for
                                 //  display
      glutMainLoop();            // transfers control to GL window
                  // NEVER LEAVES until window is closed!
      return 0;
   }
   
   // Code example C:
   // OpenGL: Pixel Coordinates start in TOP-LEFT!
   //    ===> Compared to cartesian coords, which are BOTTOM-LEFT.
   /*=== Other GL Functions: ===*/
   glutInitWindowSize(500, 500);    // Sets size
   glutInitWindowPosition(0, 0);    // Sets initial position
   glutInitDisplayMode(GLUT_RGB);   // Sets display mode for new windows
   glutMouseFunc(mouse);
   // How to force program to redraw screen: sets dirty bit on window
   glutPostRedisplay(); // will call display() when glut loop polls bit
   
   // Code example D:
   const GLubyte BLACK[] {0, 0, 0};
   const GLubyte RED[] = {255, 0, 0};
   const GLubyte GREEN[] = {0, 255, 0};
   const GLubyte BLUE[] = {0, 0, 255};
   //
   const GLubyte YELLOW[] = {255, 255, 0};
   const GLubyte MAGENTA[] = {255, 0, 255};
   const GLubyte CYAN[] = {0, 255, 255};
   //
   const GLubyte WHITE[] = {255, 255, 255};
   /* You WILL have to memorize the above for the final, but you
      don't have to memorize the European flag's colors. =) */
   glutReshapeFunc(reshape);  // Reshape function callback
      // WHENEVER the user resizes their window, reshape() gets called
   void reshape(int width, int height) {
      // Do stuff with width and height to match new window size.
      glViewport(0, 0, width, height); // does stuff I don't know...
   }
   
   // Code example E:
   /* In OpenGL, you don't draw circles. You draw equilateral polygons
      with dozens of points!!! */
   { 
      /*
         * radius: Desired radius of our circle
         * xoffset, yoffset: Desired center of our circle
         * delta: Desired resolution of the circle:
            smaller the delta -> The more "refined" the circle shows up
            BUT: Diminishing returns...
      */
      // #include <cmath>
      for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
         GLfloat xpos = radius * cos(theta) + xoffset;
         GLfloat ypos = radius * sin(theta) + yoffset;
         // Think in terms of RADIANS, not degrees.
         // Don't forget your trigonometry! Pythagorean theorem!!!
         //  This may show up on the final.
         glVertex2f(xpos, ypos);
      }
   }
   
   // Code example F:
   /* creating text is more complicated that creating images... */
   void display() {
      // set up input into draw string function:
      static const string hello {"Hello, world!"};
      static auto u_str =
         reinterpret_cast<const GLubyte*>(hello.c_str());
      void *font = GLUT_BITMAP_HELVETICA_18; // choose a bitmap font.
      /* ==NOTE:== monospace vs non-monospace; serif vs. sans serif */
      // monospace: Looking at code
      // non-mono: Different letters have different widths
      // Serif: "Little hooks" at the ends of the letters
      // Sans serif: Letters that lack little hooks (e.g. Comic Sans)
      //
      /* width dependent on string and font, height only depn on font */
      int str_width = glutBitmapLength(font, u_str);
      int str_height = glutBitmapHeight(font);
      // "baseline": bottom of text string, excluding portion that juts
      //  out underneath letters such as y, g, p, q, etc.
      // LOOK AT rest of file to figure out how to create string!
   }
   
   // Code example G:
   // NOTE: OpenGL can detect when you move your cursor inside and
   //  outside the window. It can then do various things depending on
   //  whether your cursor is inside or outside the circle.
   // GL_LINE_LOOP and GL_POLYGON do the same thing, except the latter
   //  will fill the inside of the area enclosed by the line loop.
   // Mackey believs that the # of points in a circle should be a
   //  multiple of 4 to at least show the maximum/minimum coords of the
   //  circle.
   
   /// Code example H:
   // Any color where R = G = B will be a shade of grey.
   //
   
   // Code example I:
   // We need to start with LARGER square first. The things drawn last
   //  are drawn OVER the items drawn earlier.
   
   // === MISC ===
   // https://i.imgflip.com/32uvf4.jpg
   // First image: Random CSE 111 student
   // Second image: me (Clark Hilbert)
   // 3rd image: CSE 111 Discord
}