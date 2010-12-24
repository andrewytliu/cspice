#include "parser.h"
#include "simulator.h"
#include <iostream>
#include <fstream>
using namespace std;

int main (int argc, char const* argv[])
{
   if(argc != 3) {
      cerr << "USAGE: cspice [NETLIST] [OUTPUT]" << endl;
      return 0;
   }
   ifstream fin(argv[1]);
   ofstream fout(argv[2]);
   Parser parser(fin);
   Simulator simulator(parser.getCircuit(), parser.getConfig());
   simulator.plot(fout);
   return 0;
}

