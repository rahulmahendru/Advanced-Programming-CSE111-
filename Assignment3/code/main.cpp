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

void catfile(istream &infile)
{
   regex comment_regex{R"(^\s*(#.*)?$)"};
   regex key_value_regex{R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex{R"(^\s*([^=]+?)\s*$)"};
   for (;;)
   {
      string line;
      getline(infile, line);
      if (infile.eof())
         break;
      cout << endl
           << "input: \"" << line << "\"" << endl;
      smatch result;

      if (regex_search(line, result, comment_regex))
      {
         // Case 1 : comment
         cout << "Comment or empty line." << endl;
         continue;
      }
      if (regex_search(line, result, key_value_regex))
      {
         cout << "key  : \"" << result[1] << "\"" << endl;
         cout << "value: \"" << result[2] << "\"" << endl;

         if (result[1] == "")
         {
            if (result[2] == "")
            {
               //Case 5
            }
            else
            {
               //case 6
            }
         }
         else
         {
            if (result[2] == "")
            {
               //case 3
            }
            else
            {
               //case 4
            }
         }
      }
      else if (regex_search(line, result, trimmed_regex))
      {
         // Case 2 : only key
         // Done
         cout << "query: \"" << result[1] << "\"" << endl;
      }
      else
      {
         assert(false and "This can not happen.");
      }
   }
}

int main(int argc, char **argv)
{
   sys_info::execname(argv[0]);
   scan_options(argc, argv);

   str_str_map test;
   for (char **argp = &argv[optind]; argp != &argv[argc]; ++argp)
   {
      str_str_pair pair(*argp, to_string<int>(argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert(pair);
   }

   if (argc < 2)
   {
      catfile(cin);
   }
   else
   {
      const string cin_name = "-";
      for (int index = 1; index < argc; index++)
      {
         string myFileStr = argv[index];
         if (myFileStr == cin_name)
            catfile(cin);
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
               catfile(infile);
               infile.close();
            }
         }
      }
   }

   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor)
   {
      cout << "During iteration: " << *itor << endl;
   }

   //cout << "after Iteration" << endl;
   str_str_map::iterator itor = test.begin();
   test.erase(itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
