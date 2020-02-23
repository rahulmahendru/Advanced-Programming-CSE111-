// $Id: lsfile.cpp,v 1.10 2019-11-21 16:10:16-08 - - $

// NAME
//    lsfile - print output similar to ls(1) command
// SYNOPSIS
//    lsfile filename...
// DESCRIPTION
//    Calls the stat(2) system call and prints out struct stat
//    information about the file.

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

#include <libgen.h>
#include <pwd.h>
#include <sys/stat.h>

class failure: public exception {};

using num_base = ios_base& (*) (ios_base&);

void lsfile (const char* filename) {
   struct stat stat_buf;
   if (stat (filename, &stat_buf) != 0) throw failure();
   struct tm time_tm;
   localtime_r (&stat_buf.st_mtim.tv_sec, &time_tm);
   char time_str[64];
   strftime (time_str, sizeof time_str, "%Y-%m-%d %T", &time_tm);
   auto passwd_buf = getpwuid (stat_buf.st_uid);
   if (passwd_buf == nullptr) throw failure();
   cout << showbase << oct << setw(7) << stat_buf.st_mode << dec
        << setw(2) << stat_buf.st_nlink
        << setw(10) << passwd_buf->pw_name
        << setw(12) << stat_buf.st_size
        << " " << time_str
        << " " << filename << endl;
}

int main (int argc, char** argv) {
   string exec_name = basename (argv[0]);
   int exit_status = EXIT_SUCCESS;
   if (argc < 2) {
      cerr << "Usage: " << exec_name << " filename..." << endl;
      exit_status = EXIT_FAILURE;
   }else {
      for (auto argp = &argv[1]; argp != &argv[argc]; ++argp) {
         try {
            lsfile (*argp);
         }catch (failure&) {
            cerr << exec_name << ": " << *argp << ": "
                 << strerror (errno) << endl;
         }
      }
   }
   return exit_status;
}

