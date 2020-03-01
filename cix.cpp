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
   //outlog << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   //outlog << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      outlog << "cix: ls failed: " << strerror(errno) << endl;
      //outlog << "sent LS, server did not return LSOUT" << endl;
      //outlog << "server returned " << header << endl;
   }
   else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      outlog << "received " << host_nbytes << " bytes" << endl;
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   }
}

// Functions to be completed
void cix_get(client_socket& server, string& file) {
   cix_header header;
   header.command = cix_command::GET;
   strcpy(header.filename, file.c_str());
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   outlog << "received header " << header << endl;
   if(header.command != cix_command::FILEOUT) {
      outlog << "sent GET, server did not return FILEOUT" << endl;
      outlog << "server returned " << header << endl;
   }
   else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      outlog << "received " << host_nbytes << " bytes" << endl;
      buffer[host_nbytes] = '\0';
      ofstream newFile;
      newFile.open(file);
      newFile << buffer.get();
      newFile.close();
   }
}

void cix_put(client_socket& server, string& file) {
   cix_header header;
   header.command = cix_command::PUT;
   strcpy(header.filename, file.c_str());
   ifstream get_file (header.filename, ifstream::binary);
   if(!get_file) {
      outlog << "get: File not found: " << strerror (errno) << endl;
      header.command = cix_command::NAK;
      header.nbytes = htonl (errno);
      send_packet (server, &header, sizeof header);
   }
   else {
      get_file.seekg(0, get_file.end);
      int length = get_file.tellg();
      get_file.seekg(0, get_file.beg);
      header.nbytes = htonl (length);
      char buffer[length];
      get_file.read(buffer, length);
      get_file.close();
      send_packet (server, &header, sizeof header);
      send_packet (server, buffer, length);
      recv_packet (server, &header, sizeof header);
      outlog << "Sent header " << header << endl;
      memset (header.filename, 0, FILENAME_SIZE);
   }
   
   if(header.command != cix_command::ACK) {
      outlog << "sent PUT, server did not return ACK" << endl;
      outlog << "server returned " << header << endl;
   }

}

void cix_rm(client_socket& server, string& file) {
   cix_header header;
   header.command = cix_command::RM;
   strcpy(header.filename, file.c_str());
   header.nbytes = 0 ;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   outlog << "received header " << header << endl;
   if(header.command != cix_command::ACK) {
      outlog << "sent RM, server did not return ACK" << endl;
      outlog << "server returned " << header << endl;
   }
   cout << "Now here" << endl;
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

