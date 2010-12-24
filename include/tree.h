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

enum InputType {
   VIN,
   IIN
};

class Tree {
public:
   Tree() ;                         // should generate GND first
   Node* getNodeById(unsigned id) ; // consturct new node if not exist
   Node* getGnd() { return getNodeById(0); }
   bool  checkTree() ;              // check floating node by DFS from GND

   Node* input_high, input_low, output_high, output_low ;
   InputType input_type ;
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

