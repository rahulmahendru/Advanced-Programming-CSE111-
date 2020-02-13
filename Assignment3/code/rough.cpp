for (int index = 1; index < argc; index++)
      {
         string myFileStr = argv[index];
         if (myFileStr == cin_name)
            catfile(cin, "-", myMap);
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