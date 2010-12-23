#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "tree.h"

class Simulator {
public:
   Simulator(Tree& tree) ;
private:
   Tree _tree;
};

class TimeSimulator : public Simulator {
public:
   void simulate() ;
   void plot() ;
};

class FreqSimulator : public Simulator {
public:
   void simulate() ;
   void plot() ;
};

#endif /* __SIMULATOR_H__ */

