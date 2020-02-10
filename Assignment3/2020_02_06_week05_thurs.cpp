/*
$code=Assignments/asg3-listmap-templates/code/
$misc=Assignments/asg3-listmap-templates/misc/
$notes=Examples/wk04c-templates/
// misc/matchlines.cpp:
   Take the code from here and copy it over to:
// code/main.cpp:
   But... make sure to make a dedicated function to copy the for loop to
// code/listmap.h:
   (Code example A)
// code/listmap.tcc:
   (Code example B)
// notes/pointer.h:
   (Code example C)
// notes/linked_tstack.cpp
   (Code example D)
// notes/testbool.cpp
   (Code example E)
// notes/xvector.h
   (Code example F)
*/

{
   // Code example A:
   template <class less_t=xless<key_t>>;
      // specifies default template param
   struct foo {};
   sizeof(foo); // equals 1
   // BUT, if we do:
   struct bar {
      int x;
      foo p;
   }; // takes 4 bytes
   // Thus:
   class listmap {
      // ...
      less_t less; // takes zero bytes!
      // ...
   };
   // A pointer to a node is the same as a pointer to a link
   //   --> Except, a link has no valid "value"
   // Note, whenever you initialize an object from a class (B) derived
   //  from another class (A), you will call the A constructor first
   //  before calling B's constructor.
   /* class listmap::iterator:
      * We want to point at nodes within the list
      * When the iterator points at the listmap object (the anchor),
         then that pointer is invalid
      *
    */
   // listmap copiers/movers: Strictly speaking, we don't need the
   //  movers, but we need the copiers. If we define the copiers, then
   //  the movers do not get automatically instantiated
   // WARNING: Classes that are nested within one another are not
   //  automatically friended to each other. You must explicitly state
   //  that they are "friends" of each other.
   class iterator {
      private:
         iterator(node *where_): where(where_) {};
      public:
         iterator() {};
   }; // note, we'd like to be able to say externally:
   iterator myIterator;
   // then say something like:
   myIterator = list.begin();
   *iter = x; // iter is non-const, used as input iterator
   x = *iter; // iter is const, used as output iterator
   // Syntactic sugar:
   ptr->value; // is the same as:
   (*ptr).value;
   a[i]; // is the same as:
   *(a + i);
   // same goes for the colon for-loop: makes writing code easier
   /* === TYPES OF ITERATORS: ===
      Input iterator
      Output iterator
      Forward iterator (the same as the above two)
         // all above 3 have: iterator::operator++
      Bidirectional iterator (same as above + bidirectional access)
         // Also has iterator::operator--
      Random access iterator (same as above, but assumes array impl)
   */
   /*
      operator++:
      // Prefix: foo x = ++val;
         foo::operator++(foo &x);
      // Postfix: foo x = val++;
         foo::operator++(foo &x, int); // requires shadow argument
   */
   #include "listmap.tcc" // makes header file smaller and easier to
             // read. Who really wants to have giant monolithic files???
   
   // Code example B:
   // For the destructor, do:
   listmap::~listmap{
      while (not empty()) {
         erase(begin());
      }
   }
   listmap::insert(const item_t &newItem) {
      node *curr = some_function(...);
      // Be sure to use lese(...); to compare KEYS
      if (less(curr->val.first, newItem.first)) {
         // ... do something
      } else if (less(/*reverse the above order*/) {
         // do something else
      } else {
         // the two values are equal
         // IF A NODE HAS A KEY WITH THE SAME VALUE AS YOU'RE INSERTING,
         //  DO NOT ADD A NEW ELEM TO LIST, UPDATE THE KEY'S VALUE!!!
      }
   }
   listmap::find(const key_t &key) {
      // Very simple... just iterate down the list and find the node
      //  with the matching key using less(...) as describing above
   }
   listmap::erase(iterator it) {
      // Should be trivial to implement, see misc/ folder if confused
   }
   
   // Code example C:
   pointer<T> ptr = pointer(new T(stuff...)); // we need a ctor to catch
                                   // this. But... we _should_ be using:
   pointer<T> ptr = make_pointer<T>(stuff...);
   // Smart pointers: reference counting:
   template <typename T>
   class pointer {
      private:
         unsigned long *refCount {}; // must have a ref counter to track
                                     // usage of the smart pointer.
         T *_ptr {};
      public:
         pointer() {}   // or else this will be surpressed
         pointer(T *ptr) {
            if (ptr != nullptr) {
               _ptr = ptr;
               refCount = new unsigned long;
               ++(*refCount);
            }
         }
         pointer(nullptr_t) {} // only possible value of nullptr_t = 0
            // invoked if this is called: pointer<T> = nullptr;
         pointer& operator=(const pointer& pptr) {
            // some stuff
         }
         T& operator*() {
            // should return REFERENCE to pointed-at object
         }
         T* operator->() {
            // should return ADDRESS to pointed-at object
         }
         operator bool() const { return _ptr };
            // I want to be able to say: "if (special_ptr) { }"
         bool operator== (nullptr_t) const { /**/ }
            // Likewise, I want to say "if (special_ptr == nullptr) { }"
         /* Please see file to see other useful boolean comparators */
         // ...
   };
   template <typename T>
   ostream operator<< (ostream& out, const pointer<T>& that) {
      // we need to friend this within the class pointer { }; defn
   }
   template <typename Type, typename... Args>
   inline pointer<type> make_pointer (Args&&... args) {
      // We can have ANY number of argument, so that we may use any 
      //  default constructor that we want.
   }
   
   // Code example D:
   // "Deleter" specifies how to delete the object of type "Type"
   template </*...*/, class Deleter = deleter<Type>>
   class linked_tstack {
      private:
         // ...
         // Pre C++11, you'd have to put non-wanted ctors in private:
         linked_tstack(const linked_tstack&) = delete;
            // but, now you could just use the delete keyword! OwO
      public:
         linked_tstack() = default;
         // ...
         pop() {
            node *tmp = top_;
            top_ = top->link;
            Deleter() (tmp->item); /* We are using a special Deleter */
            delete tmp;
            // ...
         }
   };

   // Code example E:
   // sieve: mechanism to find prime numbers up to a certain value.
   //    => Sieve of Eratosthenes: Some Greek dude from modern-day Libya
   //        that invented a mechanism to find prime numbers
   // What we need to have is an "xvector":
   using dvector = xvector<double>;
   using matrix_vec = xvector<dvector>; // allows 2d vector
   
   // Code example F:
   template <typename Type>
   class xvector_base {
         friend class xvector<Type>;
      private:
         allocator<Type> alloc;
         Type *begin {nullptr};
         Type *end {nullptr};
         Type *limit {nullptr}; // enables efficient runtime of the
                                // functions: begin() and end()
         // Note1: We make begin(), end(), and some other functions
         //  "inline", such that it incurs less runtime overhead when
         //  "calling" them (versus doing a save and jump set of insn)
         // BECAUSE: When you do a regular function call, you must do
         //  a whole bunch of stack stuff, set some variables, save some
         //  other variables, then you can make the jump. Versus
         //  inlining, which skips all this junk.
         // ...
         // ...
         // Note2: when we initially create a vector, it has an initial
         //  capacity of unused elements (usually of cap = 4), we then
         //  fill those fields until we run out of capacity, in which
         //  we resize the array.
         // ...
         // RESIZING: We have to allocate a new array of double the
         //  size, then we copy element-by-element to the new array.
   }; // Note: these containers allow for custom memory management
      // functions, such as when you have a system with no heap memory.
   class xvector {
      public:
         // ...
      private:
         // xvector HAS A xvector_base; we are NOT doing inheritance!!!
         static constexpr size_type MIN_RESERVE = 4;
         xvector_base<value_type> base {};
         // ...
      public:
         // ...
         xvector(): base() {}
         // ..
         ~xvector() { resize(0); } // will dealloc elems
         explicit xvector (initializer_list<value_type> ilist);
         // I want to be able to say:
         // xvector<int> vec = {1, 2, 3, 4, 5, 6};
         size_type size() const { return base.end - base.begin; }
         // BE WARNED, if base.end = 0x7f80, and base.begin = 0x7f00,
         //  the difference = 0x80 = d128, but if we're storing ints,
         //  then we'll return 32.
         void push_back(const value_type&); // uses copy ctor
         void push_back(value_type&&);      // uses move ctor
         // ...
         // Iterators:
         iterator begin() { /**/ }
         const_iterator begin() const { /**/ }
         const_iterator cbegin() const { /**/ }
         // then same for end(), end() const, and cend() const
         // Depending on if the parent's container is const or not,
         //  if we call begin() and end(), it'll dispatch to the correct
         //  (const or non-const) function.
         reference operator[] (size_type pos) { /**/ }
         const_reference operator[] (size_type pos) const {/**/}
         // depending on if you do: c[i] = x, or x = c[i]
         reference back() {return base.end[-1];}
         // since the iterator is RANDOM ACCESS, you can subscript it!
         // Note, we could have had an "emplace_back", which constructs
         //  the item within the member function itself, rather than
         //  having to move the item ourselves.
   };
   xvector<int> vec(10);   // initialize with 10 elems of default value
   xvector<int> vec2(10, 20); // same, but value = 20
   // But, why worry about initial vector size? We can just use
   //  push_back(). But, if we know in advance the vector size, we can
   //  initialize the vector with a certain (initial) size.
   // to see real vector implementation, see (if you dare):
   //  /opt/rh/devtoolset-8/root/usr/include/c++/8/bits/vector.tcc
   vector<string> myStrVec;
   myStrVec.push_back("abc"); // translates into:
   // myStrVec.push_back( std::move(string("abc")) );
   ///
   /// Reminder: all boolean ops can derive from a single "operator<"
   ///
   class xvector_iterator {
      // ...
   }
   
   // Code example G:
}

