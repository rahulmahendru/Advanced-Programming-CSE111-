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
   root->inode_nr = inode::next_inode_nr ;
   root->contents = make_shared<directory>();
   root->contents->mkdir(".");
   root->contents->mkdir("..");
   cwd = root;
}

const string& inode_state::prompt() const { return prompt_; }

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
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
   data.erase(data.begin(), data.end());
   vector<const string>::iterator index;
   for ( index = words.begin(); index != words.end(); index++ ) {
      data.push_back(*index);
   }
}

size_t directory::size() const {
   size_t size {dirents.size()};
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   bool check = false ;
   map<string, inode_ptr>::iterator index;
   for (index = dirents.begin(); index != dirents.end; index++){
      if (index->first == filename){
         dirents.erase(filename);
         check = true;
      }
   }
   if (!check) {
      throw file_error("is a " + error_file_type() ) ;
   }
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   const auto result = dirents.find ( dirname ) ;

   if ( result != dirents.end() ) {
      throw file_error("is a " + error_file_type() ) ;
   }

   inode_ptr newDirectoryPtr = make_shared<inode>(file_type::DIRECTORY_TYPE) ;

   dirents.insert({dirname, newDirectoryPtr});
   return newDirectoryPtr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr newFilePtr = make_shared<inode>(file_type::PLAIN_TYPE) ;
   dirents.insert({filename, newFilePtr}) ;
   return newFilePtr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Self functions

inode_ptr inode_state::getCwd() {
   return cwd;
}

inode_ptr inode_state::getParentDir(inode_ptr dirname) {
   if ( cwd == root ) {
      return root ;
   }
   else {

   }
}

map<string,inode_ptr> plain_file::getDirents(const string& dirname) {
   throw file_error ( "is a " + error_file_type() ); 
}

map<string,inode_ptr> directory::getDirents(const string& dirname) {
   return dirents;
}