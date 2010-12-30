#include <algorithm>
#include <iomanip>
#include <iostream>
#include "element.h"
#include "circuit.h"
#include "utils.h"

using namespace std ;
#ifdef __ELIMINATION__
template<class T>
static void eliminate(int index , vector< T >& array) {
   typedef typename std::vector<T>::iterator It;
   for (It it = array.begin() ; it != array.end() ; it ++) {
      if (index == 0) {
         array.erase(it) ; break ;
      } else {
         -- index ;
      }
   }
   return ;
}
#endif // __ELIMINATION__


void Circuit::dfs(
   int size,
   vector<bool>& visited,
   vector<vector<bool> >& used,
   vector<const Element*>& current_tree,
   vector<vector<const Element*> >& result
   #ifdef __ELIMINATION__
   ,vector<pair<char , unsigned long long> >& trees
   #endif
) {
   // visited[i]  -> has nodes[i] been contained in the tree yet?
   // used[i][j]  -> has nodes[i].connections[j] been used yet?
   // current_tree-> an array storing current tree edges
   // result      -> an array storing tree edges of all spanning trees

   // check if all the nodes are used
   bool done = true ;

   vector<pair<int , int> > recoverList ;

   for(int v = 0 ; v < size ; ++ v) {
      if(visited[v] == false) {
         done = false ;
         int amountOfConnections = this->nodes[v]->connections.size() ;
         for(int i = 0 ; i < amountOfConnections ; ++ i) {
            unsigned desId = this->nodes[v]->connections[i].destination->nodeId ;
            unsigned desIndex = getIndexById(desId) ;

            if(visited[desIndex] == true && used[v][i] == false) {
               recoverList.push_back(pair<int , int>(v , i)) ;
               visited[v] = true ;
               used[v][i] = true ;
               current_tree.push_back(this->nodes[v]->connections[i].element) ;
#ifdef __ELIMINATION__
               this->dfs(size , visited , used , current_tree , result , trees) ;
#else
               this->dfs(size , visited , used , current_tree , result) ;
#endif
               current_tree.pop_back() ;
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
#ifdef __ELIMINATION__
      // check if elimination could be done
      char sign = 1;
      int eliminateId ;
      int amountOfElements = current_tree.size() ;
      int amountOfTrees ;
      unsigned long long hashValue = 0ull ;

      string concatName ;

      for (int i = 0 ; i < amountOfElements ; ++ i) {
         // if two elements only different in sign,
         // they should have same formula.
         // Ex: gm and -gm

         // use + , since it's commutative, that is,
         // the order of elements doesn't matter.

         // Be ware that, we have to take the risk of
         // hash(a) + hash(b) == hash(c) + hash(d),
         // though I don't think it would happen so easily.
         hashValue += hash(current_tree[i]->formula().c_str()) ;

         sign *= current_tree[i]->sign() ;
      }

      eliminateId = -1 ;
      amountOfTrees = trees.size() ;
      for (int i = 0 ; i < amountOfTrees ; ++ i) {
         if (trees[i].first == -sign && trees[i].second == hashValue) {
            eliminateId = i ; break ;
         }
      }

      if (eliminateId == -1) {
         result.push_back(current_tree) ;
         trees.push_back(pair<char , unsigned long long>(sign , hashValue)) ;
      } else {
         eliminate(eliminateId , result);
         eliminate(eliminateId , trees) ;
      }
#else // __ELIMINATION__
      result.push_back(current_tree) ;
#endif // __ELIMINATION__
   }
}

vector<vector<const Element*> > Circuit::enumTree(const Node * refNode) {
   vector<vector<const Element*> > result ;
   vector<const Element*> current_tree ;
   int size = this->nodes.size() ;
   unsigned refNodeIndex = getIndexById(refNode->nodeId) ;

   vector<bool> visited(size , false) ;
   vector<vector<bool> > used(size) ;
#ifdef __ELIMINATION__
   // pair of sign, sum of hashed element names
   vector<pair<char , unsigned long long> > trees ;
#endif // __ELIMINATION__
   for(int i = 0 ; i < size ; i ++) {
      int size_2 = this->nodes[i]->connections.size() ;
      used[i] = vector<bool>(size_2 , false) ;
   }

   visited[refNodeIndex] = true ;

#ifdef __ELIMINATION__
   this->dfs(size, visited , used , current_tree , result , trees) ;
#else
   this->dfs(size, visited , used , current_tree , result) ;
#endif
   return result ;
}

unsigned Circuit::getIndexById(unsigned id) {
   map<unsigned, unsigned>::const_iterator it = idMap.find(id) ;
   if(it == idMap.end()) {
      // node not exist
      unsigned index = nodes.size() ;
      nodes.push_back(new Node(id)) ;
      idMap.insert(pair<unsigned , unsigned>(id , index)) ;
      return index ;
   } else {
      return (*it).second ;
   }
}

Node * Circuit::getNodeById(unsigned id) { // consturct new node if not exist
   return nodes[getIndexById(id)] ;
}

void Node::setConnect(const Node * dest, const Element* element) {
   this->connections.push_back(Connection(dest , element)) ;
}

Circuit::~Circuit() {
   int size = nodes.size() ;
   for(int i = 0 ; i < size ; ++ i) {
      delete nodes[i] ;
   }
   size = elements.size() ;
   for(int i = 0 ; i < size ; ++ i) {
      delete elements[i] ;
   }
}

void Circuit::print() const {
   unsigned size = nodes.size() ;
   cout << "===== Circuit Detail =====" << endl;
   for(unsigned i = 0; i < size ; ++ i) {
      cout << "[" << setw(2) << nodes[i]->nodeId << "]" << endl;
      for(unsigned j = 0; j < nodes[i]->connections.size(); ++j) {
         cout << " -> [" << setw(2) << nodes[i]->connections[j].destination->nodeId << "] " << *(nodes[i]->connections[j].element) << endl ;
      }
   }
}

