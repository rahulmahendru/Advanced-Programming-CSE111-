// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   // To erase the listmap
   while(!(empty())){
      erase(begin());
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   node* newNode;
   iterator curr;
   for (curr = begin(); curr != end(); ++curr) {

      // To check if the key already exists
      // replace the value in the case
      if (!(less(pair.first, curr.where->value.first)) &&
          !(less(curr.where->value.first, pair.first))) {
             curr.where->value.second = pair.second;
             break;
      }

      // To find the point where the key should be inserted
      if (less(pair.first, curr.where->value.first)) {
         newNode = new node(anchor(), anchor(), pair) ;
         newNode->prev = curr.where->prev;
         newNode->next = curr.where;
         curr.where->prev->next = newNode;
         curr.where->prev = newNode;
         break;
      }
   }
   // If insertion take splace at the end of the listmap
   if (curr == end()) {
      newNode = new node(anchor(), anchor(), pair) ;
      newNode->prev = curr.where->prev;
      curr.where->prev->next = newNode;
      curr.where->prev = newNode;
   }
   return iterator(newNode);
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);

   // If the listmap is empty
   if (begin() == end()){
      return end();
   }
   // Find the key
   for(iterator curr = begin(); curr != end(); ++curr) {
      if(!(less(curr.where->value.first, that) && 
         !(less(that, curr.where->value.first)))){
         return curr;
      }
   }
   // if key not found
   return end();
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   // To erase the position
   iterator curr = position.where->next;
   position.where->prev->next = position.where->next;
   position.where->next->prev = position.where->prev;
   delete(position.where);
   return curr;
}


