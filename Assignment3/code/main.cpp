// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string, string>;
using str_str_pair = str_str_map::value_type;

void scan_options(int argc, char **argv) {
   opterr = 0;
   for (;;) {
      int option = getopt(argc, argv, "@:");
      if (option == EOF)
         break;
      switch (option) {
      case '@':
         debugflags::setflags(optarg);
         break;
      default:
         complain() << "-" << char(optopt) << ": invalid option"
                    << endl;
         break;
      }
   }
}

// This function has been adapted from the catfile.cpp, loop-cin.cpp
// and matchlines.cpp for input check
void catfile(istream &infile, string filename, str_str_map &myMap) {
   regex comment_regex{R"(^\s*(#.*)?$)"};
   regex key_value_regex{R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex{R"(^\s*([^=]+?)\s*$)"};
   int argNum {0};
   for (;;) {
      string line;
      getline(infile, line);
      if (infile.eof())
         break;

      // Print the input line
      cout << filename 
           << ": "
           << ++argNum
           << ": "
           << line 
           << endl;
      smatch result;

      // Comment Case
      if (regex_search(line, result, comment_regex)) {
         continue;
      }

      // Case where the input line has a " = "
      if (regex_search(line, result, key_value_regex)) {
         // If the string before "=" is empty
         if (result[1] == "") {
            // If the string after "=" is also empty
            // Prints all the (key,value) pairs
            if (result[2] == "") {
               for (str_str_map::iterator itor = myMap.begin();
                    itor != myMap.end(); ++itor){
                       cout << itor->first
                            << " = " 
                            << itor->second
                            << endl ;
               }
            }
            // If the string after "=" is not empty
            // prints the pairs which match the specified value
            else {
               for (str_str_map::iterator itor = myMap.begin();
                    itor != myMap.end(); ++itor) {
                       if (itor->second == result[2]){
                       cout << itor->first
                            << " = " 
                            << itor->second
                            << endl ;
                       }
               }
            }
         }
         // If the string before "=" is not empty
         else {
            // If the string after "=" is empty
            // Deletes the (key,value) pair
            if (result[2] == "") {
               if (myMap.find(result[1]) != myMap.end()) {
                  myMap.erase(myMap.find(result[1]));
               }
            }
            // If the string after "=" is not empty
            // Inserts the new (key,value) pair
            // Replaces the value if the key already exists
            else {
               str_str_pair pair(result[1], result[2]);
               myMap.insert(pair);
               cout << result[1]
                    << " = " 
                    << result[2]
                    << endl;
            }
         }
      }
      // If only the key is specified
      else if (regex_search(line, result, trimmed_regex)) {
         // To find the key and value pair
         str_str_map::iterator curr = myMap.find(result[1]);
         if (curr != myMap.end()) {
            cout << curr->first
                 << " = " 
                 << curr->second
                 << endl ;
         }
         // Returns error statement if key is not found
         else {
            cout << result[1]
                 << ": Key not found" 
                 << endl;
         }
      }
      else {
         assert(false && "This can not happen.");
      }
   }
}

int main(int argc, char **argv)
{
   sys_info::execname(argv[0]);
   scan_options(argc, argv);

   str_str_map myMap;

   // Check if no operands are given
   if (argc < 2) {
      catfile(cin, "-", myMap);
   }
   // If operands are given
   else {
      const string cin_name = "-";
      for (char **argp = &argv[optind]; argp != &argv[argc]; ++argp) {
         string myFileStr = *argp;            // Store the filename
         if (myFileStr == cin_name) {         // Check for stdin
            catfile(cin, "-", myMap);
         }
         else {
            ifstream infile(myFileStr);
            // Check for file existence       
            if (infile.fail()) {
               cerr << "keyvalue: "
                    << myFileStr << ": "
                    << strerror(errno) << endl;
            }
            else {
               catfile(infile, myFileStr, myMap);
               infile.close();
            }
         }
      }
   }

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
