#include "parser.h"
#include <sstream>

Parser::Parser(ifstream& fin) {
   string line;
   while(true) {
      fin.getline(line);
      if(fin.eof()) break;
      switch(line[0]) {
         case 'R':
         case 'L':
         case 'C':
            getRLC(line);
            break;
         case 'G':
            getGm(line);
            break;
         case 'V':
         case 'I':
         case 'O':
            getPreset(line);
            break;
         default:
            // TODO: should raise error here
      }
   }
}

void Parser::getRLC(const string& line) {
   isstream sin(line);
   string name;
   unsigned n1, n2;
   double value;
   Element *element;
   sin >> name >> n1 >> n2 >> value;

   switch(name[0]) {
      case 'R':
         element = new Resistor(name, value);
         break;
      case 'L':
         element = new Inductor(name, value);
         break;
      case 'C':
         element = new Capacitor(name, value);
         break;
   }

   Node *node1 = tree.getNodeById(n1), *node2 = tree.getNodeById(n2);
   if(!n1) node1->setConnect(node2, element);
   if(!n2) node2->setConnect(node1, element);
}

