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
};

class Simulator {
public:
   Simulator(Circuit * circuit, ofstream& f) ;
   void simulate(SimulateConfig& config);
private:
   void plotFreq(const vector<pair<double,complex<double> > >& points, const SimulateConfig& config);
   void plotTime(const vector<pair<double,double> >& points, const SimulateConfig& config);
   void findFormula(vector<double>& num, vector<double>& den) ;

   Circuit * _circuit;
   ofstream& _fout;
};

#endif /* __SIMULATOR_H__ */

