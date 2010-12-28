#include <iostream>
#include <fstream>
#include "parser.h"
#include "simulator.h"

using namespace std;

int main (int argc, char const* argv[])
{
   if(argc != 3) {
      cerr << "USAGE: cspice [NETLIST] [OUTPUT]" << endl;
      return 0;
   }
   ifstream fin(argv[1]);
   if(!fin.is_open()) {
      cerr << argv[1] << " cannot be opened!" << endl;
      return 0;
   }
   ofstream fout(argv[2]);
   if(!fout.is_open()) {
      cerr << argv[2] << " cannot be opened!" << endl;
   }

   try{
      Parser parser(fin);
      parser.getCircuit().print() ;
      Simulator simulator(&parser.getCircuit());
      for(int i = 0 , size = parser.getConfig().size() ; i < size ; ++ i) {
         simulator.simulate(parser.getConfig()[i]);
      }
   } catch (ParseError e) {
      cerr << e.message << endl;
   }

   SmartObj::print() ;
   return 0;
}

