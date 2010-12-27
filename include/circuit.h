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

   // Node* getGnd() { return getNodeById(0); }

   bool  checkCircuit() ;           // check floating node by DFS from GND

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
   //Node *input_high, *input_low, *output_high, *output_low ;
   map<unsigned, unsigned> idMap ; // mapping node id to index of nodes
   vector<Node *> nodes ;
};

class Node : public SmartObj {
public:
   Node(const unsigned nodeId) ;
   Node(const Node& n) : nodeId(n.nodeId) , connections(n.connections) {
      cout << "[" << __func__ << "] nodeId = " << this->nodeId << endl ;
   }

   unsigned nodeId ; // used to identify
   vector<Connection> connections ;

   void setConnect(const Node * destination,const SmartPtr<Element>& element) ;

   ~Node() {
      // do nothing, just log
      cout << "[" << __func__ << "]" << endl ;
      //connections = vector<Connection>() ;
   }
};

class Connection {
public:
   Node * destination ;
   SmartPtr<Element> element ;

   Connection(const Node * , const SmartPtr<Element>&) ;
   Connection(const Connection& c) : destination(c.destination) , element(c.element) {
      cout << "[" << __func__ << " (Copy constructor)]" << endl ;
   }
   Connection() : destination(NULL) , element(NULL) {}
   ~Connection() ;
/*
   Connection& operator=(const Connection& rhs) {
      this->destination = rhs.destination ;
      this->element = rhs.element ;
   }
*/
};

#endif /* __CIRCUIT_H__ */

