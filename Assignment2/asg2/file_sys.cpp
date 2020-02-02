// $Id: file_sys.cpp,v 1.7 2019-07-09 14:05:44-07 - - $

// Rahul Mahendru (ramahend@ucsc.edu)
// Ivan Garcia-Sanchez (igarci33@ucsc.edu)

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>

using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");

   // Initialize the root
   root = make_shared<inode>(file_type::DIRECTORY_TYPE) ;
   root->getContentsAsDirectory()->initializeDirectory(root, root) ;
   cwd = root ;
}

const string& inode_state::prompt() const { return prompt_; }

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   // to store the type of file in order to check for file_type
   typeOfFile  = type ;
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}


file_error::file_error (const string& what):
            runtime_error (what) {
}

const wordvec& base_file::readfile() const {
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type());
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);

   // to check if data is empty
   if (data.empty()){
      return size;
   }

   int loopsize = data.size();

   // Loop to count the number of characters
   for (int loopIndex = 0; loopIndex < loopsize; loopIndex++) {
      size = size + data[loopIndex].length();
   }

   // Add the number of words and resturn the size - 1 space
   size = size + data.size();
   return size - 1 ;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   data.clear();
   for (unsigned long loopIndex = 2; loopIndex < words.size();
        loopIndex++) {
      data.push_back(words[loopIndex]);
   }
}

size_t directory::size() const {
   size_t size {dirents.size()};
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   map<string, inode_ptr>::iterator index = dirents.find(filename) ;
   file_type filenameType = index->second->getFileType();

   // Check for plain file
   if ( filenameType == file_type::PLAIN_TYPE) {
      index->second = nullptr;
      dirents.erase(filename) ;
   }
   else if (filenameType == file_type::DIRECTORY_TYPE) {
      if ( index->second->getContentsAsDirectory()->
           dirents.size() > 2) {
         throw runtime_error 
         ("rm: " + filename + ": Directory not empty") ;
      }
      else {
         index->second->getContentsAsDirectory()->
         dirents.find("..")->second = nullptr;
         index->second->getContentsAsDirectory()->
         dirents.find(".")->second = nullptr;
         index->second->getContentsAsDirectory()->
         dirents.clear();
         index->second = nullptr;
         dirents.erase(filename) ;
      }
   }
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);

   inode_ptr newDirectoryPtr = make_shared<inode>
                               (file_type::DIRECTORY_TYPE);
   inode_ptr parentPtr = dirents.find(".")->second;
   newDirectoryPtr->getContentsAsDirectory()->
   initializeDirectory(parentPtr, newDirectoryPtr);
   dirents.insert({dirname, newDirectoryPtr});
   return newDirectoryPtr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr newFilePtr = make_shared<inode>(file_type::PLAIN_TYPE) ;
   dirents.insert({filename, newFilePtr});
   return newFilePtr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Self Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//-------------- Inode_state ---------------

// returns the cwd
inode_ptr inode_state::getCwd() {
   return cwd;
}

// returns the root
inode_ptr inode_state::getRoot() {
   return root;
}

// returns the path
string inode_state::getPath(inode_ptr current){
   string path {""};

   // if cwd is root, return root
   if ( current == root ) {
      return "/";
   }
   
   else {
      while (current != root){

         inode_ptr parent = current->getContentsAsDirectory()->
                            getDirents().find("..")->second;
         map<string, inode_ptr> currentDir = parent->
                                getContentsAsDirectory()->
                                getDirents();
         map<string, inode_ptr>::iterator index;

         for(index = currentDir.begin(); index != currentDir.end() ; 
             index++ ) {
            if (index->first=="." || index->first=="..") {
            }
            else if (index->second == current) { 
               path = index->first + "/" + path ;
            }
         }
         current = parent;
      }

      path.pop_back() ;
      path = "/" + path ;
   }
   return path ;
}


// Prints the directory
void inode_state::printDirectory(inode_ptr current, 
                                 const string& path) {
   
   // To print the directory name
   if (path == "") {
      cout << getPath(current) << ":" << "\n" ;
   }
   else {
      cout << path << ":" << "\n" ;
   }

   // To print a file
   if ( current->getFileType() == file_type::PLAIN_TYPE ){
      cout << right << setw(6)  
              << current->inode_nr
              << "  " 
              << right << setw(6)
              << current->getContentsAsPlainFile()->size()
              << "  " 
              << left
              << path
              << "\n" ;
   }
   // To print a directory
   else {
      map<string, inode_ptr> printDir = current->
                             getContentsAsDirectory()->
                             getDirents();
      map<string, inode_ptr>::iterator index;
      for (index = printDir.begin(); index != printDir.end(); 
           index++) {
         // To check for . and .. and not print a following /
         if (index->first == ".." || index->first == ".") {
            cout << right << setw(6)
                 << index->second->inode_nr
                 << "  "
                 << right << setw(6)
                 << index->second->getContentsAsDirectory()->size()
                 << "  "
                 << left
                 << index->first
                 << "\n";
         }
         // print a following / for other directories
         else if (index->second->getFileType() == 
                  file_type::DIRECTORY_TYPE) {
            cout << right << setw(6)
                 << index->second->inode_nr
                 << "  "
                 << right << setw(6)
                 << index->second->getContentsAsDirectory()->size()
                 << "  "
                 << left
                 << index->first
                 << "/"
                 << "\n";
         }
         else if (index->second->getFileType() == 
                  file_type::PLAIN_TYPE) {
            cout << right << setw(6)
                 << index->second->inode_nr
                 << "  "
                 << right << setw(6)
                 << index->second->getContentsAsPlainFile()->size()
                 << "  "
                 << left
                 << index->first
                 << "\n";
         }
      }
   }
}

