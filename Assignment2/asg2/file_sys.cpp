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
   size_t size {data.size()};
   DEBUGF ('i', "size = " << size);
   return size;
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
   const auto index = dirents.find(filename) ;
   if ( index == dirents.end()) {
      throw file_error ("No such file or directory");
   }
   else {
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
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);

   const auto result = dirents.find(dirname);
   if (result != dirents.end())
   {
      throw file_error("is a " + error_file_type());
   }

   inode_ptr newDirectoryPtr = make_shared<inode>(file_type::DIRECTORY_TYPE);
   inode_ptr parentPtr = dirents.find(".")->second;
   newDirectoryPtr->getContentsAsDirectory()->initializeDirectory(parentPtr, newDirectoryPtr);
   dirents.insert({dirname+"/", newDirectoryPtr});
   return nullptr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr newFilePtr = make_shared<inode>(file_type::PLAIN_TYPE);
   dirents.insert({filename, newFilePtr});
   return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Self Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//-------------- Inode ---------------------
shared_ptr<directory> inode::getContentsAsDirectory() {
   return dynamic_pointer_cast<directory>(contents) ;
}

file_type inode::getFileType() {
   return typeOfFile ;
}

//-------------- Plain_file ----------------
void plain_file::initializeDirectory(const inode_ptr&, const inode_ptr&) {
   throw file_error ("is a " + error_file_type());
}

//--------------- Directory ----------------
void directory::initializeDirectory(const inode_ptr& parent, const inode_ptr& current)
{
   dirents.insert({"..", parent});
   dirents.insert({".", current});
}

