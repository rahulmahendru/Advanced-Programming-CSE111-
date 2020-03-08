// $Id: cixd.cpp,v 1.8 2019-04-05 15:04:28-07 - - $

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream outlog (cout);
struct cix_exit: public exception {};

void reply_ls (accepted_socket& client_sock, cix_header& header) {
   const char* ls_cmd = "ls -l 2>&1";
   FILE* ls_pipe = popen (ls_cmd, "r");
   if (ls_pipe == NULL) { 
      header.command = cix_command::NAK;
      header.nbytes = htonl (errno);
      send_packet (client_sock, &header, sizeof header);
      return;
   }
   string ls_output;
   char buffer[0x1000];
   for (;;) {
      char* rc = fgets (buffer, sizeof buffer, ls_pipe);
      if (rc == nullptr) break;
      ls_output.append (buffer);
   }
   pclose (ls_pipe);
   header.command = cix_command::LSOUT;
   header.nbytes = htonl (ls_output.size());
   memset (header.filename, 0, FILENAME_SIZE);
   send_packet (client_sock, &header, sizeof header);
   send_packet (client_sock, ls_output.c_str(), ls_output.size());
}

void reply_get(accepted_socket& client_sock, cix_header& header){
   // Use ifstream to read the file
   ifstream get_file (header.filename, ifstream::binary);

   // Check of file does not exist
   if(!get_file) {
      header.command = cix_command::NAK;
      header.nbytes = htonl (errno);
      send_packet (client_sock, &header, sizeof header);
      return;
   }

   // Calculate file size
   get_file.seekg(0, get_file.end);
   int length = get_file.tellg();
   get_file.seekg(0, get_file.beg);

   // set the buffer and read the file
   char buffer[0x1000];
   get_file.read(buffer, length);
   get_file.close();
   header.command = cix_command::FILEOUT;
   header.nbytes = htonl (length);

   // Send the FILE packet
   memset (header.filename, 0, FILENAME_SIZE);
   send_packet (client_sock, &header, sizeof header);
   send_packet (client_sock, buffer, length);
}

void reply_put(accepted_socket& client_sock, cix_header& header){
   // set header command, buffer and nbytes
   header.command = cix_command::ACK;
   size_t host_nbytes = ntohl (header.nbytes);
   auto buffer = make_unique<char[]> (host_nbytes + 1);
   recv_packet (client_sock, buffer.get(), host_nbytes);
   buffer[host_nbytes] = '\0';

   // Use ofstream to write to a file in the server
   ofstream newFile;
   newFile.open(header.filename);
   newFile << buffer.get();
   newFile.close();
   send_packet (client_sock, &header, sizeof header);
}

void reply_rm(accepted_socket& client_sock, cix_header& header){
   ifstream get_file (header.filename, ifstream::binary);

   // Check if the file does not exist
   if(!get_file) {
      memset (header.filename, 0, FILENAME_SIZE);
      header.command = cix_command::NAK;
      send_packet (client_sock, &header, sizeof header);
      return;
   }
   // Remove the file
   else {
      // Use unlink to remove the file
      int removed_file = unlink(header.filename);
      memset (header.filename, 0, FILENAME_SIZE);

      // Check for an error removing the file
      if (removed_file == -1 ) {
         header.command = cix_command::NAK;
         }
      else {
         header.command = cix_command::ACK;
      }
      send_packet (client_sock, &header, sizeof header);
   }
}

void run_server (accepted_socket& client_sock) {
   outlog.execname (outlog.execname() + "-server");
   outlog << "connected to " << to_string (client_sock) << endl;
   try {   
      for (;;) {
         cix_header header; 
         recv_packet (client_sock, &header, sizeof header);
         switch (header.command) {
            case cix_command::LS: 
               reply_ls (client_sock, header);
               break;
            case cix_command::GET:
               reply_get (client_sock, header);
               break;
            case cix_command::PUT:
               reply_put (client_sock, header);
               break;
            case cix_command::RM:
               reply_rm (client_sock, header);
               break;
            default:
               break;
         }
      }
   }catch (socket_error& error) {
      outlog << error.what() << endl;
   }catch (cix_exit& error) {
      outlog << "caught cix_exit" << endl;
   }
   outlog << "finishing" << endl;
   throw cix_exit();
}

void fork_cixserver (server_socket& server, accepted_socket& accept) {
   pid_t pid = fork();
   if (pid == 0) { // child
      server.close();
      run_server (accept);
      throw cix_exit();
   }else {
      accept.close();
      if (pid < 0) {
         outlog << "fork failed: " << strerror (errno) << endl;
      }else {
         outlog << "forked cixserver pid " << pid << endl;
      }
   }
}


void reap_zombies() {
   for (;;) {
      int status;
      pid_t child = waitpid (-1, &status, WNOHANG);
      if (child <= 0) break;
   }
}

void signal_handler (int signal) {
   strsignal(signal);
   reap_zombies();
}

void signal_action (int signal, void (*handler) (int)) {
   struct sigaction action;
   action.sa_handler = handler;
   sigfillset (&action.sa_mask);
   action.sa_flags = 0;
   int rc = sigaction (signal, &action, nullptr);
   if (rc < 0) outlog << "sigaction " << strsignal (signal)
                      << " failed: " << strerror (errno) << endl;
}


int main (int argc, char** argv) {
   outlog.execname (basename (argv[0]));
   outlog << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   signal_action (SIGCHLD, signal_handler);
   in_port_t port = get_cix_server_port (args, 0);
   try {
      server_socket listener (port);
      for (;;) {
         outlog << to_string (hostinfo()) << " accepting port "
             << to_string (port) << endl;
         accepted_socket client_sock;
         for (;;) {
            try {
               listener.accept (client_sock);
               break;
            }catch (socket_sys_error& error) {
               switch (error.sys_errno) {
                  case EINTR:
                     outlog << "listener.accept caught "
                         << strerror (EINTR) << endl;
                     break;
                  default:
                     throw;
               }
            }
         }
         outlog << "accepted " << to_string (client_sock) << endl;
         try {
            fork_cixserver (listener, client_sock);
            reap_zombies();
         }catch (socket_error& error) {
            outlog << error.what() << endl;
         }
      }
   }catch (socket_error& error) {
      outlog << error.what() << endl;
   }catch (cix_exit& error) {
      outlog << "caught cix_exit" << endl;
   }
   outlog << "finishing" << endl;
   return 0;
}

