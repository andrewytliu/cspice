#ifndef __PARSER_H__
#define __PARSER_H__

#include "tree.h"
#include <fstream>
using namespace std;

class Parser {
public:
   Parser(ifstream fin) ;
   Tree& getTree() const { return _tree; }
private:
   Tree _tree;
};

#endif /* __PARSER_H__ */

