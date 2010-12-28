#include <sstream>
#include "parser.h"
#include "element.h"

Parser::Parser(ifstream& fin) {
   char line[1024] ;
   while(true) {
      fin.getline(line , 1024); // this can only use "char *" as argument
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
         case 'F':
         case 'T':
            getConfig(line);
            break;
         case ' ':
         case '\n':
            break;
         default:
            string l = line;
            if(l.size() == 0) break;
            throw ParseError("Invalid token type: "  + l);
            break ;
      }
   }
}

void Parser::getRLC(const string& line) {
   stringstream sin(line , stringstream::in);
   string name;
   unsigned n1, n2;
   double value;
   SmartPtr<Element> element;
   sin >> name >> n1 >> n2 >> value;

   if(sin.fail()) throw ParseError("Format error: " + line);
   if(n1 == n2) throw ParseError("Invalid connection: " + line);

   switch(name[0]) {
      case 'R':
         element = SmartPtr<Element>(new Resistor(name, value));
         break;
      case 'L':
         element = SmartPtr<Element>(new Inductor(name, value));
         break;
      case 'C':
         element = SmartPtr<Element>(new Capacitor(name, value));
         break;
      default:
         throw ParseError("Invalid element" + line);
         break ;
   }

   Node * node1 = circuit.getNodeById(n1);
   Node * node2 = circuit.getNodeById(n2);

   node1->setConnect(node2, element);
   node2->setConnect(node1, element);
}

void Parser::getGm(const string& line) {
   stringstream sin(line , stringstream::in);
   string name;
   unsigned n1, n2, n3, n4;
   double value;
   sin >> name >> n1 >> n2 >> n3 >> n4 >> value;

   if(sin.fail()) throw ParseError("Format error: " + line);
   if(n1 == n3 && n2 == n4) throw ParseError("Invalid connection: " + line);

   SmartPtr<Element> element(new VCCS(name, value , 1)) ;
   SmartPtr<Element> rev_element(new VCCS(name, value , -1));

   Node * node1 = circuit.getNodeById(n1);
   Node * node2 = circuit.getNodeById(n2);
   Node * node3 = circuit.getNodeById(n3);
   Node * node4 = circuit.getNodeById(n4);

   if(n3 != n2) node3->setConnect(node2, element);
   if(n3 != n1) node3->setConnect(node1, rev_element);
   if(n4 != n1) node4->setConnect(node1, element);
   if(n4 != n2) node4->setConnect(node2, rev_element);
}

void Parser::getPreset(const string& line) {
   stringstream sin(line , stringstream::in);
   string name;
   unsigned n1, n2;
   sin >> name >> n1 >> n2;
   if(sin.fail()) throw ParseError("Format error: " + line);

   switch(name[0]) {
      case 'O':
         circuit.outputHighId = n1;
         circuit.outputLowId  = n2;
         break;
      case 'V':
         circuit.inputHighId = n1;
         circuit.inputLowId  = n2;
         circuit.inputType   = VIN;
         break;
      case 'I':
         circuit.inputHighId = n1;
         circuit.inputLowId  = n2;
         circuit.inputType  = IIN;
         break;
      default:
         throw ParseError("Invalid element" + line);
         break ;
   }
}

void Parser::getConfig(const string& line) {
   stringstream sin(line , stringstream::in);
   string type;
   SimulateConfig single;
   sin >> type >> single.start >> single.end >> single.step >> single.filename;
   if(sin.fail()) throw ParseError("Format error: " + line);

   switch(type[0]) {
      case 'F':
         single.type = FREQ;
         break;
      case 'T':
         single.type = TIME;
         break;
      default:
         throw ParseError("Invalid element" + line);
         break ;
   }
   config.push_back(single);
}

