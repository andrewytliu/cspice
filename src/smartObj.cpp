#include <iostream>
#include "smartObj.h"

using namespace std ;

map<const void* , int> SmartObj::mapping ;

SmartObj::SmartObj() {
   //_ref = 1;
}

SmartObj::~SmartObj() {
   int amount = SmartObj::getRefAmount(this) ;
   if (amount < 0) {
      // TODO make an exception class for this exception
      cerr << "[" << __func__ << "]" << "Reference handling error! (amount = " << amount << ")" << endl ;
      throw "Error for reference amount handling" ;
   }
}

const SmartObj * SmartObj::clone() const {
   int amount = SmartObj::addRefAmount(this , 1) ;
   return this;
}

void* SmartObj::operator new(size_t size) {
   void * p = malloc(size * sizeof(char));
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
   /* first, remove ptr from mapping */
   SmartObj::mapping.erase(ptr) ;
   free(ptr);
   return;
}

void SmartObj::release() {
   //-- this->_ref;
   int amount = SmartObj::addRefAmount(this ,-1) ;
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

void SmartObj::print() {
   cout << "========= SmartObj::print() =========" << endl ;
   for(map<const void* , int>::const_iterator it = SmartObj::mapping.begin() ;
         it != SmartObj::mapping.end() ; it ++) {
      cout << (*it).first << "\t => \t" << (*it).second << endl ;
   }
}

