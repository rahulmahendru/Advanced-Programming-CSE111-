iterator newLink {find(pair.first)};
   if (newLink == end()) {
      node *newNode { new node(anchor(), anchor(), pair) };
      bool check = true;
      iterator curr;
      for(curr = begin(); curr != end(); ++curr) {
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