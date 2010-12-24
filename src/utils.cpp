#include <iostream>
#include <vector>
#include "element.h"
#include <complex>
using namespace std;

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

complex<double , double> evalFormula(vector<vector<Element*> > collection , double freq) {
   double omega = 2.0 * acos(-1.0) * freq ; // pi = acos(-1.0)
   double image = 0.0 , real = 0.0 ;

   int sizeOfCollection = collection.size() ;

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

      if(order >= 0) {
         for(int j = 0 ; j < order ; ++ j) {
            value *= omega ;
         }
         order &= 3 ;
      } else {
         for(int j = 0 ; j < (-order) ; ++ j) {
            value /= omega ;
         }
         order = 3 ^ (order & 3) ;
      }
      // i^0 = 1, i^1 = i, i^2 = -1, i^3 = -i
      switch((order&3)) {
         case 0:
            real  += value ;  break ;
         case 1:
            image += value ;  break ;
         case 2:
            real  -= value ;  break ;
         case 3:
            image -= value ;  break ;
      }
   }
   return complex<double , double>(real , image) ;
}
