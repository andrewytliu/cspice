#include <iomanip>
#include <iostream>
#include <vector>
#include <complex>
#include <cstdlib>
#include <cmath>
#include "element.h"
#include "utils.h"
using namespace std;

void printFormula(const vector<vector<SmartPtr<Element> > >& collection , ostream& fout) {
   int sizeOfCollection = collection.size() ;

   for(int i = 0 ; i < sizeOfCollection ; ++ i) {
      int amountOfElements = collection[i].size() ;
      string line = "" ;
      char sign = 1 ;
      for(int j = 0 ; j < amountOfElements ; ++ j) {
         if (collection[i][j]->type() != "Dummy") {
            line += "(" + collection[i][j]->formula() + ") " ;
         }
         sign *= collection[i][j]->sign() ;
      }
      if (sign == 1) {
         if (i > 0)  fout << " + " << line << endl ;
         else        fout << "   " << line << endl ;
      } else {
         fout << " - " << line << endl ;
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
vector<pair<int , double> > expandFormula(const vector<vector<SmartPtr<Element> > >& collection) {
   int sizeOfCollection = collection.size() ;
   map<int ,double> mapping ;
   vector<pair<int , double> > result ;

   for(int i = 0 ; i < sizeOfCollection ; ++ i) {
      double value = 1 ;
      int order = 0;
      int amountOfElements = collection[i].size() ;
      for(int j = 0 ; j < amountOfElements ; ++ j) {
         //if (collection[i][j]->type() != "Dummy") {
            value *= collection[i][j]->value() * collection[i][j]->sign() ;
            order += collection[i][j]->order() ;
         //}
      }
      mapping[order] += value ;
   }

   for(map<int , double>::const_iterator it = mapping.begin() ; it != mapping.end() ; ++ it) {
      if (abs(it->second) > 1.0e-10) { // if the value is too small, ignore it!
         result.push_back((*it)) ;
      }
   }

   return result ;
}

/*
 * coefficients is an array contains a_0, a_1, a_2, ... a_n and this function
 * evaluate sum[a_k * s^k], where s = i * 2 * pi * freq, k = 0 ... n
 */
complex<double> evalFormula(const vector<double>& coefficients,const double& freq) {
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
   return complex<double>(real , image) ;
}

ostream& operator<<(ostream& out , const Element& element) {
   out << setw(5) << left << element._name
      << setw(10) << setprecision(3) << right << scientific << element._value ;

   return out ;
}

unsigned long hash(const char* p) {
   // using sdbm hash algorithm
   unsigned long result = 0;
   const unsigned char * str = (const unsigned char *)p ;
   int c;
   while ((c = (*(str++))))
      result = c + (result << 6) + (result << 16) - result;

   return result;
}

unsigned long hash(const vector<unsigned long>& array) {
   // using FNV-1 hash algorithm
   unsigned long result = 14695981039346656037ul ;
   for (vector<unsigned long>::const_iterator it = array.begin() ; it < array.end() ; ++ it) {
      result *= 1099511628211ul ;
      result ^= (*it) ;
   }

   result *= 1099511628211ul ;
   result ^= array.size() ;
   return result ;
}
