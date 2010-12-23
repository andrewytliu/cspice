#ifndef __TREE_H__
#define __TREE_H__

#include "element.h"
#include <vector>
using namespace std;

class Tree ;
class Node ;
class Connection ;

/**  Class definitions **/

class Tree {
public:
   vector<Node> nodes ;
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

