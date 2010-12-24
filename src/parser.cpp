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
   // TODO: raise error

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
      default:
         // TODO: raise error
   }

   Node *node1 = circuit.getNodeById(n1), *node2 = circuit.getNodeById(n2);
   if(!n1) node1->setConnect(node2, element);
   if(!n2) node2->setConnect(node1, element);
}


void Parser::getGm(const string& line) {
   isstream sin(line);
   string name;
   unsigned n1, n2, n3, n4;
   double value;
   sin >> name >> n1 >> n2 >> n3 >> n4 >> value;
   // TODO: raise error

   Element *element = new VCCS(name, value);
   Element *rev_element = new VCCS("R" + name, -value);
   Node *node1 = circuit.getNodeById(n1), *node2 = circuit.getNodeById(n2);
   Node *node3 = circuit.getNodeById(n3), *node4 = circuit.getNodeById(n4);
   // FIXME: this is kind of strange...
   // maybe we need a new element for every link for memory deletion
   node3->setConnect(node2, element);
   node3->setConnect(node1, rev_element);
   if(!n4) {
      node4->setConnect(node1, element);
      node4->setConnect(node2, rev_element);
   }
}


void Parser::getPreset(const string& line) {
   isstream sin(line);
   string name;
   unsigned n1, n2;
   sin >> name >> n1 >> n2;

   switch(name[0]) {
      case 'O':
         circuit.output_high = circuit.getNodeById(n1);
         circuit.output_low = circuit.getNodeById(n2);
         break;
      case 'V':
         circuit.input_high = circuit.getNodeById(n1);
         circuit.input_low = circuit.getNodeById(n2);
         circuit.input_type = VIN;
         break;
      case 'I':
         circuit.input_high = circuit.getNodeById(n1);
         circuit.input_low = circuit.getNodeById(n2);
         circuit.input_type = VIN;
         break;
      default:
         // TODO: error
   }
}

