// $Id: matchlines.cpp,v 1.1 2016-06-28 14:46:02-07 - - $

//
// Matches lines according to the program specs.
// Prints out results depending on type of line.
//

#include <cassert>
#include <iostream>
#include <regex>
#include <string>
using namespace std;

int main() {
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   for (;;) {
      string line;
      getline (cin, line);
      if (cin.eof()) break;
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

