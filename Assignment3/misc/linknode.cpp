// $Id: linknode.cpp,v 1.1 2016-06-28 14:46:57-07 - - $

#include <iostream>
using namespace std;

struct node;
struct link {
   node* next {};
   node* prev {};
};
struct node: link {
   int value {};
};

int main() {
   node* nn = new node();
   link* ll = nn;
   nn = static_cast<node*> (ll);
   cout << "nn = " << nn << ", sizeof = " << sizeof (node) << endl;
   cout << "ll = " << ll << ", sizeof = " << sizeof (link) << endl;
   char* p = new char;
   for (int i = 0; i < 8; ++i) {
      char* q = new char;
      cout << static_cast<void*> (q) << " " << q - p << endl;
      p = q;
   }
}

//TEST// linknode >linknode.out 2>&1
//TEST// mkpspdf linknode.ps linknode.cpp* linknode.out

