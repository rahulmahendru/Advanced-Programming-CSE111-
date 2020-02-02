inode_ptr current;
   if (words.size() == 1 || words[1] == ".")
   {
      current = state.getCwd();
   }
   else if (words[1] == "/")
   {
      current = state.getRoot();
   }
   else
   {
      current = state.resolveInputPtr(words[1], state.getCwd(), 0);
   }
   if (current == nullptr)
   {
      throw command_error("Path does not exist");
   }
   if (current->getFileType() == file_type::PLAIN_TYPE)
   {
      throw command_error("Not a directory");
   }
   string path = "";
   if (words.size() > 1)
   {
      path = words[1];
   }
   state.printDirectory(current, path);