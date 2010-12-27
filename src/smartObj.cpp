#include <iostream>
#include "smartObj.h"

using namespace std ;

map<const void* , int> SmartObj::mapping ;

SmartObj::SmartObj() {
   //_ref = 1;
}

SmartObj::~SmartObj() {
   cout << "[" << __func__ << "]" << endl ;
   int amount = SmartObj::getRefAmount(this) ;
   if (amount < 0) {
      // TODO make an exception class for this exception
      cerr << "[" << __func__ << "]" << "Reference handling error! (amount = " << amount << ")" << endl ;
      throw "Error for reference amount handling" ;
   }
}

const SmartObj * SmartObj::clone() const {
   int amount = SmartObj::addRefAmount(this , 1) ;
   cout << "[SmartObj::" << __func__ << "(this = " << this << ")] Now amount increase to " << amount << endl ;
   return this;
}

void* SmartObj::operator new(size_t size) {
   void * p = malloc(size * sizeof(char));
   cout << "Allocate memory size " << size << ", Addr = " << p << endl ;
   SmartObj::setRefAmount(p , 0) ;
   return p ;
}

void SmartObj::operator delete(void* ptr) {
   //SmartObj* obj = (SmartObj*) ptr;
   int amount = SmartObj::getRefAmount(ptr) ;
   //int amount = obj->_ref ;
   if(amount != 0) {
      cerr << "[" << __func__ << "] Reference handling error! (_ref = " << amount << ")" << endl ;
      throw "Reference handling error!" ;
   }
   cout << "[" << __func__ << "] Deleting ptr = " << ptr << endl ;
   free(ptr);
   return;
}

void SmartObj::release() {
   //-- this->_ref;
   int amount = SmartObj::addRefAmount(this ,-1) ;
   cout << "[" << __func__ << " (this = " << this << ")] Now amount decrease to " << amount << endl ;
}

int SmartObj::getRefAmount(const void* ptr) {
   map<const void* , int>::const_iterator it = SmartObj::mapping.find(ptr) ;
   int amount ;
   if (it == SmartObj::mapping.end()) {
      cerr << "ptr = " << ptr << " is not in SmartObj::mapping" << endl ;
      throw "invalid pointer" ;
      // SmartObj::mapping.insert(pair<void* , int>(ptr , amount = 0)) ;
   } else {
      amount = (*it).second ;
   }
   return amount ;
}

int SmartObj::setRefAmount(const void* ptr, int amount) {
   SmartObj::mapping[ptr] = amount ;
   /*
   map<void* , int>::iterator it = SmartObj::mapping.find(ptr) ;
   if (it == SmartObj::mapping.end()) {
      SmartObj::mapping.insert(pair<void* , int>(ptr , amount = 0)) ;
   } else {
      amount = (*it).second ;
   }
   */
   return amount ;
}

int SmartObj::addRefAmount(const void* ptr, int delta) {
   map<const void* , int>::iterator it = SmartObj::mapping.find(ptr) ;
   int amount ;
   if (it == SmartObj::mapping.end()) {
      cerr << "ptr = " << ptr << " is not in SmartObj::mapping" << endl ;
      throw "invalid pointer" ;
      // SmartObj::mapping.insert(pair<void* , int>(ptr , amount = 0)) ;
   } else {
      amount = ((*it).second += delta) ;
   }
   return amount ;
}
