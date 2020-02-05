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

