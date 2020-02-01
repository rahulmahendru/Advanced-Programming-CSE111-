// $Id: file_sys.cpp,v 1.7 2019-07-09 14:05:44-07 - - $

#include <iostream>
#include <stdexcept>
#include <unordered_map>

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

   root = make_shared<inode>(file_type::DIRECTORY_TYPE) ;
   root->getContentsAsDirectory()->initializeDirectory(root, root);
   cwd = root ;
}

const string& inode_state::prompt() const { return prompt_; }

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
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
   if (data.empty()){
      return size;
   }
   int loopsize = data.size();
   for (int loopIndex = 0; loopIndex < loopsize; loopIndex++){
      size = size + data[loopIndex].length();
   }
   size = size + data.size();
   return size - 1;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   data.clear();
   for (unsigned long loopIndex = 2; loopIndex < words.size(); loopIndex++)
   {
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

   // Try to make functions out of these as removeFile and removeDirectory
   if ( filenameType == file_type::PLAIN_TYPE) {
      index->second = nullptr;
      dirents.erase(filename) ;
    }
   else if (filenameType == file_type::DIRECTORY_TYPE){
      if ( index->second->getContentsAsDirectory()->dirents.size() > 2) {
         throw file_error ("Directory is not empty") ;
      }
      else {
         index->second->getContentsAsDirectory()->dirents.find("..")->second = nullptr;
         index->second->getContentsAsDirectory()->dirents.find(".")->second = nullptr;
         index->second->getContentsAsDirectory()->dirents.clear();
         index->second = nullptr;
         dirents.erase(filename) ;
      }
   }
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);

   inode_ptr newDirectoryPtr = make_shared<inode>(file_type::DIRECTORY_TYPE);
   inode_ptr parentPtr = dirents.find(".")->second;
   newDirectoryPtr->getContentsAsDirectory()->initializeDirectory(parentPtr, newDirectoryPtr);
   dirents.insert({dirname, newDirectoryPtr});
   return newDirectoryPtr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr newFilePtr = make_shared<inode>(file_type::PLAIN_TYPE);
   dirents.insert({filename, newFilePtr});
   return newFilePtr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Self Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//-------------- Inode_state ---------------
inode_ptr inode_state::getCwd() {
   return cwd;
}

inode_ptr inode_state::getRoot() {
   return root;
}

string inode_state::getPath(inode_ptr current){
   string path {""};
   if ( current == root ) {
      return "/";
   }
   else {
      while (current != root){
         inode_ptr parent = current->getContentsAsDirectory()->getDirents().find("..")->second;
         map<string, inode_ptr> currentDir = parent->getContentsAsDirectory()->getDirents();
         map<string, inode_ptr>::iterator index;
         for(index = currentDir.begin(); index != currentDir.end(); index++ ) {
            if (index->first=="." || index->first==".."){
            }
            else if (index->second == current){
               path = index->first + "/" + path ;
            }
         }
         current = parent;
      }
      path.pop_back();
      path = "/" + path;
   }
   return path;
}

void inode_state::printDirectory(inode_ptr current) {
   cout << getPath(current) << ":" << "\n" ;
   map<string, inode_ptr> printDir = current->getContentsAsDirectory()->getDirents();
   map<string, inode_ptr>::iterator index;
   for(index = printDir.begin(); index != printDir.end(); index++ ) {
      if (index->first == ".." || index->first == "."){
         cout << "     " 
              << index->second->inode_nr 
              << "  " 
              << "     "
              << index->second->getContentsAsDirectory()->size() 
              << "  " 
              << index->first 
              << "\n" ;
      }
      else if (index->second->getFileType() == file_type::DIRECTORY_TYPE){
         cout << "     " 
              << index->second->inode_nr 
              << "  "
              << "     "
              << index->second->getContentsAsDirectory()->size() 
              << "  " 
              << index->first 
              << "/"
              << "\n";
      }
      else if (index->second->getFileType() == file_type::PLAIN_TYPE) {
         cout << "     " 
              << index->second->inode_nr 
              << "  " 
              << "     "
              << index->second->getContentsAsPlainFile()->size() 
              << "  " 
              << index->first 
              << "\n" ;
      }
   }
}

inode_ptr inode_state::resolveInputPtr(const string& words, const inode_ptr& current, const int check) {
   inode_ptr currentDir = nullptr;
   if (words[0] == '/'){
      currentDir = root;
   }
   else {
      currentDir = current;
   }
   wordvec path = split(words,"/");
   int loopSize = path.size() - check;
   if (loopSize < 1){
      return currentDir;
   }
   for (int loopIndex = 0; loopIndex < loopSize; loopIndex++){
      map<string, inode_ptr> currentDirPtr = currentDir->getContentsAsDirectory()->getDirents();
      map<string, inode_ptr>::iterator checkPtr = currentDirPtr.find(path[loopIndex]);
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

string inode_state::resolveInputString(const string& words) {
   wordvec path = split(words,"/");
   return path[path.size() - 1];
}

void inode_state::setPrompt(const string& words) {
   prompt_ = words + " ";
}

void inode_state::setCwd(inode_ptr current) {
   cwd = current;
}

//-------------- Inode ---------------------
shared_ptr<directory> inode::getContentsAsDirectory() {
   return dynamic_pointer_cast<directory>(contents) ;
}

shared_ptr<plain_file> inode::getContentsAsPlainFile() {
   return dynamic_pointer_cast<plain_file>(contents) ;
}

file_type inode::getFileType() {
   return typeOfFile ;
}

//-------------- Plain_file ----------------
void plain_file::initializeDirectory(const inode_ptr&, const inode_ptr&) {
   throw file_error ("is a " + error_file_type());
}

map<string, inode_ptr> plain_file::getDirents() {
   throw file_error ("is a " + error_file_type());
}

wordvec plain_file::getData() {
   return data;
}

//--------------- Directory ----------------
void directory::initializeDirectory(const inode_ptr& parent, const inode_ptr& current)
{
   dirents.insert({"..", parent});
   dirents.insert({".", current});
}

map<string, inode_ptr> directory::getDirents() {
   return dirents;
}

wordvec directory::getData() {
   throw file_error ("is a " + error_file_type());
}