// Recursive funtion to print by depth search
void inode_state::printDirectoryRecursive(inode_ptr current) {
   printDirectory(current, getPath(current));
   map<string, inode_ptr> checkDir = current->
                          getContentsAsDirectory()->
                          getDirents();
   map<string, inode_ptr>::iterator index = checkDir.begin() ;

   while (index != checkDir.end()){
      if (index->first=="." || index->first==".."){
      }
      else if (index->second->getFileType() == 
               file_type::DIRECTORY_TYPE){
         printDirectoryRecursive(index->second);
      }
      index++;
   }
}

inode_ptr inode_state::resolveInputPtr(const string& words, 
                                       const inode_ptr& current, 
                                       const int check) {
   inode_ptr currentDir = nullptr;

   // Check for the root
   if (words[0] == '/'){
      currentDir = root;
   }
   else {
      currentDir = current;
   }

   // split the path to get the path size
   wordvec path = split(words,"/");
   int loopSize = path.size() - check;

   if (loopSize < 1) {
      return currentDir;
   }

   // Loop to decode the path name and find a path
   for (int loopIndex = 0; loopIndex < loopSize; loopIndex++) {
      map<string, inode_ptr> currentDirPtr = currentDir->
                             getContentsAsDirectory()->
                             getDirents();
      map<string, inode_ptr>::iterator checkPtr = 
                             currentDirPtr.find(path[loopIndex]);

      if (checkPtr == currentDirPtr.end()){
         return nullptr;
      }

      currentDir = checkPtr->second;

      if (currentDir->getFileType() == file_type::PLAIN_TYPE){
         return nullptr;
      }

   }
   return currentDir;
}

// To return the name of the file as a string
string inode_state::resolveInputString(const string& words) {
   wordvec path = split(words,"/");
   return path[path.size() - 1];
}

// To set the prompt 
void inode_state::setPrompt(const wordvec& words) {

   int loopSize = words.size() ;
   int loopIndex = 1 ;
   prompt_ = "";

   // Loop to get all the words specified as the prompt
   while ( loopIndex < loopSize ) {
      prompt_ = prompt_ + words[loopIndex] + " ";
      loopIndex++;
   }
}

// Set the cwd
void inode_state::setCwd(inode_ptr current) {
   cwd = current;
}


//-------------- Inode ---------------------

// Reutrns a shared pointer to directory
shared_ptr<directory> inode::getContentsAsDirectory() {
   return dynamic_pointer_cast<directory>(contents) ;
}

// returns a shared pointer to a plain_file
shared_ptr<plain_file> inode::getContentsAsPlainFile() {
   return dynamic_pointer_cast<plain_file>(contents) ;
}

// returns the file type of contents
file_type inode::getFileType() {
   return typeOfFile ;
}

//-------------- Plain_file ----------------
void plain_file::initializeDirectory(const inode_ptr&,
                                     const inode_ptr&) {
   throw file_error ("is a " + error_file_type());
}

map<string, inode_ptr> plain_file::getDirents() {
   throw file_error ("is a " + error_file_type());
}

void plain_file::removeRecursive(){
   throw file_error ("is a " + error_file_type());
}

//--------------- Directory ----------------
void directory::initializeDirectory(const inode_ptr& parent, 
                                    const inode_ptr& current) {
   dirents.insert({"..", parent});
   dirents.insert({".", current});
}

// return the dirents
map<string, inode_ptr> directory::getDirents() {
   return dirents;
}

// recursive function to remove all the files on the pathname
void directory::removeRecursive() {

   map<string, inode_ptr>::reverse_iterator index = 
                                 dirents.rbegin() ;

   while ( index != dirents.rend() ) {
      if (index->first=="." || index->first==".."){
         index++;
         continue;
      }
      else if ( index->second->getFileType() == 
                file_type::DIRECTORY_TYPE) {
         index->second->getContentsAsDirectory()->
         removeRecursive() ;
      }
      remove(index->first);
   }
}