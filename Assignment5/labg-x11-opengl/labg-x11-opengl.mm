.so Tmac.mm-etc
.if t .Newcentury-fonts
.INITR* \n[.F]
.SIZE 12 14
.TITLE CSE-111 Fall\~2019 Lab\~GL "X11 and OpenGL"
.RCS "$Id: labg-x11-opengl.mm,v 1.265 2019-10-29 17:02:15-07 - - $"
.PWD
.URL
.nr HISTORY 0 1
.de PROMPT
.   TVCODE* 1 "bash-\\n+[HISTORY]\[Do] " "\\$*"
..
This is a
.E= non-credit
lab, in preparation for an upcoming project which
will involve using OpenGL,
which is a graphics toolkit.
There is nothing to submit,
and it will not be graded.
In order to run an X11 application on the server,
and have it display on your own workstation,
you will need to install an X11 running on your computer.
.H 1 "Unix Timeshare\(:) X2Go"
X2Go allows you to access a remote graphical
desktop over a low bandwidth connection.
In otherwords, you can use X2Go to login to the
Unix timeshare in a graphical fashion instead of using a command line.
It is similar to the X11 (or XWindows)
system that use XDMCP (or SSH) to view a remote desktop.
X2Go is availble on multiple platforms, so simply pick
the client that works best for you.
More information can be found on the X2Go website\(::
.V= https://wiki.x2go.org/
.P
Setting Up X2Go\(::
.br
.V= http://its.ucsc.edu/unix-timeshare/tutorials/setting-up-x2go.html
.H 1 "Installing X11"
Depending on what personal computer you use,
choose one of the following methods of installing X11.
.ALX a ()
.LI
.BR "X11 on Linux."
X11 is the native windowing system used on Linux.
You may use any window manager of your choice.
.LI
.BR "X11 on OSX."
The Macintosh OSX operating system is based on BSD
(the Berkeley Systems Distribution of Unix).
Install XQuartz if you do not already have X11 running on your
personal workstation.
.br
.VTCODE* 0 "https://www.xquartz.org/"
.LI
.BR "X11 on W*nd*z*."
There are some choices\(::
Xming.
Cygwin.
.br
.VTCODE* 0 "http://www.straightrunning.com/XmingNotes/"
.VTCODE* 0 "http://www.geo.mtu.edu/geoschem/docs/putty_install.html"
.VTCODE* 0 "https://www.cygwin.com/"
.LE
.H 1 "Verifying X11"
Once X11 has been installed on your computer,
verify that it is working.
When running an X11 program repomely but displaying on the local
workstation, 
it is necessary to set up X11 forwarding with the
.V= -X
flag of
.V= ssh ,
as in\(::
.PROMPT ssh -X \fIusername\fP@unix.ucsc.edu
.P
One may run an X11 program by typing the command name,
usually by backgrounding the process,
as in the following.
Note the ampersand
.=V ( & ).
.DS
.PROMPT gv &
.PROMPT xev &
.PROMPT xpdf &
.DE
.H 1 "Running OpenGL programs"
In the subdirectory
.V= hello-world
there are several OpenGL programs available for testing.
If your workstation has X11 properly installed,
they can just be run as any other X11 program.
Example\(::
.PROMPT glxgears &
.P
.E= Problems\(??
Because of incorrect configuration of OpenGL on the server,
it is possible that you might see the following message\(::
.DS
.PROMPT glxgears &
.TVCODE* 1 "libGL error: No matching fbConfigs or visuals found"
.TVCODE* 1 "libGL error: failed to load driver: swrast"
.TVCODE* 1 "Error: couldn't get an RGB, Double-buffered visual"
.DE
The following statement in your
.V= \&.bash_profile
might solve that problem\(::
.DS
.VTCODE* 1 \
 "export GLES_MESA=/usr/local/android-sdk/emulator/lib64/gles_mesa
.VTCODE* 1 "export\0LD_LIBRARY_PATH=\[Do]LD_LIBRARY_PATH:\[Do]GLES_MESA
.DE
.H 1 "Hello, World!"
The subdirectory
.V= hello-world/
contains some sample programs.
You are not expected to understand the code in these programs
at this time,
but you should run the programs to verify that you can.
at the end of the line.
Three of the programs draw flags on the screen.
You can move them around and resize them.
.DS
.PROMPT bonjour-le-monde &
.PROMPT ciao-mondo &
.PROMPT hallo-welt &
.PROMPT konnichiwa-sekai &
.DE
.P
The program
.V= hello-world
can also be moved around and resized,
and it also reacts to inputs you give it.
.DS
.PROMPT hello-world &
.TVCODE* 1 "To cycle the colors: right mouse button or key 'c' or 'C'."
.TVCODE* 1 "To cycle the fonts: left mouse button or key 'f' or 'F'."
.TVCODE* 1 \
"To cycle the greetings: middle mouse button or key 'g' or 'G'."
.TVCODE* 1 "To invert the colors: key Backspace, Return, or Delete."
.TVCODE* 1 "To quit: key 'q' or 'Q' or ESC."
.DE
.if dPSPIC \{
.DS 
.PSPIC hello-world/hello-world.xwd.ps 400p
.V= \0\0\0\0\0hello-world/hello-world.xwd.ps
.DE
.H 1 "Other tests"
For other tests of OpenGL,
look in the directory
.V= other-tests
and run\(::
.P
.DS
.PROMPT glclock &
.PROMPT color-wheel &
.PROMPT teapot &
.DE
The last one
shows a teapot picture both in a solid view and a wire frame.
Every time you press the left mouse button the view get swapped.
.H 1 "Installing a local OpenGL"
Running OpenGL across the internet will be net-intensive,
especially if you have a slow internet connection.
So you might want to install OpenGL on your own workstation.
.VTCODE* 1 "http://freeglut.sourceforge.net/"
.P
Once you have installed OpenGl on your own workstation,
you may copy the files from this 
.V= code
subdirectory onto your own workstation and compile them.
Depending on the OS you are using,
you may have to change the line
.VTCODE* 1 "#include <GL/freeglut.h>"
to something else, for example\(::
.DS
.VTCODE* 1 "#ifdef __linux__"
.VTCODE* 1 "#include <GL/freeglut.h>"
.VTCODE* 1 "#endif"
.VTCODE* 1 "#ifdef __macosx__"
.VTCODE* 1 "#include <\f[I]??????????\fP>"
.VTCODE* 1 "#endif"
.DE
.P
The
.V= #ifdef s
detect which operation system you are using and
will then include the appropriate header files,
thus possibly leading to a portable program.
You need to figure out what the predefined macro identifying your
OS, and where the OpenGL header files are.
.H 1 "Other references"
When looking for documentation on a particular function,
look at
.V= https://www.opengl.org/ .
For example,
to find out about the function
.V= glutPostRedisplay ,
type
.=V `` "OpenGL glutPostRedisplay" ''
into Google, then select
.V= www.opengl.org .
.P
.ne 5
Getting Started.
.VTCODE* 1 "https://www.opengl.org/wiki/Getting_Started"
.P
OpenGL Programming Guide.
.VTCODE* 1 "http://www.glprogramming.com/red/"
.P
OpenGL Reference Manual.
.VTCODE* 1 "http://www.glprogramming.com/blue/"
.P
GLUT Tutorial.
GLUT stands for the OpenGL Utility Toolkit.
.VTCODE* 1 "http://www.lighthouse3d.com/tutorials/glut-tutorial/"
.P
Compiling OpenGL programs on Windows, Linux and OS X.
.VTCODE* 1 "http://myweb.wit.edu/wernerm/compilingOpenGl.html"
.H 1 "Alternatives"
Instead of installing X11 on your workstation,
you may do your development in the lab.
Instead of installing OpenGL on your workstation,
you may work remotely and run all OpenGL programs on the timeshares.
.bp
.H 1 "Some more examples"
.DS
.TS
tab(|); lw(225p) lw(225p).
T{
.PSPIC hello-world/bonjour-le-monde.xwd.ps 220p
.V= hello-world/bonjour-le-monde.xwd.ps
T}|T{
.PSPIC hello-world/ciao-mondo.xwd.ps 220p
.V= hello-world/ciao-mondo.xwd.ps
T}
.TE
.DE
.DS
.TS
tab(|); lw(225p) lw(225p).
T{
.PSPIC hello-world/hallo-welt.xwd.ps 220p
.V= hello-world/hallo-welt.xwd.ps
T}|T{
.PSPIC hello-world/konnichiwa-sekai.xwd.ps 220p
.V= hello-world/konnichiwa-sekai.xwd.ps
T}
.TE
.DE
.\}
