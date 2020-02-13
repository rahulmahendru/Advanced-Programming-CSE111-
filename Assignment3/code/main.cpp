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

void scan_options(int argc, char **argv)
{
   opterr = 0;
   for (;;)
   {
      int option = getopt(argc, argv, "@:");
      if (option == EOF)
         break;
      switch (option)
      {
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

string trim(string line)
{
   size_t first = line.find_first_not_of(' ');
   size_t last = line.find_last_not_of(' ');
   return line.substr(first, (last - first + 1));
}

void catfile(istream &infile, string filename, str_str_map &myMap)
{
   regex comment_regex{R"(^\s*(#.*)?$)"};
   regex key_value_regex{R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex{R"(^\s*([^=]+?)\s*$)"};
   int argNum {0};
   for (;;)
   {
      string line;
      getline(infile, line);
      if (infile.eof())
         break;
      cout << filename 
           << ": "
           << ++argNum
           << ": "
           << line 
           << endl;
      smatch result;

      if (regex_search(line, result, comment_regex))
      {
         continue;
      }
      if (regex_search(line, result, key_value_regex))
      {
         //cout << "key  : \"" << result[1] << "\"" << endl;
         //cout << "value: \"" << result[2] << "\"" << endl;

         if (result[1] == "")
         {
            if (result[2] == "")
            {
               for (str_str_map::iterator itor = myMap.begin();
                    itor != myMap.end(); ++itor){
                       cout << itor->first
                            << " = " 
                            << itor->second
                            << endl ;
               }
            }
            else
            {
               for (str_str_map::iterator itor = myMap.begin();
                    itor != myMap.end(); ++itor){
                       if (itor->second == result[2]){
                       cout << itor->first
                            << " = " 
                            << itor->second
                            << endl ;
                       }
               }
            }
         }
         else
         {
            if (result[2] == "")
            {
               if (myMap.find(result[1]) != myMap.end()) {
                  myMap.erase(myMap.find(result[1]));
               }
            }
            else
            {
               str_str_pair pair(result[1], result[2]);
               myMap.insert(pair);
               cout << line << endl;
            }
         }
      }
      else if (regex_search(line, result, trimmed_regex))
      {
         str_str_map::iterator curr = myMap.find(result[1]);
         if (curr->first == result[1]) {
            cout << curr->first
                 << " = " 
                 << curr->second
                 << endl ;
         }
         else {
            cout << result[1]
                 << ": Key not found" 
                 << endl;
         }
      }
      else
      {
         assert(false && "This can not happen.");
      }
   }
}

int main(int argc, char **argv)
{
   sys_info::execname(argv[0]);
   scan_options(argc, argv);

   str_str_map myMap;
   if (argc < 2)
   {
      catfile(cin, "-", myMap);
   }
   else {
      const string cin_name = "-";
      for (char **argp = &argv[optind]; argp != &argv[argc]; ++argp) {
         string myFileStr = *argp;
         if (myFileStr == cin_name) {
            catfile(cin, "-", myMap);
         }
         else
         {
            ifstream infile(myFileStr);
            if (infile.fail())
            {
               cerr << "matchlines: "
                    << ": " << myFileStr << ": "
                    << strerror(errno) << endl;
            }
            else
            {
               catfile(infile, myFileStr, myMap);
               infile.close();
            }
         }
      }
   }

   //str_str_map::iterator itor = myMap.begin();
   //myMap.erase(itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
