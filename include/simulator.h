#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <fstream>
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
};

class Simulator {
public:
   Simulator(Circuit * circuit) ;
   void simulate(SimulateConfig& config);
   void plot(ofstream& fout) const;
private:
   void findFormula(vector<double>& num, vector<double>& den) ;
   Circuit * _circuit;
};

#endif /* __SIMULATOR_H__ */

