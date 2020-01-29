// $Id: commands.cpp,v 1.18 2019-10-08 13:55:31-07 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if ( words.size == 1 ) {
      throw command_error("No files specified");
   }

   map<string, inode_ptr> check = state.getCwd()->getContents(file_type::DIRECTORY_TYPE)->getDirents();
   for ( int loopIndex = 1; loopIndex < check.size() ; loopIndex++ ) {
      const auto ptr = check.find(words[loopIndex]);
      if (ptr == check.end()){
         throw command_error("File not found");
      }
      else{
         if (ptr->second->getFileType() == file_type::PLAIN_TYPE) {
            cout << ptr->second->getContents(file_type::PLAIN_TYPE)->getData() ; << '\n';
         }
         else {
            throw command_error("Directory specified");
         }
      }
   }
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if ( words.size == 1 ) {
      throw command_error("No files specified");
   }

   if ( words[1] == "/" ) {
      state.setCwd(state.getRoot());
   }

   map<string, inode_ptr> check = state.getCwd()->getContents(file_type::DIRECTORY_TYPE)->getDirents();
   const auto ptr = check.find(words[1]);
   if (ptr == check.end()){
      throw command_error("File not found");
   }
   else{
      if (ptr->second->getFileType() == file_type::DIRECTORY_TYPE) {
         state.setCwd(ptr->second);
      }
      else {
         throw command_error("File specified");
      }
   }
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if ( words.size == 1 ) {
      throw command_error("No files specified");
   }
   
   map<string, inode_ptr> check = state.getCwd()->getContents(file_type::DIRECTORY_TYPE)->getDirents();

}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

