#ifndef __SMARTOBJ_H__
#define __SMARTOBJ_H__
#include <new>
#include <cstdlib>
using namespace std;

// Class for double-free preventing
class SmartObj {
public:
   SmartObj();
   ~SmartObj();

   void * clone();
   void * operator new(size_t size);
   void   operator delete(void* ptr);
protected:
   unsigned _ref;
};

#endif /* __SMARTOBJ_H__ */
