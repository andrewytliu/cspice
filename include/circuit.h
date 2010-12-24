#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "element.h"
#include <vector>
#include <map>
using namespace std;

class Circuit ;
class Node ;
class Connection ;

/**  Class definitions **/

enum InputType {
   VIN,
   IIN
};

class Circuit {
public:
   Circuit() ;                         // should generate GND first
   Node* getNodeById(unsigned id) ; // consturct new node if not exist
   Node* getGnd() { return getNodeById(0); }
   bool  checkCircuit() ;              // check floating node by DFS from GND

   Node* input_high, input_low, output_high, output_low ;
   InputType input_type ;
   vector<Node> nodes ;
   map<unsigned, unsigned> id_map ;

   vector<vector<Element*> > enumTree() ;  // list all the spanning tree of the
                                          //current circuit.
private:
   void dfs(vector<bool>& visited, vector<vector<bool> >& used, vector<Element*> elements) ;
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

#endif /* __CIRCUIT_H__ */

