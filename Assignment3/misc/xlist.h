// $Id: xlist.h,v 1.3 2019-04-24 17:08:41-07 - - $

#ifndef __XLIST_H__
#define __XLIST_H__

template <typename T>
struct xlist {
   struct node;
   struct link {
      node* next;
      node* prev;
      explicit link (node* next_ = nullptr, node* prev_ = nullptr):
                     next(next_), prev(prev_) {}
      node* operator&() { return static_cast<node*> (this); }
   };
   struct node: link {
      T item;
      explicit node (node* next_ = nullptr, node* prev_ = nullptr,
                     const T& item_ = T()):
            link(next_, prev_), item(item_) {}
   };
   link base;

   xlist(): base (&base, &base) {}
   xlist (const xlist&) = delete;
   xlist& operator= (const xlist&) = delete;
   ~xlist() { while (not empty()) pop_back(); }

   bool empty() { return base.next == &base; }
   void push_back (const T&);
   void pop_back();
   T& back() { return base.prev->item; }

   class iterator;
   iterator begin() { return iterator (base.next); }
   iterator end() { return iterator (&base); }
};


template <typename T>
struct xlist<T>::iterator {
   node* curr;
   explicit iterator (node* curr_ = nullptr): curr(curr_) {}
   T& operator*() { return curr->item; }
   iterator& operator++() { curr = curr->next; return *this; }
   iterator& operator--() { curr = curr->prev; return *this; }
   bool operator== (const iterator &that) { return curr == that.curr; }
   bool operator!= (const iterator &that) { return curr != that.curr; }
};

template <typename T>
void xlist<T>::push_back (const T& item) {
   node* tmp = new node (&base, base.prev, item);
   base.prev = tmp;
   tmp->prev->next = tmp;
}

template <typename T>
void xlist<T>::pop_back() {
   node* tmp = base.prev;
   base.prev = tmp->prev;
   base.prev->next = &base;
   delete tmp;
}

#endif

