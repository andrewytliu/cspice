#ifndef __PARSER_H__
#define __PARSER_H__

#include "tree.h"
#include "simulator.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
   Parser(ifstream& fin) ;
   const Tree& getTree() const { return tree; }
   const vector<SimulateConfig>& getConfig() const { return config; }
   Tree& getTree() { return tree; }
   vector<SimulateConfig>& getConfig() { return config; }

private:
   void getRLC(ifstream& fin, string& name);
   void getGm(ifstream& fin);
   void getPreset(ifstream& fin, string& name);

   Tree tree;
   vector<SimulateConfig> config;
};

#endif /* __PARSER_H__ */

