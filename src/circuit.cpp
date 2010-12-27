#include <iostream>
#include "element.h"
#include "circuit.h"

using namespace std ;

void Circuit::dfs(int size, vector<bool>& visited, vector<vector<bool> >& used,
      vector<SmartPtr<Element> >& elements, vector<vector<SmartPtr<Element> > >& result) {
   // visited[i]  -> has nodes[i] been contained in the tree yet?
   // used[i][j]  -> has nodes[i].connections[j] been used yet?
   // elements    -> an array storing current tree edges
   // result      -> an array storing tree edges of all spanning trees

   // check if all the nodes are used
   bool done = true ;

   vector<pair<int , int> > recoverList ;

   for(int v = 1 ; v < size ; ++ v) { // we don't need to check nodes[0], it is GND
      if(visited[v] == false) {
         done = false ;
         int amountOfConnections = this->nodes[v]->connections.size() ;
         for(int i = 0 ; i < amountOfConnections ; ++ i) {
            unsigned desId = this->nodes[v]->connections[i].destination->nodeId ;
            if(visited[desId] == true && used[v][i] == false) {
               recoverList.push_back(pair<int , int>(v , i)) ;
               visited[v] = true ;
               used[v][i] = true ;
               elements.push_back(this->nodes[v]->connections[i].element) ;
               this->dfs(size , visited , used , elements , result) ;
               elements.pop_back() ;
               visited[v] = false ;
            }
         }
      }
   }

   // recover used
   int recoverListLength = recoverList.size() ;
   for(int i = 0 ; i < recoverListLength ; ++ i) {
      used[recoverList[i].first][recoverList[i].second] = false ;
   }

   if(done) {
      result.push_back(elements) ;
   }
}

vector<vector<SmartPtr<Element> > > Circuit::enumTree(unsigned refNodeId) {
   vector<vector<SmartPtr<Element> > > result ;
   vector<SmartPtr<Element> > elements ;
   int size = this->nodes.size() ;
   unsigned refNodeIndex = getIndexById(refNodeId) ;

   vector<bool> visited(size) ;
   vector<vector<bool> > used(size) ;

   for(int i = 0 ; i < size ; i ++) {
      int size_2 = this->nodes[i]->connections.size() ;
      visited[i] = false ;
      used[i] = vector<bool>(size_2) ;
      for(int j = 0 ; j < size_2 ; j ++) {
         used[i][j] = false ;
      }
   }

   visited[refNodeIndex] = true ;

   this->dfs(size, visited , used , elements , result) ;

   return result ;
}

unsigned Circuit::getIndexById(unsigned id) {
   map<unsigned, unsigned>::const_iterator it = idMap.find(id) ;
   if(it == idMap.end()) {
      // node not exist
      unsigned index = nodes.size() ;
      nodes.push_back(new Node(id)) ;
      //cout << "[" << __func__ << "] Size of connections = " << nodes[index].connections.size() << endl ;
      //cout << "[" << __func__ << "] Insert new Node, index = " << index << endl ;
      idMap.insert(pair<unsigned , unsigned>(id , index)) ;
      return index ;
   } else {
      //cout << "[" << __func__ << "] found index = " << (*it).second << endl ;
      return (*it).second ;
   }
}

Node * Circuit::getNodeById(unsigned id) { // consturct new node if not exist
   unsigned index = getIndexById(id) ;
   //cout << "[" << __func__ << "] id(" << id << ") => (" << index << "), ID = " << nodes[index].nodeId << endl ;
   return (nodes[index]) ;
}

void Node::setConnect(const Node * dest, const SmartPtr<Element>& element) {
   cout << "[" << __func__ << "]" << " Setting up connection " << this->nodeId << " -> " << dest->nodeId << " Element = " << element->name() << endl ;
   cout << "[" << __func__ << "] Size of connections = " << this->connections.size() << endl ;
   //element->clone() ;
   this->connections.push_back(Connection(dest , element)) ;
}

Circuit::~Circuit() {
   cout << "[" << __func__ << "]" << endl ;
   //nodes.resize(0) ;
   int size = nodes.size() ;
   for(int i = 0 ; i < size ; ++ i) {
      delete nodes[i] ;
   }
}

Connection::Connection(const Node * dest ,const SmartPtr<Element>& elem) : destination((Node *)dest) , element(elem) {
   cout << "[" << __func__ << "] Cloning " << elem->name() << " (" << elem->type() << "), addr = " << elem.ptr << endl ;
   //this->element = elem ;
}

Connection::~Connection() {
   cout << "[" << __func__ << "]" << endl ;
   //delete this->element ;
}

Node::Node(const unsigned id) : connections() , nodeId(id) {
   cout << "[" << __func__ << "] nodeId = " << this->nodeId << endl ;
}

void Circuit::print() const {
   int size = nodes.size() ;
   cout << "===== Circuit Detail =====" << endl;
   for(unsigned i = 0; i < size ; ++i) {
      cout << "[" << nodes[i]->nodeId << "]" << endl;
      for(unsigned j = 0; j < nodes[i]->connections.size(); ++j)
         cout << " -> [" << nodes[i]->connections[j].destination->nodeId
            << "] " << nodes[i]->connections[j].element->formula() << endl;
   }
}

void Circuit::print() {
   cout << "===== Circuit Detail =====" << endl;
   for(unsigned i = 0; i < nodes.size(); ++i) {
      cout << "[" << nodes[i].nodeId << "]" << endl;
      for(unsigned j = 0; j < nodes[i].connections.size(); ++j)
         cout << " -> [" << nodes[i].connections[j].destination->nodeId
              << "] " << nodes[i].connections[j].element->formula() << endl;
   }
}

void Node::setConnect(Node* destination, Element* element) {
   connections.push_back(Connection(destination, (Element*)element->clone()));
}

