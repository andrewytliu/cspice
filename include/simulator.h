#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <fstream>
#include <complex>
#include "circuit.h"

using namespace std;

enum SimulateType {
   TIME,
   FREQ
};

class SimulateConfig {
public:
   SimulateType type;
   double start, end, step;
   string filename;
   string srcName ;
};

class Simulator {
public:
   class TransferFunction {
      public:
         TransferFunction() : num() , den() { }
         TransferFunction(const TransferFunction& t) : num(t.num) , den(t.den) { }
         vector<double> num ;
         vector<double> den ;

         TransferFunction & operator= (const TransferFunction& tf) {
            this->num = tf.num ;
            this->den = tf.den ;
            return *this ;
         }
   };
   Simulator(Circuit * circuit, ofstream& f) ;
   void simulate(SimulateConfig& config);
private:

   void plotFreq(const vector<pair<double,complex<double> > >& points, const SimulateConfig& config);
   void plotTime(const vector<pair<double,double> >& points, const SimulateConfig& config);
   void findFormula(const Source * , TransferFunction&) ;

   Circuit * _circuit;
   ofstream& _fout;
   map<const Source *, TransferFunction> transferFunctions ;
};

class SimulateException {
   private:
      string _message ;
   public:
      SimulateException(const string& msg) : _message(msg) { }
      const char * message() {
         return _message.c_str() ;
      }
} ;

#endif /* __SIMULATOR_H__ */

