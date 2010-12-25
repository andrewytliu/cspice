#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include <vector>
#include <map>
#include "element.h"

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
friend class Parser;
public:
   Circuit() ;                      // should generate GND first
   Node* getNodeById(unsigned id) ; // consturct new node if not exist
   Node* getGnd() { return getNodeById(0); }
   bool  checkCircuit() ;           // check floating node by DFS from GND
   vector<vector<Element*> > enumTree() ;  // list all the spanning tree of the
                                          //current circuit.
private:
   void dfs(int, vector<bool>&, vector<vector<bool> >&, vector<Element*>&, vector<vector<Element*> >&) ;

   InputType input_type ;
   Node *input_high, *input_low, *output_high, *output_low ;
   map<unsigned, unsigned> id_map ;
   vector<Node> nodes ;
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

