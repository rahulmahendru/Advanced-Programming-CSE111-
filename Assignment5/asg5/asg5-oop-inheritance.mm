.so Tmac.mm-etc
.if t .Newcentury-fonts
.INITR* \n[.F]
.SIZE 12 14
.TITLE CSE-111 Winter\~2020 Program\~5 "OOP, Inheritance, OpenGL"
.RCS "$Id: asg5-oop-inheritance.mm,v 1.27 2020-02-19 14:49:10-08 - - $"
.PWD
.URL
.EQ
delim $$
.EN
.de COMMAND 
.   ds commandstr \f[CB]\\$1
.   while \\n[.$]>1 \{ .\"
.      shift   
.      if '\\$1'?' .as commandstr \0\fR[\fI\\$2\fR]\f[CB]
.      if '\\$1'I' .as commandstr \0\fI\\$2\f[CB]
.      if '\\$1'R' .as commandstr \0\fR\\$2\f[CB]
.      if '\\$1'V' .as commandstr \0\\$2
.      shift   
.   \}
.   LI "\\*[commandstr]\f[R]"
..
.H 1 "Overview"
In this assignment you will implement a simple graphics package
using the OpenGL graphics library.
Using inheritance, 
geometric objects will be displayed in a window.
The lab workstations all run X11 so there is no problem there.
When you log into the server remotely, be sure to specify the
.V= -X
option, as in\(::
.VTCODE* 1 "ssh -X \fIusername\fP@unix.ucsc.edu"
Before you log into the server make sure you have an X11 client
running on your own workstation.
.H 1 "Program Specification"
The program is presented in the form of a Unix 
.V= man (1)
page.
.SH=BVL
.MANPAGE=LI "NAME"
gdraw
\[em]
drawing program displaying objects in a window
.MANPAGE=LI "SYNOPSIS"
.V= gdraw
.RI \|[
.IR options ]
.RI [ filename ]
.MANPAGE=LI "OPTIONS"
All options are recognized by
.V= getopt (3).
.VL \n[Pi]
.MANOPT=LI -@ flags
The flags specified are passed to the debugging macro.
Debug output is printed to the standard error.
.MANOPT=LI -w width
The initial width of the window.
Default\(::
640.
.MANOPT=LI -h height
The initial height of the window.
Default\(::
480.
.LE
.MANPAGE=LI "OPERANDS"
Commands are read from the file whose name is given as an operand.
If no filename is given, commands are read from the standard input.
.MANPAGE=LI "COMMANDS"
When the program begins, a window is created for displaying
information, which consists of text and geometric objects of
various kinds.
The project proceeds in two parts\(::
.ALX a ()
.LI
Shapes are defined in terms of their sizes (length, width,
or vertices).
.LI
Objects are drawn in the window by drawing commands,
after which they may be altered by keyboard commands.
.LE
.P
Sizes, lengths, widths, and co\[:o]rdinates are all
floating point numbers, and measure pixels.
.P
Colors may be specified by name using one of the names in
the file
.V= rgb.txt ,
or by their hexadecimal values in the form
.VI 0xRRGGBB ,
where
.V= RR ,
.V= GG ,
and
.V= BB
are the red, green, and blue components each specified as
two hexadecimal digits in the range
.V= 00
to
.V= FF .
.P
The
.V= define
command makes a record of the definition of an shape and makes
it available later for a draw command.
The
.V= draw
command
creates an object in the screen's object list,
to be displayed when the program starts.
.P
The following commands are recognized.
Each command is on a line by itself and terminates with the
newline character.
If the last character on a line is a backslash
.=V ( \[rs] ),
the command is continued onto the next line.
.BVL \n[Pi]
.COMMAND # R \|.\|.\|.
A comment line is ignored,
as are empty lines and lines consisting only of white space.
.COMMAND define I name V text I font I words\|.\|.\|.
A text object is created by concatenating all of the words together,
each separated from the next by spaces.
The font is any of the seven GLUT bitmap fonts\(::
.ALX \[bu] 0 "" 0 0
.LI
.V= Fixed-8x13
.LI
.V= Fixed-9x15
.LI
.V= Helvetica-10
.LI
.V= Helvetica-12
.LI
.V= Helvetica-18
.LI
.V= Times-Roman-10
.LI
.V= Times-Roman-24
.LE
.COMMAND define I name V ellipse I width I height
An ellipse is specified with the major and minor diameters specified.
When drawn, the drawing co\[:o]rdinate is the center.
.COMMAND define I name V circle I diameter
A circle is just an ellipse whose length and width are equal.
.COMMAND define I name V polygon I x0 I y0 I x1 I y1 \
I x2 I y2 R \|.\|.\|.\|
A polygon is specified with the number of vertices being equal to
the number of $ ( x , y ) $ co\[:o]rdinates given.
The polygon must be convex (each interior angle less than 180 degrees).
The convexity is not verified by the program,
so the appearance of a non-convex polygon is undefined.
The center of the polygon is at $ ( x bar , y bar ) $, i.e.,
at the average of all the $x$ co\[:o]rdinates and the average of
the $y$ co\[:o]rdinates.
After computing the average,
it is subtracted from each of the vertices to normalize them.
For example, if the vertices are
specified as shown in the left side of the diagram,
they are normalized to that shown on the right side,
so that $ ( x bar , y bar ) $ changes from $ ( .5 , .5 ) $ to
$ ( 0 , 0 ) $.
.nr Si 2*\n[Pi]
.DS I
.TS
tab(|); c cw(1i) c.
T{
.PS 1i
"$ ( 0 , 0 ) $" at 0,-.1
"$ ( 1 , 0 ) $" at 1,-.1
"$ ( 1 , 1 ) $" at 1,1.1
"$ ( 0 , 1 ) $" at 0,1.1
line from 0,0 to 1,0 then to 1,1 then to 0,1 then to 0,0
"$ ( .5 , .5 ) $" at .5,.5
.PE
T}||T{
.PS 1i
"$ ( -.5 , -.5 $)" at 0,-.1
"$ ( +.5 , -.5 $)" at 1,-.1
"$ ( +.5 , +.5 $)" at 1,1.1
"$ ( -.5 , +.5 $)" at 0,1.1
line from 0,0 to 1,0 then to 1,1 then to 0,1 then to 0,0
"$ ( 0 , 0 ) $" at .5,.5
.PE
T}
.TE
.DE
.COMMAND define I name V rectangle I width I height
A rectangle with the given width and height is defined with
strictly horizontal and vertical lines.
When drawn, the drawing co\[:o]rdinates will be the center.
.COMMAND define I name V square I width
A square is a rectangle with equal width and height.
.COMMAND define I name V diamond I width I height
A diamond's width is from the leftmost to rightmost point and its
height is from topmost to bottommost height.
The draw command specifies its center.
.COMMAND define I name V triangle I x0 I y0 I x1 I y1 I x2 I y2
A triangle is just another name for a polygon.
.COMMAND define I name V equilateral I width
An equilateral triangle has a horizontal base (two points have
the same $y$-value).
The apex has a higher $y$-value.
The constructor just uses the constructor for a triangle.
.COMMAND border I color I thickness
Specifies the thickness in pixels of the border surrounding the selected
object, and its color.
Defaults\(::
red, 4.
.COMMAND moveby I pixels
The move parameter indicates how many pixels an object moves when
directed by one keystroke.
Default\(::
4.
.COMMAND draw I color I name I xcenter I ycenter
A geometrical object (an ellipse or a polygon)
is drawn with the center at the co\[:o]rdinates
$ ( x , y ) $, using the color specified.
A text object is drawn using $ ( x , y ) $ as the co\[:o]rdinate
of the left end of the string at the baseline.
.LE
.MANPAGE=LI "KEYBOARD INPUT"
Once the input file is finished,
the keyboard is used to command the movements of the objects.
The following keystrokes control the program\(::
.nr Si \n[Pi]
.DS I
.TS
tab(|); lfCB l.
q|Quit (exit the program).
h|Move the selected object left.
j|Move the selected object down.
k|Move the selected object up.
l|Move the selected object right.
n|Set the selection to the next object.
p|Set the selection to the previous object.
0\fR ... \f[CB]9|Set the selection to objects 0 through 9, respectively.
.TE
.DE
An object is always displayed using the co\[:o]rdinates of the center.
If an object is moved off the window, it appears at the other
end of the window.
That is, if the $x$ or $y$ co\[:o]rdinate exceeds the width (height)
of the window, it is set to 0.
If it becomes negative, it is set to the width (height).
.P
The selected object is always surrounded by a border to make
it more visible,
and is always displayed on top of all of the others.
Other objects are displayed in the order that they appear 
in the object vector.
.P
For objects 0 through 9,
except for text objects,
the object's number is displayed in the center of the object.
A text object might disappear if its left end is out of the window.
(A bug in OpenGL?)
.P
.br
.ne 3
.MANPAGE=LI "EXIT STATUS"
.VL \n[Pi]
.LI 0
No errors were detected.
.LI 1
Errors were detected and messages printed to the standard error.
.LE
.LE
.H 1 "Code"
OpenGL and GLUT will be used to display the window.
See the short example programs in the
following subdirectories\(::
.VTCODE* 1 Assignments/labg-x11-opengl/hello-world/
.VTCODE* 1 Assignments/labg-x11-opengl/other-tests/
.VTCODE* 1 Examples/opengl-examples/
Starter code is in the
.V= code/
subdirectory.
For documentation, Google using the terms ``OpenGL'' and/or ``GLUT''
and the name of the function, 
then look for results naming the web pages in
.VTCODE* 1 www.opengl.org/documentation
Following are some functions you might need\(::
.DS
.TS
tab(|); lfCB lfCB lfCB.
T{
glBegin
glClear
glClearColor
glColor3ub
glColor3ubv
glEnable
glEnd
glFlush
glHint
glLineWidth
glLoadIdentity
glMatrixMode
glOrtho
glPointSize
glPopMatrix
glPushMatrix
glRasterPos2f
glRasterPos2i
T}|T{
glRotatef
glTranslatef
glVertex2f
glViewport
gluOrtho2D
glutAddMenuEntry
glutAddSubMenu
glutAttachMenu
glutBitmapCharacter
glutBitmapHeight
glutBitmapLength
glutBitmapString
glutCloseFunc
glutCreateMenu
glutCreateWindow
glutDisplayFunc
glutEntryFunc
glutInit
T}|T{
glutInitDisplayMode
glutInitWindowPosition
glutInitWindowSize
glutKeyboardFunc
glutKeyboardUpFunc
glutMainLoop
glutMotionFunc
glutMouseFunc
glutPassiveMotionFunc
glutPostRedisplay
glutReshapeFunc
glutSetIconTitle
glutSetWindowTitle
glutSpecialFunc
glutSpecialUpFunc
glutSwapBuffers
glutTimerFunc
glutWireTeapot
T}
.TE
.DE
.FINISH
