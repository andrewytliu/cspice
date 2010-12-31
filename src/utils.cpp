#include <iomanip>
#include <iostream>
#include <vector>
#include <complex>
#include <cstdlib>
#include <cmath>
#include "element.h"
#include "utils.h"
using namespace std;

void printFormula(const vector<vector<const Element*> >& collection , ostream& fout) {
   int sizeOfCollection = collection.size() ;

   for(int i = 0 ; i < sizeOfCollection ; ++ i) {
      int amountOfElements = collection[i].size() ;
      string line = "" ;
      char sign = 1 ;
      for(int j = 0 ; j < amountOfElements ; ++ j) {
         if (collection[i][j]->type() != Element::T_DUMMY) {
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
vector<pair<int , double> > expandFormula(
   const vector<vector<const Element*> >& collection) {
   int sizeOfCollection = collection.size() ;
   map<int ,double> mapping ;
   vector<pair<int , double> > result ;

   for(int i = 0 ; i < sizeOfCollection ; ++ i) {
      double value = 1 ;
      int order = 0;
      int amountOfElements = collection[i].size() ;
      for(int j = 0 ; j < amountOfElements ; ++ j) {
         value *= collection[i][j]->value() * collection[i][j]->sign() ;
         order += collection[i][j]->order() ;
      }
      mapping[order] += value ;
   }

   for(map<int , double>::const_iterator it = mapping.begin() ;
      it != mapping.end() ; ++ it) {
      result.push_back((*it)) ;
   }

   return result ;
}

/*
 * coefficients is an array contains a_0, a_1, a_2, ... a_n and this function
 * evaluate sum[a_k * s^k], where s = i * 2 * pi * freq, k = 0 ... n
 */
complex<double> evalFormula(const vector<double>& coefficients,
   const double& freq) {
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

unsigned long long hash(const char * p) {
   // using FNV-1a hash algorithm
   const unsigned long long prime = 1099511628211ull ;
   unsigned long long result = 14695981039346656037ull ;
   unsigned long long length = 0 ;
   for (const unsigned char * it = (const unsigned char*) p ; *it ; ++ it) {
      result ^= (*it) ;
      result *= prime ;
      ++ length ;
   }

   result ^= length ;
   result *= prime ;
   return result ;
}

vector<double> numericalIntegration(const vector<double>& times , Simulator::TransferFunction& tf , unsigned shift) {
   // shift has a default value of 8

   // this is for unit step response, that is,
   // In[t] = 1.0 for t >= 0
   //
   // for convenient, make Num and Den have same size.
   int N = max(tf.den.size() , tf.num.size()) ;
   vector<double> result ;

   tf.num.resize(N , 0.0) ;
   tf.den.resize(N , 0.0) ;

   double * newU = new double[N] ; // u[k][t + 1]
   double * oldU = new double[N] ; // u[k][t]

   // Initialize
   for (int i = 0 ; i < N ; ++ i) {
      newU[i] = oldU[i] = 0 ;
   }
   oldU[0] = 1.0 / tf.den[N - 1] ;

   result.push_back(0.0) ;
   unsigned size = times.size() ;
   for (unsigned t = 1 ; t < size ; ++ t) {
      double step = (times[t] - times[t - 1]) / (N << shift); // Real time step is smaller than output set
      double DEN = 0 ;

      for (int k = 0 ; k < N ; ++ k) {
         DEN = DEN * (step / 2.0) + tf.den[k] ;
      }

      for(int i = 0 ; i < (N << shift) ; ++ i) {
         double sum1 , sum2 ;

         newU[0] = 0.0 ;
         sum1 = oldU[0] * (step / 2.0) ;
         sum2 = 0.0 ;
         for (int k = 1 ; k < N ; ++ k) {
            sum2 = sum2 * (step / 2.0)  + oldU[k] ;
            newU[0] += tf.den[N - k - 1] * (sum1 + sum2) ;
            sum1 = (step / 2.0) * (sum1 + oldU[k]) ;
         }
         newU[0] = (1.0 - newU[0]) / DEN ;

         for (int k = 1 ; k < N ; ++ k) {
            newU[k] = oldU[k] + (step / 2.0) * (oldU[k - 1] + newU[k - 1]) ;
         }
         for (int k = 0 ; k < N ; ++ k) {
            oldU[k] = newU[k] ;
         }
      }
      double out = 0.0 ;
      for (int k = 0 ; k < N ; ++ k) {
         out += tf.num[N - k - 1] * newU[k] ;
      }

      result.push_back(out) ;
   }

   delete [] newU ;
   return result ;
}
