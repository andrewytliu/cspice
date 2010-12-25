#include <iostream>
#include <vector>
#include <complex>
#include <cmalloc>
#include "element.h"
#include "utils.h"
using namespace std;

SmartObj::SmartObj() {
   _ref = 0;
}

SmartObj::~SmartObj() {
   assert(_ref == 0);
}

SmartObj* SmartObj::clone() {
   ++_ref;
   return this;
}

void* SmartObj::operator new(size_t size) {
   return malloc(size * sizeof(size_t));
}

void SmartObj::operator delete(void* ptr) {
   SmartObj* obj = (SmartObj*) ptr;
   --obj->_ref;
   if(obj->_ref > 0) return;
   free(ptr);
   return;
}

void printFormula(vector<vector<Element*> > collection , ostream& fout) {
   int sizeOfCollection = collection.size() ;

   for(int i = 0 ; i < sizeOfCollection ; ++ i) {
      int amountOfElements = collection[i].size() ;
      for(int j = 0 ; j < amountOfElements ; ++ j) {
         if (collection[i][j]->type() != "Dummy") {
            fout << "(" << collection[i][j]->formula() << ")" ;
         }
      }
      if(i != sizeOfCollection - 1) {
         fout << " +" << endl ;
      } else {
         fout << endl ;
      }
   }
}

/*
 * pair<int , double>: order => coefficient
 * that is, coefficient * s^order
 *
 * need to shift the orders and make them all nonnegative,
 * then one can use evalFormula(coefficients, freq) to evaluate.
 */
vector<pair<int , double> > expandFormula(vector<vector<Element*> > collection) {
   int sizeOfCollection = collection.size() ;
   map<int ,double> mapping ;
   vector<pair<int , double> > result ;

   for(int i = 0 ; i < sizeOfCollection ; ++ i) {
      double value = 1 ;
      int order = 0;
      int amountOfElements = collection[i].size() ;
      for(int j = 0 ; j < amountOfElements ; ++ j) {
         if (collection[i][j]->type() != "Dummy") {
            value *= collection[i][j]->value() ;
            order += collection[i][j]->order() ;
         }
      }
      mapping[order] += value ;
   }

   for(map<int , double>::const_iterator it = mapping.begin() ; it != mapping.end() ; ++ it) {
      result.push_back((*it)) ;
   }

   return result ;
}

/*
 * coefficients is an array contains a_0, a_1, a_2, ... a_n and this function
 * evaluate sum[a_k * s^k], where s = i * 2 * pi * freq, k = 0 ... n
 */
complex<double , double> evalFormula(vector<double> coefficients, double freq) {
   double omega = 2.0 * acos(-1.0) * freq ; // pi = acos(-1.0)
   double image = 0.0 , real = 0.0 ;

   int maxOrder = coefficients.size() ;
   double s = 1 ;

   for(int order = 0 ; order < maxOrder ; ++ order) {
      // i^0 = 1, i^1 = i, i^2 = -1, i^3 = -i
      switch(order & 3) {
         case 0:
            real  += s * coefficients[order] ; break ;
         case 1:
            image += s * coefficients[order] ; break ;
         case 2:
            real  -= s * coefficients[order] ; break ;
         case 3:
            image -= s * coefficients[order] ; break ;
      }
      s *= omega ;
   }
   return complex<double , double>(real , image) ;
}

