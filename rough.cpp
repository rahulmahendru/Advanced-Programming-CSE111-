// $Id: matchlines.cpp,v 1.1 2016-06-28 14:46:02-07 - - $


// Goals of the ASG3
// 1. Parse over line by line either an input file or cin
// 2. Per line, use regex regular expressions to match input 
// and extract results
// 3. Implement a template class: a map implemented as a circular
// dobule linked list that has an interator class
//
// Matches lines according to the program specs.
// Prints out results depending on type of line.
//

#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
using namespace std;

void catfile (istream& infile) {
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   for (;;) {
      string line;
      getline (infile, line);
      if (infile.eof()) break;
      cout << endl << "input: \"" << line << "\"" << endl;
      smatch result;
      
      if (regex_search (line, result, comment_regex)) {
         // Case 1 : comment
         cout << "Comment or empty line." << endl;
         continue;
      }
      if (regex_search (line, result, key_value_regex)) {
         cout << "key  : \"" << result[1] << "\"" << endl;
         cout << "value: \"" << result[2] << "\"" << endl;
         /* if ( - result 1 : empty - ) {
            if (resutl2 : empty) {
               Case 5

            } else {
               case 6
            }
            else {
               if(){
                  case 3
               }
               else(){
                  case 4
               }
            }
         } */
         
      }else if (regex_search (line, result, trimmed_regex)) {
         // Case 2 : only key
         cout << "query: \"" << result[1] << "\"" << endl;
      }else {
         assert (false and "This can not happen.");
      }
   }

   for(;;) {
      string line;
      getline (infile, line);
      if (infile.eof()) break;
      cout << line << endl;
   }
}

int main() {
    // For file
    string myFileStr = "shared.cpp" ;
    ifstream infile(myFileStr);
    if (infile.fail()) {
            cerr << "matchlines: " << ": " << myFileStr << ": "
                 << strerror (errno) << endl;
    }else {
            catfile (infile);
            infile.close();
     }
     catfile(cin);
   return 0;
}

/*
//TEST// (echo '    # comment   ';
//TEST//  echo '                  ';
//TEST//  echo '    No equal in this line.           ';
//TEST//  echo '   key key key  =  value value value  ';
//TEST//  echo 'only key here =     ';
//TEST//  echo '  = only value here     ';
//TEST//  echo '   =   '
//TEST// ) | matchlines >matchlines.out 2>&1
//TEST// mkpspdf matchlines.ps matchlines.cpp* matchlines.out
*/


// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt(argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

string trim (string line) {
   size_t first = line.find_first_not_of(' ');
   size_t last = line.find_last_not_of(' ');
   return line.substr(first, (last - first + 1));
}

void catfile (istream& infile) {
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   for (;;) {
      string line;
      getline (infile, line);
      if (infile.eof()) break;
      cout << endl << "input: \"" << line << "\"" << endl;
      smatch result;
      
      if (regex_search (line, result, comment_regex)) {
         // Case 1 : comment
         cout << "Comment or empty line." << endl;
         continue;
      }
      if (regex_search (line, result, key_value_regex)) {
         cout << "key  : \"" << result[1] << "\"" << endl;
         cout << "value: \"" << result[2] << "\"" << endl;
         /* if ( - result 1 : empty - ) {
            if (resutl2 : empty) {
               Case 5

            } else {
               case 6
            }
            else {
               if(){
                  case 3
               }
               else(){
                  case 4
               }
            }
         } */
         
      }else if (regex_search (line, result, trimmed_regex)) {
         // Case 2 : only key
         cout << "query: \"" << result[1] << "\"" << endl;
      }else {
         assert (false and "This can not happen.");
      }
   }

   for(;;) {
      string line;
      getline (infile, line);
      if (infile.eof()) break;
      cout << line << endl;
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   

   str_str_map test;
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

