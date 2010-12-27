#include <iostream>
#include <vector>

#include "utils.h"
#include "simulator.h"

using namespace std ;

static void store(const vector<pair<int , double> >& mapping , vector<double>& array) {
   // we may assume that
   //    1. mapping is sorted by pair::first, and all pair::first >= 0
   //    2. array is empty
   int size = mapping.size() ;
   for(int order = 0 , i = 0 ; i < size ; ++ i) {
      while(order < mapping[i].first) {
         array.push_back(0.0) ;
         order ++ ;
      }
      array.push_back(mapping[i].second) ;
   }
   return ;
}

Simulator::Simulator(Circuit * circuit) : _circuit(circuit) {
   // _circuit is a pointer
}

void Simulator::findFormula(vector<double>& num, vector<double>& den) {
   vector<pair<int , double> > tmp_den ;
   vector<pair<int , double> > tmp_num ;
   vector<vector<SmartPtr<Element> > > denSpanningTrees ;
   vector<vector<SmartPtr<Element> > > numSpanningTrees ;
   Node *iH , *iL , *oH , *oL ;

   iH = _circuit->   getInputHigh   () ;
   iL = _circuit->   getInputLow    () ;
   oH = _circuit->   getOutputHigh  () ;
   oL = _circuit->   getOutputLow   () ;

   if (_circuit->getInputType() == VIN) {
      cout << "Input type is VIN" << endl ;
      // 1. back up and remove all the elements leaving input_high
      vector<Connection> backUp ;
      backUp = iH->connections;
      iH->connections.clear() ;
      // 2. find den:
      //    a. add a dummy cell (value = 1) from input_high to input_low
      iH->setConnect(iL , SmartPtr<Element>(new Dummy("Dummy element for DEN" , 1.0))) ;
      cout << "Print out DEN graph" << endl ;
      _circuit->print() ;
      //    b. get all spanning trees (input_low is the reference node)
      denSpanningTrees = _circuit->enumTree(iL) ;
      //    c. remove dummy cell
      iH->connections.clear() ;
      // 3. find num:
      //    a. add a dummy cell (value =-1) from input_high to output_high
      iH->setConnect(oH , SmartPtr<Element>(new Dummy("Dummy element for NUM" ,-1.0))) ;
      //    b. add a dummy cell (value = 1) from input_high to output_low
      iH->setConnect(oL , SmartPtr<Element>(new Dummy("Dummy element for NUM" , 1.0))) ;
      cout << "Print out NUM graph" << endl ;
      _circuit->print() ;
      //    c. get all spanning trees (input_low is the reference node)
      numSpanningTrees = _circuit->enumTree(iL) ;
      //    d remove dummy cell
      iH->connections.clear() ;
      // 4. restore the elements leaving input_high
      iH->connections = backUp ;
   } else if(_circuit->getInputType() == IIN) {
      cout << "Input type is IIN" << endl ;
      cout << "Print out DEN graph" << endl ;
      _circuit->print() ;

      // 1. find den:
      //    a. get all spanning trees (input_low is the reference node)
      denSpanningTrees = _circuit->enumTree(iL) ;
      // 2. back up and remove all the elements leaving input_high
      vector<Connection> backUp = iH->connections ;
      iH->connections.clear() ;
      // 3. find num:
      //    a. add a dummy cell (value =-1) from input_high to output_high
      iH->setConnect(oH , SmartPtr<Element>(new Dummy("Dummy element for NUM" ,-1.0))) ;
      //    b. add a dummy cell (value =+1) from input_high to output_low
      iH->setConnect(oL , SmartPtr<Element>(new Dummy("Dummy element for NUM" , 1.0))) ;
      cout << "Print out DEN graph" << endl ;
      _circuit->print() ;
      //    c. get all spanning trees (input_low is the reference node)
      numSpanningTrees = _circuit->enumTree(iL) ;
      //    d remove dummy cell
      iH->connections.clear() ;
      // 4. restore the elements leaving input_high
      iH->connections = backUp ;
   } else {
      // TODO create corresponding exception class
      throw "Input type should be VIN or IIN" ;
   }
   // DEBUG: print out formula
   cout << "======== Print out Den Tree ========" << endl ;
   printFormula(denSpanningTrees , cout) ;
   cout << "======== Print out Num Tree ========" << endl ;
   printFormula(numSpanningTrees , cout) ;
   cout << endl ;
   // 5. expand formula
   tmp_den = expandFormula(denSpanningTrees) ;
   tmp_num = expandFormula(numSpanningTrees) ;
   // 6. adjust the order of num and den
   int minOrder = tmp_num[0].first > tmp_den[0].first ? tmp_den[0].first : tmp_num[0].first;
   int tmp_num_size = tmp_num.size() , tmp_den_size = tmp_den.size() ;
   for(int i = 0 ; i < tmp_num_size ; ++ i)
      tmp_num[i].first -= minOrder ;
   for(int i = 0 ; i < tmp_den_size ; ++ i)
      tmp_den[i].first -= minOrder ;
   // 7. store num
   store(tmp_num , num) ;
   // 8. store den
   store(tmp_den , den) ;

   return ;
}

void Simulator::simulate(SimulateConfig& config) {
   // first, find out the formula of circuit
   vector<double> num ;
   vector<double> den ;
   this->findFormula(num , den) ;
   // TODO complete the rest part of simulation
}
