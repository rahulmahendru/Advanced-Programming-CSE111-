// $Id: testxless.cpp,v 1.2 2019-02-07 13:26:15-08 - - $

// Example of how to use xless object on a pair of strings.

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

template <typename Type>
struct xless {
   bool operator() (const Type &left, const Type &right) const {
      return left < right;
   }
};

template <typename Type, class Less=xless<Type>>
void compare (const Type &left, const Type &right) {
   Less less;
   cout << boolalpha;
   bool is_less = less (left, right);
   bool is_greater = less (right, left);
   bool is_equal = not is_less and not is_greater;
   cout << left << " <  " << right << " = " << is_less << endl;
   cout << left << " == " << right << " = " << is_equal << endl;
   cout << left << " >  " << right << " = " << is_greater << endl;
   cout << endl;
}

int main () {
   compare (string ("hello"), string ("world"));
   compare (string ("qwert"), string ("qwert"));
   compare (string ("zxcvb"), string ("asdfg"));
   return 0;
}

//TEST// testxless >testxless.out 2>&1
//TEST// mkpspdf testxless.ps testxless.cpp* testxless.out

