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
   Node* getNodeById(unsigned id) { // consturct new node if not exist
      return & nodes[getIndexById(id)] ;
   }

   Node* getGnd() { return getNodeById(0); }
   bool  checkCircuit() ;           // check floating node by DFS from GND
   vector<vector<Element*> > enumTree(unsigned refNodeId) ; // list all the spanning tree of the
                                          //current circuit.
   Node * getInputHigh () { return getNodeById(inputHighId) ; }
   Node * getOutputHigh() { return getNodeById(outputHighId); }
   Node * getInputLow  () { return getNodeById(inputLowId)  ; }
   Node * getOutputLow () { return getNodeById(outputLowId) ; }
   unsigned getInputHighId () { return inputHighId ; }
   unsigned getOutputHighId() { return outputHighId; }
   unsigned getInputLowId  () { return inputLowId  ; }
   unsigned getOutputLowId () { return outputLowId ; }
   InputType getInputType() const {return inputType ;}

   void print(); // for debug purpose

private:
   void dfs(int, vector<bool>&, vector<vector<bool> >&, vector<Element*>&, vector<vector<Element*> >&) ;

   unsigned getIndexById(unsigned id) ;

   InputType inputType ;
   unsigned inputHighId , inputLowId , outputHighId , outputLowId ;
   //Node *input_high, *input_low, *output_high, *output_low ;
   map<unsigned, unsigned> idMap ; // mapping node id to index of nodes
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

