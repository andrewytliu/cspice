#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "circuit.h"
#include <fstream>
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
   Simulator(Circuit& circuit) ;
   void simulate(SimulateConfig& config);
   void plot(ofstream& fout) const;
private:
   Circuit circuit;
};

#endif /* __SIMULATOR_H__ */

