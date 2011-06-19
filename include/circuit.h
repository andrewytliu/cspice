#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

//#define __ELIMINATION__
#include <vector>
#include <map>
#include <utility>
#include <set>
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
#ifdef __PARALLEL__
friend void* processTask(void*);
friend class PrimState;
#endif
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

#ifdef __PARALLEL__
//   void* processTask(void *arg);
   void enumParallel(int size,vector<bool>& visited,
                     vector<const Element*>& current_tree,
                     vector<vector<const Element*> >& result
                     #ifdef __ELIMINATION__
                     ,vector<pair<char , unsigned long long> >& trees
                     #endif
                     );
#endif

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

#ifdef __PARALLEL__
class PrimState {
   public:
      /*** members ***/
      int size; // number of nodes
      vector<bool> visited; // whether a node is connected to refnode
      vector<const Element*> current_tree; // current tree edge elements
      vector<vector<const Element*> > *result; // pointer to solution vector
      #ifdef __ELIMINATION__
      vector<pair<char, unsigned long long> >* trees;
      #endif
      Circuit* circuit; // original circuit
      vector<int> startFrom; // next valid edge for v is from id
//      set<pair<int,int> > used;
      /*** functions ****/
      PrimState() {}
      PrimState(int si, vector<bool> vi, vector<const Element*> ci,
                vector<vector<const Element*> > *ri, 
                #ifdef __ELIMINATION__
                vector<pair<char, unsigned long long> >* ti,
                #endif
                Circuit* cir):
                size(si), visited(vi), current_tree(ci), result(ri),
                #ifdef __ELIMINATION__
                trees(ti),
                #endif
                circuit(cir) {
                   startFrom.resize(size,0);
//                   used.clear();
                }
      PrimState shrink(int u, const Element* e) {
         PrimState ret = *this;
         ret.visited[u]=1;
         ret.current_tree.push_back(e);
         ret.circuit->propagateEquivalents(u,visited);
         return ret;
      }
};
#endif

#endif /* __CIRCUIT_H__ */

