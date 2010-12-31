#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#define __ELIMINATION__
#include <vector>
#include <map>
#include "element.h"
#include "source.h"

using namespace std;

class Circuit ;
class Node ;
class Connection ;
class Equivalent ;

/**  Class definitions **/

enum InputType {
   UNDEFINE = -1,
   VIN = 0,
   IIN
};

class Circuit {
friend class Parser;
friend class Simulator;
public:
   Circuit() :
      outputHighId(0) , outputLowId(0) , idMap() , nodes() { }

   ~Circuit() ;

   Node * getNodeById(unsigned id) ; // consturct new node if not exist
   void pushElement(Element* elem) { elements.push_back(elem); }
   void pushSource(Source * src) { sources.push_back(src); }

   bool  checkCircuit() ;            // check floating node by DFS from GND

   // list all the spanning tree of the current circuit.
   vector<vector<const Element*> > enumTree(const Node *) ;

   Node * getOutputHigh() { return getNodeById(outputHighId); }
   Node * getOutputLow () { return getNodeById(outputLowId) ; }
   unsigned getOutputHighId() { return outputHighId; }
   unsigned getOutputLowId () { return outputLowId ; }

   vector<const Source*> getSources() const {
      vector<const Source*> clone ;
      int size = sources.size() ;
      for(int i = 0 ; i < size ; ++ i) {
         clone.push_back(sources[i]) ;
      }
      return clone ;
   }

   const Source * getSourceByName(const char * srcName) const {
      int size = sources.size() ;
      for(int i = 0 ; i < size ; ++ i) {
         if(sources[i]->name() == srcName) {
            return sources[i] ;
         }
      }
      return NULL ;
   }

   void print() const ;
private:
   void propagateEquivalents(unsigned,vector<bool>&) ;

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

   unsigned outputHighId , outputLowId ;
   map<unsigned, unsigned> idMap ; // mapping node id to index of nodes
   vector<Node *> nodes ;
   vector<Element *> elements ;
   vector<Source *> sources ;

   mutable const Source * currentSource ; // used to point the source currently turns on.
};

class Node {
public:
   Node(const unsigned nodeId) :
      nodeId(nodeId) , connections() , equivalents() { }

   Node(const Node& n) : // copy constructor
      nodeId(n.nodeId) , connections(n.connections) ,
      equivalents(n.equivalents) { }

   void setConnect(const Node * destination,const Element* element) ;
   void addEquivalent(const Node * node , const VoltageSource * reason) ;

   unsigned nodeId ; // used to identify
   vector<Connection> connections ;
   vector<Equivalent> equivalents ;
};

class Equivalent {
   public:
      Equivalent(const Node * n = NULL , const VoltageSource * r = NULL) :
         node(n) , reason(r) { }
      Equivalent(const Equivalent& e) :
         node(e.node) , reason(e.reason) { }
      ~Equivalent() { }

      const Node * node ;
      const VoltageSource * reason;
      // two node would be short together only if they are connected by a
      // voltage source
};

class Connection {
public:
   Connection(const Node * dest = NULL, const Element* elem = NULL) :
      destination(dest) , element(elem) { }
   Connection(const Connection& c) :
      destination(c.destination) , element(c.element) { }
   ~Connection() { }

   const Node * destination ;
   const Element* element ;
};

#endif /* __CIRCUIT_H__ */

