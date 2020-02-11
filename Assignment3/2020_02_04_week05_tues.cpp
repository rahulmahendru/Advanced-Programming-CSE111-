/*
 Asgn 3 Information:
   Input read from a file or stdin (when no cmd args are provided,
    or we specify - as arg)
   We will be using linear list to implement "listmap", not RB tree
   
   4 operations:
    key =
      // The key is deleted from the map
    key = value
      // Key,val pair added to map. If key already in map, replace val
    =
      // prints full listmap
    = value
      // prints all keys with value
    
   We will impl the class "listmap"
     * Template class where we provide key, mapped_type, and compare
      => Note that compare function defaults to xless
      (To override, look at Code note A)
     * This will be a circularly doubly-linked list, where the head
       acts as a node itself, but doesn't contain a pair
     * We will implement listmap::iterator as well
  #####   -> Look at code note B
      
   look at pkeyvalue.perl for Perl impl of this asgn
*/

/*
// $misc = Assignments/asg3-listmap-templates/misc
// $code = Assignments/asg3-listmap-templates/code
// misc/stl-template-list.cpp:
   -> Look at code note C
// misc/linknode.cpp:
   -> Look at code note D
// misc/catfile.cpp:
   -> Look at this file to figure out file input/output
   -> Look at code note E
// misc/regex.cpp:
   -> Please look at this file; it's very important for asg3!!!
   -> Please please please take this program and play with it.
      -> Meaning, please try out different inputs
   -> Look at code note F
// misc/sortedlist.cpp:
   -> look at code note G
// misc/xlist.h:
   -> Look at code note H
   -> You will mimic something like this for asg3
// code/listmap.h (and all the other files):
   -> Look at code note I
*/

// Code notes:
{
   // Code note A
   map<string, string, greater<string>> myMap;
   // this will be a map sorted in reverse lex order
   
   // code note B
   for (i: c) f(i);
   // translates into:
   for (p = c.begin(); p != c.end(); ++p) f(*p);
   // note: p needs the following:
   //  -> begin() and end()
   // note: p::iterator needs:
   //  -> operator++ and operator*
   
   //-----------------------------------------------------
   
   // code note C:
   list<int> li {1, 2, 3, 4, 5};
   &*li.cbegin(); // this points at a value WITHIN the list, which is
                  // in the heap!!!
   &*li.cend();   // Likewise, but this is on the STACK! It's within
                  // the list!
   
   // code note D:
   struct node;
   struct link {
      node *prev{};
      node *next{};
   };
   struct node: link {
      int value {};
      // note: node will also contain "prev" and "next"
   }
   for (unsigned i = 0; i < 8; ++i) {
      char *ptr = new char;
      cout << static_cast<void*>(ptr) << endl;
      // note, even though we alloc just 1 byte, we are wastine 15
      //  bytes BUT also! We are requiring 16 additional bytes for a
      //  boundary tag to determine whether the node prev is free or
      //  not (Note: we also check if the tag next is free or not)
      // https://www.codeguru.com/images/article/14435/AppMem09.gif
      //
      // Total byte requirement = roundup(16, sizeof(mem alloced)) + 16
   }

   // code note E:
   vector<string> filenames (&argv[1], &argv[argc]); // vector of args
   strerr(errno); // line 51: print out what errno refers to
   const static string colons (32, ':'); // creates variable that only
                                         // gets created ONCE
   while (1) {
      getline(cin, line);
      if (cin.eof())
         break;
      cout << line << endl;
   } // works as well with file istream (ifstream)
   
   // Code note F:
   regex comment_regex {R"(^\s*(#.*)?$)"};
   // \s: matches any whitespace character
   // \S: anything EXCEPT whitespace
   // \d: Digit
   // \D: Anythin except digit
   // \w: Word character
   // \W: Anything BUT a word character
   //
   // \w* matches any amount of whitespace
   // (#.*)? Matches a hash up until the end of the line
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*$)"};
   // (.*?)\s*= will match a key, meaning that it expects some key
   //     preceeded by any amount of whitespace til the equal sign
   regex trimmed_regex { /**/ };
   // matches any line with no starting '#' that has no equal sign
   //
   // within for (;;) loop:
   /*
      When we have a (.*?), we will trim what matches (.*?) and push it
       into "result"
   */
   
   // Code note G:
   template <typename Type>
   struct xless { // It's "like a function" with two arguments
      bool operator() (const Type& left, const Type& right) const {
         return left < right;
      }
   };
   // f() => f.operator()()
   // g(x, y) => g.operator()(x, y)
   // We could also have a case-blind comparator:
   struct cbless {
      // make the strings case-blind by turning upper to lower case
   };
   template <typename item_t, class less_t=xless<item_t>>
   void process(/**/) {
      // This is a template argument where, if we don't override
      //  "less_t", it'll default to "xless<item_t>"
   }
   template <typename item_t, class less_t=xless<item_t>>
   struct sorted_list { // this ought to be a class, not a struct...
      // Professor's "standard linear linked list"
      // ...
      sorted_list() = default;
      sorted_lsit (const sorted_list&) = delete;
      sorted_list& operator= (const sorted_list&) = delete;
      // we don't want to do shallow copying
   }
   // example template member function:
   template <typename item_t, class less_t>
   sorted_list::~sorted_list() {
      while (head != nullptr) pop_front();
   }
   template <typename item_t, class less_t>
   sorted_list::insert(const item_t& newitem) {
      node ** curr = &head;
      while (/*curr hasn't gone off the end, and the pointed-at elem is
              less than the item we're inserting (using less)*/) {
         curr = &(*curr)->link;
      }
      if (/*we've gone off the end, or the pointed-at isn't equal*/) {
         *curr = new node(newitem, *curr);
      }
   } // note, this is a 1x linked list; asg3 is a 2x linked list

   // Code note H:
   // Look at "node" and "link" in Note-D
   /*
      * "end" points at the link prior to the 1st element
      * The last valid elem points at "end"
      **** An empty xlist: the "anchor" prev and next will point at
           itself
   */
   show_list(...); // will print the list
   show_node(...); // will print a node that you pass in (by addr)
   push_back();
   pop_back(); // these are basic doubly-linked list operations
   
   // Code note I:
   // Template functions (definition + impl) must ALWAYS be in header
   ///  Note: templates are implemented compile-time, versus Java
   ///  ==> Java: Containers for generic objects
   ///  ==> C++: We copy-paste the implementation, replacing the
   ///      template types with the passed-in type
   int main(void) {
      // the current main function in main.cpp is a "load of junk"
      // you will need to fully re-write that function to make your asg
      //  work. This is where you'll iterate over the files
      ///  => To begin, write main() to iterate over each file, then
      ///     write a function that prints out the contents of each
      ///     line-by-line. Once done, then begin working on listmap
   }
   // WARNING: There are non-properly implemented functinos in
   //  listmap.h that will start generating errors when instantiated.
   //  You must fix those functions to make your program compile.
}


