#include <iostream>
#include "element.h"
#include "circuit.h"

void Circuit::dfs(int size, vector<bool>& visited, vector<vector<bool> >& used,
      vector<Element*>& elements, vector<vector<Element*> >& result) {
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
         int amountOfConnections = this->nodes[v].connections.size() ;
         for(int i = 0 ; i < amountOfConnections ; ++ i) {
            unsigned desId = this->nodes[v].connections[i].destination->nodeId ;
            if(visited[desId] == true && used[v][i] == false) {
               recoverList.push_back(pair<int , int>(v , i)) ;
               visited[v] = true ;
               used[v][i] = true ;
               elements.push_back(this->nodes[v].connections[i].element) ;
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

vector<vector<Element*> > Circuit::enumTree(unsigned refNodeId) {
   vector<vector<Element*> > result ;
   vector<Element*> elements ;
   int size = this->nodes.size() ;
   unsigned refNodeIndex = getIndexById(refNodeId) ;

   vector<bool> visited(size) ;
   vector<vector<bool> > used(size) ;

   for(int i = 0 ; i < size ; i ++) {
      int size_2 = this->nodes[i].connections.size() ;
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
      nodes.push_back(Node()) ;
      nodes[index].nodeId = id ;
      idMap.insert(pair<unsigned , unsigned>(id , index)) ;
      return index ;
   } else {
      return it->second ;
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

