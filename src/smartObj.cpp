#include <iostream>
#include <sstream>
#include "smartObj.h"

using namespace std ;

map<const void* , int> SmartObj::mapping ;

SmartObj::SmartObj() { }

SmartObj::~SmartObj() {
   int amount = SmartObj::getRefAmount(this) ;
   if (amount < 0) {
      stringstream msgBuffer ;
      msgBuffer << "Error for reference amount handling (amount = " << amount << ")" ;

      throw SmartObjException(msgBuffer.str()) ;
   }
}

const SmartObj * SmartObj::clone() const {
   SmartObj::addRefAmount(this , 1) ;
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
      stringstream msgBuffer ;
      msgBuffer << "Error for reference amount handling (amount = " << amount << ")" ;

      throw SmartObjException(msgBuffer.str()) ;
   }
   /* first, remove ptr from mapping */
   SmartObj::mapping.erase(ptr) ;
   /* second, release resource */
   free(ptr);
   return;
}

void SmartObj::release() {
   SmartObj::addRefAmount(this ,-1) ;
}

int SmartObj::getRefAmount(const void* ptr) {
   map<const void* , int>::const_iterator it = SmartObj::mapping.find(ptr) ;
   int amount ;
   if (it == SmartObj::mapping.end()) {
      // generate error message
      stringstream msgBuffer ;
      msgBuffer << "Invalid pointer (ptr = " << ptr << " is not in SmartObj::mapping)" ;
      throw SmartObjException(msgBuffer.str()) ;
   } else {
      amount = (*it).second ;
   }
   return amount ;
}

int SmartObj::setRefAmount(const void* ptr, int amount) {
   SmartObj::mapping[ptr] = amount ;
   return amount ;
}

int SmartObj::addRefAmount(const void* ptr, int delta) {
   map<const void* , int>::iterator it = SmartObj::mapping.find(ptr) ;
   int amount ;
   if (it == SmartObj::mapping.end()) {
      // generate error message
      stringstream msgBuffer ;
      msgBuffer << "Invalid pointer (ptr = " << ptr << " is not in SmartObj::mapping)" ;

      throw SmartObjException(msgBuffer.str()) ;
   } else {
      amount = ((*it).second += delta) ;
   }
   return amount ;
}

void SmartObj::print() {
   int amount = 0 ;
   cout << "========= SmartObj::print() =========" << endl ;
   for(map<const void* , int>::const_iterator it = SmartObj::mapping.begin() ;
         it != SmartObj::mapping.end() ; it ++) {
      cout << (*it).first << "\t => \t" << (*it).second << endl ;
      amount ++ ;
   }
   cout << "There are " << amount << " pointers in the mapping" << endl ;
}
