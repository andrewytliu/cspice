#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "tree.h"
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
   Simulator(Tree& tree) ;
   void simulate(SimulateConfig& config);
   void plot(ofstream& fout) const;
private:
   Tree tree;
};

#endif /* __SIMULATOR_H__ */

