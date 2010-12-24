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
   Tree() ;                         // should generate GND first
   Node* getNodeById(unsigned id) ; // consturct new node if not exist
   Node* getGnd() { return getNodeById(0); }
   vector<Node> nodes ;
   map<unsigned, unsigned> id_map ;
};

class Node {
public:
   unsigned nodeId ; // used to identify
   void setConnect(Node* destination, Element* element) ;
   vector<Connection> connections ;
};

class Connection {
public:
   Node * destination ;
   Element * element ;
};

#endif /* __TREE_H__ */

