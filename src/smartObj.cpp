#include "smartObj.h"

SmartObj::SmartObj() {
   _ref = 0;
}

SmartObj::~SmartObj() {
   if (this->_ref <= 0) {
      // TODO make an exception class for this exception
      throw "Error for reference amount handling" ;
   }
}

void * SmartObj::clone() {
   ++_ref;
   return this;
}

void* SmartObj::operator new(size_t size) {
   return malloc(size * sizeof(size_t));
}

void SmartObj::operator delete(void* ptr) {
   SmartObj* obj = (SmartObj*) ptr;
   -- obj->_ref;
   if(obj->_ref > 0) return;
   free(ptr);
   return;
}

