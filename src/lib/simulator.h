#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "tree.h"
#include <fstream>
using namespace std;

class SimulateConfig {};

class TimeSimulateConfig {

};

class FreqSimulateConfig {

};

class Simulator {
public:
   Simulator(Tree& tree, SimulateConfig& config) ;
   void simulate();
   void plot(ofstream fout) const;
private:
   Tree _tree;
   SimulateConfig _config;
};

#endif /* __SIMULATOR_H__ */

