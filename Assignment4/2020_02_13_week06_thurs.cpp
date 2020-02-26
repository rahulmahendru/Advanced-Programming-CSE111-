/*
If there are any mistakes, or things I should add, please let Clark
 Hilbert know!

Lecture-notes/sockets-networking.pdf
   * Code example A
Examples/sockets/small-examples/telnet-www2.ucsc.edu-80
   * Code ex B
Examples/sockets/small-examples/endian.listing
   * Code ex C
Examples/sockets/small-examples/dateserver.listing
Examples/sockets/small-examples/dateserver.cpp
   * Code ex D
Examples/sockets/small-examples/addrinfo.cpp
   * Code ex E
Examples/sockets/small-examples/httpget.c
   * Code ex F
Examples/sockets/small-examples/inettime.cpp
   * Code ex G
Examples/sockets/small-examples/socket.h
Examples/sockets/small-examples/socket.cpp
   * Code ex H
Examples/sockets/small-examples/logstream.h
   * Code ex I
Examples/sockets/small-examples/socket.cpp
   * Code ex J
Examples/wk08a-sockets/childwait.cpp
   * Code ex K
*/

{
   // Code Example A:
   // Look at page 6 onwards in linked PDF
   //  (There will be FINAL questions asking about network programming)
   // server socket functions:
   socket(...);
   bind(...);
   listen(...); // set up queue for clients to connect to
   accept(...); // BLOCKING: when a client tries to connect, establish
                // connection
   // client socket functions:
   socket(...);
   connect(...);
   // message passing:
   send(...);
   recv(...);
   
   // Code Example B:
   /*
   bash-3$ telnet www2.ucsc.edu 80
      GET /
   */
   
   // Code Example C:
   // htonl and ntohl: Convert 32-bit number between host and network
   //  byte order. On sparc machine, these functions do nothing. But on
   //  x86 machines (such as literally any mainstream computer), they
   //  will reverse the byte order.
   
   // Code Example D:
   // Note: if you attempt to run "./dateserver <Port>", and someone
   //  is already using <Port>, then the bind() operation will fail.
   // "kill" and "pkill": specify pid.
   // "kill -9 -1": Logs you out of your machine :)
   // At runtime, it's possible to print out the name of a type by
   //  including <typeinfo>
   int nbytes = server.recv(buffer);
   // which, in turn, calls the C-std function: "recv(..., buffer)"
   // recv return type:
   // nbytes < 0: Error
   // nbytes == 0: End of file (note that recv hangs)
   // nbytes > 0: # bytes received
   time_t now = time(nullptr); // UTC time:
   // Number of seconds from Jan 1st, 1970 00:00 UTC
   // Using a 32-bit signed number: runs out in 2038.
   // But, we use a 64-bit signed number now. So we're good! :)
   int nbytes = client.send(bufp);
   // which, in turn, calls C-std "send(...)"
   // if data is too large for a single packet, send() will break it up
   // recv() and send() are blocking calls!!!
   
   // Code Example E:
   int rc = getaddrinfo(hostname.c_str(), ...);// this is a C function
   
   // Code Example F:
   // Review this file to see what to write to create a client program.
   // In C++ (compared to C), you do not have to write out "struct foo"
   //  to name the type "struct foo"; you could jsut type "foo":
   struct foo myFoo;
   // is the same as:
   foo myFoo; // BUT NOT POSSIBLE IN REGULAR C
   // But, this file is in C, so we have to use the struct keyword
   
   // Code Example G:
   // UNIX Epoch starts at Jan1 1970, but Inet epoch starts at 1900.
   
   // Code Example H:
   // Good idea to create wrapper for C socket functions in C++:
   // Whenever you start a process, you get a random process ID from the
   //  OS, which you will then have to deal with.
   logstream clog (cout, basename(argv[0])); // special output logging
   // When you catch an exception, you should catch it by reference, NOT
   //  by value!!!
   // The server has two sockets:
   //  *** The accept socket: the socket which clients connect to
   //  *** The socket allocated by the OS to the server to recv/send
   //       msgs between the server and client
   //       (Note: there could be many of this type).
   // The client has only one socket type:
   //  *** The client has a single socket that's used to connect, recv, 
   //       and send msgs too and from the server.
   // classes wrapped around sockets, much like fstreams, should not
   //  be copied; they must be passed around by reference!
   /*
      Exception types:
         * logic_error (bad logic within your code)
         * runtime_error (bad input from user)
   */
   
   // Code example I:
   // Implementation of logstream, as used above. Here, we define a
   //  member function that is operator<<.
   // Note: if we have an ostream as a member, it MUST be a reference!
   
   // Code example J:
   // NOTE: If there is a member function with the same name and
   //  signature as a function within the global domain, the member
   //  function will take precedence.
   void fair() { cout << "world" << endl; }
   class something {
      void fair() { cout << "hi" << endl; }
      void enough() { ::fair(); } // calls global function
      void son() { fair(); } // calls something::fair()
   }
   
   // Code example K:
   exit(); // your process INSTANTLY stops; there may be lingering state
           //  that should be caught and handled by the destructors.
           // Thus, DON'T DO IT!
   int someInt = ...;
   exit(someInt); // 0 through 255 are valid values. 0 indicates success
   // Other ways a proram can exit:
   // A signal crash: segfault, div-by-0, assert, etc.
   // The upper 7 bits indicate what signal was flagged, and why.
   // A single bit refers to whether the program dumped core or not.
   timeval now;
   size_t sec = now.tv_sec;
   size_t usec = now.tv_usec;
   // fork() system call: duplicates process.
   /*
      Two processes now, instaed of just one:
         * The parent process
         * The child process
      The parent and children can determine themselves based on the
       return value given by fork().
   */
}