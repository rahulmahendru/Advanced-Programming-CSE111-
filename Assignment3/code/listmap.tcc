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
   iterator newLink = find(pair.first);
   if (newLink == anchor()) {
      node *newNode = new node(anchor(), anchor()->prev, pair);
      bool check = true;
      iterator curr = begin();
      for(;curr.where != anchor(); ++curr) {
         if (less(pair.first, curr.where->value.first)){
            newNode->prev = curr.where->prev;
            newNode->next = curr.where;
            curr.where->prev->next = newNode;
            curr.where->prev       = newNode;
            check = false;
            break;
         }
      }
      if (check) {
         curr.where->prev->next = newNode;
         curr.where->prev = newNode;
      }
      return iterator(newNode);
   }
   else {
      newLink.where->value.second = pair.second;
      return newLink;
   }
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   for(iterator curr = begin(); curr.where != anchor(); ++curr) {
      if(!(less(curr.where->value.first, that) && !(less(that, curr.where->value.first)))){
         return iterator(curr);
      }
   }
   return end();
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   return iterator();
}


