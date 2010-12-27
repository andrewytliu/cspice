#include <iostream>
#include <fstream>
#include "parser.h"
#include "simulator.h"

using namespace std;

int main (int argc, char const* argv[])
{
   {
   //try {
      if(argc != 3) {
         cerr << "USAGE: cspice [NETLIST] [OUTPUT]" << endl;
         return 0;
      }
      ifstream fin(argv[1]);
      ofstream fout(argv[2]);
      Parser parser(fin);
      parser.getCircuit().print() ;
      Simulator simulator(&parser.getCircuit());
      for(int i = 0 , size = parser.getConfig().size() ; i < size ; ++ i) {
         simulator.simulate(parser.getConfig()[i]);
      }
   }

   SmartObj::print() ;
   return 0;
}

