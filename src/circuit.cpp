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

vector<vector<Element*> > Circuit::enumTree() {
   vector<vector<Element*> > result ;
   int size = this->nodes.size() ;

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

   visited[0] = true ;

   this->dfs(size, visited , used , vector<Element*>() , result) ;

   return result ;
}

