#ifndef __PARSER_H__
#define __PARSER_H__

#include "circuit.h"
#include "simulator.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
   Parser(ifstream& fin) ;
   const Circuit& getCircuit() const { return circuit; }
   const vector<SimulateConfig>& getConfig() const { return config; }
   Circuit& getCircuit() { return circuit; }
   vector<SimulateConfig>& getConfig() { return config; }

private:
   void getRLC(const string& line);
   void getGm(const string& line);
   void getPreset(const string& line);
   void getConfig(const string& line);

   Circuit circuit;
   vector<SimulateConfig> config;
};

class ParseError {
public:
   ParseError(const string& msg) : message(msg) { }
   string message;
};

#endif /* __PARSER_H__ */

