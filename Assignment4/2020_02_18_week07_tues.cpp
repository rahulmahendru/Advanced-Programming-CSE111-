/*
Examples/wk10a-miscellaneous/sigactionintr.cpp
   * Code Example A
Assignments/asg4-client-server/asg4-client-server.pdf
   * Code Example B
Assignments/asg4-client-server/code/logstream.h
   * Code Example C
Assignments/asg4-client-server/code/protocol.h
   * Code Example D
Assignments/asg4-client-server/code/protocol.cpp
   * Code Example E
Assignments/asg4-client-server/code/cix.cpp
   * Code Example F
Assignments/asg4-client-server/code/cixd.cpp
   * Code Example G
Assignments/asg4-client-server/code/sockets.h
Assignments/asg4-client-server/code/sockets.cpp
   * Code Example H
*/

{
   // Code example A:
   // sigint: Interrupt Signal: Produced when you press Ctrl+C in term
   //  - In general, it will kill the program, but sig can be caught
   {
      struct sigaction newaction, oldaction;
      sigemptyset(&newaction.sa_mask);
      newaction.sa_handler = action_handler;
      newaction.sa_flags = 0;
      int sigaction_rc = sigaction(SIGINT, &newaction, &oldaction);
      // Use the above to register SIGINT catcher; see file for rest
      //  of implementation
   }
   // Elsewhere:
   void action_handler (int signo) {
      // do stuff
   }
   // note that the above MUST be within an "extern "C" { /**/ }" block
   //  else, C++ will mangle the names and callstack that sa_handler
   //  expects. Though, this may not be necessary???
   getpid(); // Get Process ID of running program.
   // Whenever you start a process, the kernel obtains whatever ID it
   //  can and assigns it to your process.
   
   // Code Example B:
   /*
      * In this assignment, you will be programming two binaries:
         * "cixd": Server daemon: Waits for "cix" to connect.
            * Infinite loop program. Kill with "pkill cixd", but it will
               only kill your instance of the program.
         * "cix": Client server: Connects to server daemon.
      * "cixd" must run on the same network environment as "cix". If you
         attempt to connect to a cixd on the UNIX systems from your
         local machine that's not on the UCSC system, then you'll fail.
      * "cixd" will fork itself upon a client connection: the child will
         handle the connection between the server and the client.
      * (Please look at the PDF to figure out what all the commands are)
      * Protocol: See below:
   */
   /*
      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         Be sure to read Section 4 carefully! I will go over in section
          more carefully what the protocol looks like when drawn on the
          whiteboard.
      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         Also, be sure that your GET and PUT operations work on binary
         files!
      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   */
   
   // Code Example C:
   /* We looked at this file last Thursday. */
   
   // Code Example D:
   // PROTOCOL: C-Level structures (Assignments/asg4/code/protocol.h):
   enum class cix_command: uint8_t {
      ERROR, EXIT, // stuff ...
   }; // ": uint8_t" forces cix_command to be 1 byte in size.
   size_t constexpr FILENAME_SIZE = 59; // yes, fnames can be longer...
   struct cix_header { // MUST KNOW packet size AT COMPILE TIME!
      uint32_t nbytes {0}; // this MUST be stored in big-endian form!!!
      cix_command command {cix_command::ERROR};
      char filename[FILENAME_SIZE] {}; // must be null-terminated
   }; // "filename" must NOT have any slashes in it!!!
   static_assert(sizeof(cix_header) == 64); // this is a COMPILE check
         // static_assert() requires args to be known at compile time.
         // if static_assert() fails, then program will fail to compile
   // see PDF for info for cix_command values
   // <<< Why use enum class, instead of enum? >>>
   //   REASON: Avoid polluting global enum namespace
   send_packet(/**/); recv_packet(/**/); // functions you'll use at top
         // level within cix.cpp and cixd.cpp
   
   // Code Example E:
   to_string(cix_command command) { /**/ } // gets English name of cmd
   void some_func(const void *somePtr) { /**/ }
      // somePtr is a pointer to some arbitrary data, however, we still
      //  have to cast it to a const char* ptr
   // send, recv:
   //    * ret = 0: connection closed
   //    * ret < 0: Error
   //    * ret > 0: # bytes transmitted successfully, # = ret.
   //       * Must ensure ALL bytes transfer over, use do {...} while();
   
   // Code Example F:
   // six commands, yet only 3 commands are currently implemented.
   //  You'll have to add the other (non-implemented) commands to the
   //  command hash at the start of the file.
   // --- Multi-line raw C string (char array):
   static const char help[] = R"||(
      some stuff
      hi world
   )||"; // the double bar will indicate the start and stop of string
         //  exclusive. They CANNOT and SHOULD NOT appear in the string.
   // Search for the following in cix.cpp:
   switch (cmd) {
      // stuff
      //  You'll have to add cases here to complete the assignment...
   }
   // use "cix_ls(...)" as a reference to how to impl the other cmds
   //  Remember, "header.nbytes" may mean different things depending on
   //  which command you are implementing!!
   /* code snipper from cix_lx(...) */ {
      auto buffer = make_unique<char[]> (header.nbytes + 1);
      const char *rawPtr = buffer.get();
      // stuff ...
      // other stuff...
   } // here, buffer will go out-of-scope, freeing the buffer
   /*stuff = */ command_map.find(line); // THIS IS INCOMPLETE!!!
   // You need to parse "line" for arguments!!!
   // For instance, if I input "get someFile.txt", then even if I have
   //  "get" in the cmd map hash, find(line) will fail. Split!!!
   
   // Code Example G:
   void somefunc() {
      pid_t pid = fork();
      if (pid == 0) {
         // This is the parent process.
      } else {
         // This is the child process.
      }
   }
   void reply_ls(/**/) { /**/ } // use this function as a basis for how
      // to write functions for cixd...
   // Note: You won't be using popen() on the other cixd/cix functions
   /*    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      Be careful, there are zombies!!! 0_0 Don't worry, we can reap them
         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      * What is a Zombie? A zombie is a child process that exits before
         the parent does, but the parent hasn't "reaped" it yet. Thus,
         it remains in the process table until the parent removes it
            (removing a zombie process is known as "reaping")
      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         This is already done for you :)
   */
   // Makefile stuff: Like colorful compile outputs? Delete the
   //  following portion from your makefile:
   //       "-fdiagnostics-color=never"
   
   // Code Example H:
   // Look at simple sockets example from last Thursday
   // socket_sys_error: use to throw/catch socket errors
   
   //
}