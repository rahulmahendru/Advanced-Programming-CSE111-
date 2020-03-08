/*
Old-Exams/.solutions/soln-cse111-2020q1-midterm.txt
   * CodeEx A
Assignments/asg5-oop-opengl/code/shape.h
   * CodeEx B
Assignments/asg5-oop-opengl/code/graphics.h
   * CodeEx C
Assignments/asg5-oop-opengl/code/graphics.cpp
   * CodeEx D
Assignments/asg5-oop-opengl/code/interp.h
Assignments/asg5-oop-opengl/code/interp.cpp
   * CodeEx E
Notes/??-threads-??
   * CodeEx F
*/

{
   // Code example A
   /* QUESTION 1: */
   class foo {
      foo();   // default ctor
      ~foo();  // dtor
      foo(const foo&);              // copy ctor
      foo& operator= (const foo&);  // copy assign
      // Move ctor: std::move() to use
      // Move assign
   };
   /* QUESTION 2: */
   /* QUESTION 3: */
   /* QUESTION 4: */
   // make sure to pass the vector by: (1) const, and (2) by reference
   //    Note: were a function to modify the vector, then DON'T set it
   //          as "const"
   // For int-s, when using the colon for-loop, don't retrieve each elem
   //    by reference, since that would be more costly: CPU and mem wise
   /* QUESTION 5: */
   // Be sure to use the 3-part iterator for-loop.
   /* QUESTION 6: */
   // class members (pre and post):
   foo& operator++();
   foo operator++(int /*bogus*/);
   // non-class members:
   foo& operator++(foo&);  // DO NOT PASS IT IN AS CONST REF!!!
   foo operator++(foo&, int /*bogus*/);
   /* QUESTION 7: */
   /*invocation:*/ {
      bigint myB = 25;
      bigint other = -myB; // bigint::operator-() { ... }
   }
   /* QUESTION 8: */
   // We also assume that foo has a copy assign
   /* QUESTION 9: */
   foo operator++(foo& x, int) { // x CANNOT BE CONST!!!
      foo returnVal = x;
      ++x;
      return returnVal;
   }
   /* QUESTION 10: */
   template <typename iterator,
             typename less_t = less_equal<decltype(*iterator())>>
   // decltype: determine @ compile time what iterator "points" at.
   /**/ {
      if (begin == end) return true;   // MANY PEOPLE SHOULD HAVE HAD
                           // THIS CHECK IN THEIR CODE!!!!!
      while (1) {
         auto curr = begin++; // if this was ++begin, it would not work
         // ...
      }
   }
   /* QUESTION 11: */
   /* QUESTION 12: */
   {
      vector<int> someCnt = { /**/ };
      for (auto item: someCnt) cout << item << endl;
      // TRANSLATES INTO:
      for (auto iter = someCnt.begin(); iter != someCnt.end(); ++iter) {
         cout << *iter << endl;
      }
      /**** We used:
         > class container::iterator
         > container::begin()
         > container::end()
         > iterator::operator!=(iterator)
         > iterator::operator++()
         > iterator::operator*()
      */
   }
   /* QUESTION 13: */
   // Note: we could have "back inserters" for containers, which would
   //  call the push_back() function 
   /* QUESTION 14: */
   /* QUESTION 15: */
   // do you remember the iterstack from assignment 1 ???
   /* MULTIPLE CHOICE QUESTIONS:
      * (1): vector<int> is the only std container that uses a C array
             under the hood to store contiquous data elems
      // Review solution page and ask questions on Piazza
   */
   
   // Code example B:
   // See notes from last Thursday: Code example I
   
   // Code example C:
   class object {
      private:
         // ...
      public:
         object(shared_ptr<shape>, vertex, rgbcolor);
         // there will be NO default (no argument) ctor
   };
   class window {
      // static GLOBAL class: there's only one GLOBAL instance!
      // These functions are static, which means that you do NOT call
      //  those functions off of an object.
      // Most of these functions will be the callback functions that
      //  gl will call upon a window event (thus, these static member
      //  functions can be passed into the C-based gl library.
   };
   /**/ {
      window::somefunction();
      // versus saying myWin.somefunction()
      // Window is a SINGLETON class
   }
   
   // Code example D:
   // We can adjust WORLD coordinates, but mouse coords are ALWAYS pixel
   //  coordinates
   // This file should be more-or-less self-explanatory
   
   // Code example E:
   // note, when you start "gdraw", we are prompted for input. Only once
   //  we provide the EOF signal (with Ctrl+D) will we start drawing.
   static some_factory_function(param begin, param end);
   // param will be iterators that point to character arrays.
   /*******************************
      * BE SURE THAT YOU CAN TRACE TRE EXECUTION FLOW/STACK WITHIN
      *  interp.cpp. In section, I can (and will) go over in more detail
      *  what's occurring. You WILL have to modify interp.cpp !!!
   **********************************/
   shape_ptr interpreter::make_*(param begin, param end) {
      // You must implement each of these functions yourself
   }
   
   // Code example F:
   // A process is a fundamental unit of execution in the Unix kernel.
   //  When we perform fork(), we create a second process from the first
   //   that is an exact copy of the first.
   // > fork(), exec(), wait().
   //
   // Multicore CPU: each core can run its own process.
   // Concurrency: "taking turns" on a single CPU.
   //  > Once a process takes enough CPU time, it gets booted off the
   //    CPU to make space for other processes.
   //
   // However, there are also theads: these are lighter weight elements
   //  that exist. Threads belong to processes.
   // "Memory image": Complete memory of a single process that can be
   //  retreived by a pointer within the process.
   // Threads share the same memory image amongst all other sibling
   //  threads within a single process.
   // THIS MEANS: They can trample on each other! In the case of
   //  different processes, they cannot mess each other up!
   //
   // threads: green threads (C/C++ extension library threads)
   //                        (managed by the user program)
   //          kernel threads (coroutines, managed by operating system)
   // kernel threads are HEAVY-WEIGHT threads and allow real parallelism
   //  each thread has their own register and stack
   // green threads are LIGHT-WEIGHT but only allow up to single-core
   //  concurrency
   
   //
}