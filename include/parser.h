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
   void getRLC(const string& line);
   void getGm(const string& line);
   void getPreset(const string& line);

   Tree tree;
   vector<SimulateConfig> config;
};

#endif /* __PARSER_H__ */

