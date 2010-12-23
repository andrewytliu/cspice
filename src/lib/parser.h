#ifndef __PARSER_H__
#define __PARSER_H__

#include "tree.h"
#include <fstream>
using namespace std;

class Parser {
public:
   Parser(ifstream fin) ;
   Tree& getTree() const { return _tree; }
   SimulateConfig& getConfig() const { return *_config; }
private:
   Tree _tree;
   SimulateConfig* _config;
};

#endif /* __PARSER_H__ */

