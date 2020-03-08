// $Id: cix.cpp,v 1.9 2019-04-05 15:04:28-07 - - $

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream outlog (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"get" , cix_command::GET },
   {"put" , cix_command::PUT },
   {"rm"  , cix_command::RM  },
};

static const char help[] = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cix_help() {
   cout << help;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   if (header.command != cix_command::LSOUT) {
      errno = EFAULT;
      outlog << "cix: ls failed: " << strerror(errno) << endl;
   }
   else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   }
}

void cix_get(client_socket& server, string& file) {
   cix_header header;
   header.command = cix_command::GET;

   // Copy the filename
   strcpy(header.filename, file.c_str());

   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);

   // Check if reply_get does not return cix_command::FILEOUT
   if(header.command != cix_command::FILEOUT) {
      errno = ENOENT;
      outlog << "cix: get failed for " 
             << file << ": "
             << strerror(errno)
             << endl;
   }
   else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      buffer[host_nbytes] = '\0';

      // Use ofstream to output file
      ofstream newFile;
      newFile.open(file);
      newFile << buffer.get();
      newFile.close();

      // Success message
      outlog << "cix: get successful for "
             << file 
             << endl;
   }
}

void cix_put(client_socket& server, string& file) {
   cix_header header;
   header.command = cix_command::PUT;
   strcpy(header.filename, file.c_str());

   // Use ifstream to get the file in the client
   ifstream get_file (header.filename, ifstream::binary);

   // Check if file does not exist
   if(!get_file) {
      header.command = cix_command::NAK;
      errno = ENOENT;
      header.nbytes = htonl (errno);
      send_packet (server, &header, sizeof header);
   }
   else {
      // Get file size
      get_file.seekg(0, get_file.end);
      int length = get_file.tellg();
      get_file.seekg(0, get_file.beg);

      // Set payload to size of file
      header.nbytes = htonl (length);
      char buffer[0x1000];

      // Read the contents of the file
      get_file.read(buffer, length);
      get_file.close();

      // Send the FILE packet
      send_packet (server, &header, sizeof header);
      send_packet (server, buffer, length);
      recv_packet (server, &header, sizeof header);
      memset (header.filename, 0, FILENAME_SIZE);
   }
   
   // Error message if file does not exist
   if(header.command != cix_command::ACK) {
      errno = ENOENT;
      outlog << "cix: put failed for " 
             << file << ": " 
             << strerror (errno) 
             << endl;
   }
   // Success message
   else {
      outlog << "cix: put successful for " 
             << file
             << endl;
   }
}

void cix_rm(client_socket& server, string& file) {
   cix_header header;
   header.command = cix_command::RM;
   strcpy(header.filename, file.c_str());

   // Set payload to 0
   header.nbytes = 0 ;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);

   // Error message if remove fails
   if(header.command != cix_command::ACK) {
      errno = ENOENT;
      outlog << "cix: rm failed for " 
             << file << ": "
             << strerror(errno)
             << endl;
   }
   // Success Message
   else {
      outlog << "cix: rm succesful for " 
             << file 
             << endl;
   }
}

void usage() {
   cerr << "Usage: " << outlog.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   outlog.execname (basename (argv[0]));
   outlog << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   outlog << to_string (hostinfo()) << endl;
   try {
      outlog << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      outlog << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         string args_[] = {line.substr(0,line.find_first_of(" ")), 
                           line.substr(line.find_first_of(" ")+1, 
                           line.size())};
         outlog << "command " << line << endl;
         const auto& itor = command_map.find (args_[0]);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               break;
            case cix_command::LS:
               cix_ls (server);
               break;
            case cix_command::GET:
               cix_get (server, args_[1]);
               break;
            case cix_command::PUT:
               cix_put (server, args_[1]);
               break;
            case cix_command::RM:
               cix_rm (server, args_[1]);
               break;
            default:
               outlog << line << ": invalid command" << endl;
               break;
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

