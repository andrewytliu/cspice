#include "parser.h"

Parser::Parser(ifstream& fin) {
   string token;
   while(true) {
      fin >> token;
      if(fin.eof()) break;
      switch(token[0]) {
         case 'R':
         case 'L':
         case 'C':
            getRLC(fin, token);
            break;
         case 'G':
            getGm(fin);
            break;
         case 'V':
         case 'I':
         case 'O':
            getPreset(fin, token);
            break;
         default:
            // TODO: should raise error here
      }
   }
}

void Parser::getRLC(ifstream& fin, string& name) {

}

