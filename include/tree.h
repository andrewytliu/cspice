#ifndef __TREE_H__
#define __TREE_H__

#include "element.h"
#include <vector>
#include <map>
using namespace std;

class Tree ;
class Node ;
class Connection ;

/**  Class definitions **/

class Tree {
public:
   Node& getNodeById(unsigned id) ;
   vector<Node> nodes ;
   map<unsigned, unsigned> id_map ;
};

class Node {
public:
   unsigned nodeId ; // used to identify
   vector<Connection> connections ;
};

class Connection {
public:
   Node * destination ;
   Element * element ;
};

#endif /* __TREE_H__ */

