  wordvec path = split(words, "/");
   inode_ptr currentDir = nullptr;
   inode_ptr parentDir = nullptr;
   if (words.at(0) == '/') {
      currentDir = root ;
   }
   else {
      currentDir = current;
   }
   for (unsigned long loopIndex = 0; loopIndex < path.size(); loopIndex ++) {
      map<string, inode_ptr> currentDirPtr = currentDir->getContentsAsDirectory()->getDirents();
      const auto checkPtr = currentDirPtr.find(path[loopIndex]);
      if (checkPtr == currentDirPtr.end()){
         return nullptr;
      }
      if (loopIndex == path.size() - 2) {
         parentDir = checkPtr->second;
      }
      currentDir = checkPtr->second;
   }
   if (path.size() > 1){
      if (check == 0) {
         return currentDir;
      }
      else {
         return parentDir;
      }
   }
   return currentDir;