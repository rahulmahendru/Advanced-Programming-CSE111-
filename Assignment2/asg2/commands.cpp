// $Id: commands.cpp,v 1.18 2019-10-08 13:55:31-07 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash{
    {"cat", fn_cat},
    {"cd", fn_cd},
    {"echo", fn_echo},
    {"exit", fn_exit},
    {"ls", fn_ls},
    {"lsr", fn_lsr},
    {"make", fn_make},
    {"mkdir", fn_mkdir},
    {"prompt", fn_prompt},
    {"pwd", fn_pwd},
    {"rm", fn_rm},
    {"rmr", fn_rmr},
    {"#", fn_comment},
};

command_fn find_command_fn(const string &cmd)
{
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF('c', "[" << cmd << "]");
   const auto result = cmd_hash.find(cmd);
   if (result == cmd_hash.end())
   {
      throw command_error(cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error(const string &what) : runtime_error(what)
{
}

int exit_status_message()
{
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_cat(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   if (words.size() == 1)
   {
      throw command_error("No files specified");
   }
   int loopSize = words.size();
   int loopIndex = 1;
   while (loopIndex < loopSize)
   {
      inode_ptr current = state.resolveInputPtr(words[loopIndex], state.getCwd(), 1);
      if (current == nullptr)
      {
         throw command_error("Path does not exist");
      }
      string filename = state.resolveInputString(words[loopIndex]);
      map<string, inode_ptr> currentPtr = current->getContentsAsDirectory()->getDirents();
      map<string, inode_ptr>::iterator checkPtr = currentPtr.find(filename);
      if (checkPtr == currentPtr.end())
      {
         throw command_error("File not found");
      }
      else
      {
         if (checkPtr->second->getFileType() == file_type::DIRECTORY_TYPE)
         {
            throw command_error("Directory exists with the name");
         }
         //cout << filename << "\n";
         cout << checkPtr->second->getContentsAsPlainFile()->getData() << "\n";
      }
      loopIndex++;
   }
}

void fn_cd(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   if (words.size() == 1 || words[1] == "/")
   {
      state.setCwd(state.getRoot());
   }
   else
   {
      inode_ptr current = state.resolveInputPtr(words[1], state.getCwd(), 0);
      if (current == nullptr)
      {
         throw command_error("Invalid path");
      }
      state.setCwd(current);
   }
}

void fn_echo(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   cout << word_range(words.cbegin() + 1, words.cend()) << endl;
}

void fn_exit(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   throw ysh_exit();
}

void fn_ls(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
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
   state.printDirectory(current);
}

void fn_lsr(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
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
   state.printDirectoryRecursive(current);
}

void fn_make(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   if (words.size() == 1)
   {
      throw command_error("No files specified");
   }
   inode_ptr current = state.resolveInputPtr(words[1], state.getCwd(), 1);
   if (current == nullptr)
   {
      throw command_error("Path does not exist");
   }
   string filename = state.resolveInputString(words[1]);
   map<string, inode_ptr> currentPtr = current->getContentsAsDirectory()->getDirents();
   map<string, inode_ptr>::iterator checkPtr = currentPtr.find(filename);
   if (checkPtr == currentPtr.end())
   {

      inode_ptr newFile = current->getContentsAsDirectory()->mkfile(filename);
      if (words.size() > 2)
      {
         newFile->getContentsAsPlainFile()->writefile(words);
      }
   }
   else
   {
      if (checkPtr->second->getFileType() == file_type::DIRECTORY_TYPE)
      {
         throw command_error("Directory exists with the name");
      }
      //cout << filename << "\n";
      checkPtr->second->getContentsAsPlainFile()->writefile(words);
   }
}

void fn_mkdir(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   if (words.size() == 1)
   {
      throw command_error("No files specified");
   }
   inode_ptr current = state.resolveInputPtr(words[1], state.getCwd(), 1);
   if (current == nullptr)
   {
      throw command_error("Path does not exist");
   }
   string filename = state.resolveInputString(words[1]);
   map<string, inode_ptr> currentPtr = current->getContentsAsDirectory()->getDirents();
   map<string, inode_ptr>::iterator checkPtr = currentPtr.find(filename);
   if (checkPtr != currentPtr.end())
   {
      throw command_error("File or directory already exists");
   }
   current->getContentsAsDirectory()->mkdir(filename);
}

void fn_prompt(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   state.setPrompt(words);
}

void fn_pwd(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   string path = state.getPath(state.getCwd());
   cout << path << "\n";
}

void fn_rm(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   if (words.size() == 1)
   {
      throw command_error("No files specified");
   }
   inode_ptr current = state.resolveInputPtr(words[1], state.getCwd(), 1);
   if (current == nullptr)
   {
      throw command_error("Path does not exist");
   }
   string filename = state.resolveInputString(words[1]);
   map<string, inode_ptr> currentPtr = current->getContentsAsDirectory()->getDirents();
   map<string, inode_ptr>::iterator checkPtr = currentPtr.find(filename);
   if (checkPtr == currentPtr.end())
   {
      throw command_error("No such file or directory");
   }
   current->getContentsAsDirectory()->remove(filename);
}

void fn_rmr(inode_state &state, const wordvec &words)
{
   DEBUGF('c', state);
   DEBUGF('c', words);
   // NEEDS CORRECTION
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
   state.removeRecursive(current);
}

void fn_comment(inode_state &state, const wordvec &words)
{
   int loopSize = words.size();
   int loopIndex = 1;
   while (loopIndex < loopSize)
   {
      cout << words[loopIndex] << " ";
      loopIndex++;
   }
   cout << "\n";
}
