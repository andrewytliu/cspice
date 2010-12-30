#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#define __ELIMINATION__
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
   Circuit() : inputType(UNDEFINE) , inputHighId(0) , inputLowId(0) ,
      outputHighId(0) , outputLowId(0) , idMap() , nodes() { }

   ~Circuit() ;

   Node * getNodeById(unsigned id) ; // consturct new node if not exist
   void pushElement(Element* elem) { elements.push_back(elem); }

   bool  checkCircuit() ;            // check floating node by DFS from GND

   // list all the spanning tree of the current circuit.
   vector<vector<const Element*> > enumTree(const Node *) ;

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

   void dfs(
      int, vector<bool>&,
      vector<vector<bool> >&,
      vector<const Element*>&,
      vector<vector<const Element*> >&
#ifdef __ELIMINATION__
      ,vector<pair<char , unsigned long long> >&
#endif // __ELIMINATION__
   ) ;

   unsigned getIndexById(unsigned id) ; // consturct new node if not exist

   InputType inputType ;
   unsigned inputHighId , inputLowId , outputHighId , outputLowId ;
   map<unsigned, unsigned> idMap ; // mapping node id to index of nodes
   vector<Node *> nodes ;
   vector<Element *> elements ;
};

class Node {
public:
   Node(const unsigned nodeId) : nodeId(nodeId), connections(){ }
   Node(const Node& n) : nodeId(n.nodeId) , connections(n.connections) { }

   void setConnect(const Node * destination,const Element* element) ;

   unsigned nodeId ; // used to identify
   vector<Connection> connections ;
};

class Connection {
public:
   // Connection(Node* dest, Element* elem) : destination(dest), element(elem) { }
   Connection(const Node * dest, const Element* elem) : destination((Node *)dest) , element(elem) { }
   Connection(const Connection& c) : destination(c.destination) , element(c.element) { }
   Connection() : destination(NULL) , element(NULL) { }
   ~Connection() { }

   const Node * destination ;
   const Element* element ;
};

#endif /* __CIRCUIT_H__ */

