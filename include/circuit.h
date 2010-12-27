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
   UNDEFINE = -1,
   VIN = 0,
   IIN
};

class Circuit {
friend class Parser;
public:
   Circuit() : nodes() , idMap() , inputHighId(0) , inputLowId(0) ,
      outputHighId(0) , outputLowId(0) , inputType(UNDEFINE) { }

   ~Circuit() ;

   Node * getNodeById(unsigned id) ; // consturct new node if not exist

   bool  checkCircuit() ;            // check floating node by DFS from GND

   // list all the spanning tree of the current circuit.
   vector<vector<SmartPtr<Element> > > enumTree(unsigned refNodeId) ;

   Node * getInputHigh () { return getNodeById(inputHighId) ; }
   Node * getOutputHigh() { return getNodeById(outputHighId); }
   Node * getInputLow  () { return getNodeById(inputLowId)  ; }
   Node * getOutputLow () { return getNodeById(outputLowId) ; }
   unsigned getInputHighId () { return inputHighId ; }
   unsigned getOutputHighId() { return outputHighId; }
   unsigned getInputLowId  () { return inputLowId  ; }
   unsigned getOutputLowId () { return outputLowId ; }
   InputType getInputType() const {return inputType ;}

   void print() const ;

private:
   void dfs(int, vector<bool>&, vector<vector<bool> >&, vector<SmartPtr<Element> >&, vector<vector<SmartPtr<Element> > >&) ;

   unsigned getIndexById(unsigned id) ; // consturct new node if not exist

   InputType inputType ;
   unsigned inputHighId , inputLowId , outputHighId , outputLowId ;
   map<unsigned, unsigned> idMap ; // mapping node id to index of nodes
   vector<Node *> nodes ;
};

class Node : public SmartObj {
public:
   Node(const unsigned nodeId) : connections() , nodeId(nodeId) { }
   Node(const Node& n) : nodeId(n.nodeId) , connections(n.connections) { }

   void setConnect(const Node * destination,const SmartPtr<Element>& element) ;

   unsigned nodeId ; // used to identify
   vector<Connection> connections ;
};

class Connection {
public:
   Connection(Node* dest, Element* elem) : destination(dest), element(elem) { }
   Connection(const Node * dest, const SmartPtr<Element>& elem) : destination((Node *)dest) , element(elem) { }
   Connection(const Connection& c) : destination(c.destination) , element(c.element) { }
   Connection() : destination(NULL) , element(NULL) { }
   ~Connection() { }

   Node * destination ;
   SmartPtr<Element> element ;
};

#endif /* __CIRCUIT_H__ */

