#include <algorithm>
#include <iostream>
#include "element.h"
#include "circuit.h"

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

#ifdef __ELIMINATION__
void Circuit::dfs(int size, vector<bool>& visited, vector<vector<bool> >& used,
      vector<SmartPtr<Element> >& elements, vector<vector<SmartPtr<Element> > >& result,
      vector<pair<char , string> >& trees)
#else
void Circuit::dfs(int size, vector<bool>& visited, vector<vector<bool> >& used,
      vector<SmartPtr<Element> >& elements, vector<vector<SmartPtr<Element> > >& result)
#endif
{
   // visited[i]  -> has nodes[i] been contained in the tree yet?
   // used[i][j]  -> has nodes[i].connections[j] been used yet?
   // elements    -> an array storing current tree edges
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
               elements.push_back(this->nodes[v]->connections[i].element) ;
#ifdef __ELIMINATION__
               this->dfs(size , visited , used , elements , result , trees) ;
#else
               this->dfs(size , visited , used , elements , result) ;
#endif
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
#ifdef __ELIMINATION__
      // check if elimination could be done
      char sign = 1;
      int eliminateId ;
      int amountOfElements = elements.size() ;
      int amountOfTrees ;
      string * names = new string[amountOfElements] ;
      string concatName ;

      for (int i = 0 ; i < amountOfElements ; ++ i) {
         // if two elements only different in sign, they should have same formula.
         // Ex: gm and -gm
         names[i] = elements[i]->formula() ;
         sign *= elements[i]->sign() ;
      }
      sort(names , names + amountOfElements) ;

      concatName = "" ;
      for (int i = 0 ; i < amountOfElements ; ++ i) {
         concatName += names[i] ;
      }

      eliminateId = -1 ;
      amountOfTrees = trees.size() ;
      for (int i = 0 ; i < amountOfTrees ; ++ i) {
         if (trees[i].first == -sign) {
            if (trees[i].second == concatName) {
               eliminateId = i ; break ;
            }
         }
      }

      if (eliminateId == -1) {
         result.push_back(elements) ;
         trees.push_back(pair<char , string>(sign , concatName)) ;
      } else {
         eliminate(eliminateId , result);
         eliminate(eliminateId , trees) ;
      }

      delete [] names ;
#else // __ELIMINATION__
      result.push_back(elements) ;
#endif // __ELIMINATION__
   }
}

vector<vector<SmartPtr<Element> > > Circuit::enumTree(const Node * refNode) {
   vector<vector<SmartPtr<Element> > > result ;
   vector<SmartPtr<Element> > elements ;
   int size = this->nodes.size() ;
   unsigned refNodeIndex = getIndexById(refNode->nodeId) ;

   vector<bool> visited(size , false) ;
   vector<vector<bool> > used(size) ;
#ifdef __ELIMINATION__
   vector<pair<char , string> > trees ; // pair of sign, element names
#endif // __ELIMINATION__
   for(int i = 0 ; i < size ; i ++) {
      int size_2 = this->nodes[i]->connections.size() ;
      used[i] = vector<bool>(size_2 , false) ;
   }

   visited[refNodeIndex] = true ;

#ifdef __ELIMINATION__
   this->dfs(size, visited , used , elements , result , trees) ;
#else
   this->dfs(size, visited , used , elements , result) ;
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

void Node::setConnect(const Node * dest, const SmartPtr<Element>& element) {
   this->connections.push_back(Connection(dest , element)) ;
}

Circuit::~Circuit() {
   int size = nodes.size() ;
   for(int i = 0 ; i < size ; ++ i) {
      delete nodes[i] ;
   }
}

void Circuit::print() const {
   int size = nodes.size() ;
   cout << "===== Circuit Detail =====" << endl;
   for(unsigned i = 0; i < size ; ++i) {
      cout << "[" << nodes[i]->nodeId << "]" << endl;
      for(unsigned j = 0; j < nodes[i]->connections.size(); ++j)
         cout << " -> [" << nodes[i]->connections[j].destination->nodeId
            << "] " << nodes[i]->connections[j].element->name() << endl;
   }
}

