#include "simulator.h"

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
   cerr << "Not done yet!" << endl ;
   throw "Not done yet!" ;
   vector<pair<int , double> > tmp_den ;
   vector<pair<int , double> > tmp_num ;
   if (_circuit->getInputType() == VIN) {
      // 1. back up and remove all the elements leaving input_high [TODO]
      vector<Element *> backUp ;
      // 2. find den: [TODO]
      //    a. add a dummy cell (value = 1) from input_high to input_low
      //    b. get all spanning trees (input_low is the reference node)
      //    c. convert it into formula (den)
      //    d. remove dummy cell
      // 3. find num: [TODO]
      //    a. add a dummy cell (value = 1) from input_high to output_high
      //    b. add a dummy cell (value =-1) from input_high to output_low
      //    c. get all spanning trees (input_low is the reference node)
      //    d. convert it into formula (num)
      //    d remove dummy cell
      // 4. restore the elements leaving input_high [TODO]
   } else if(_circuit->getInputType() == IIN) {
      // 1. find den: [TODO]
      //    a. get all spanning trees (input_low is the reference node)
      //    b. convert it into formula (den)
      // 2. back up and remove all the elements leaving input_high [TODO]
      vector<Element *> backUp ;
      // 3. find num: [TODO]
      //    a. add a dummy cell (value = 1) from input_high to output_high
      //    b. add a dummy cell (value =-1) from input_high to output_low
      //    c. get all spanning trees (input_low is the reference node)
      //    d. convert it into formula (num)
      //    d remove dummy cell
      // 4. restore the elements leaving input_high [TODO]
   } else {
      // TODO create corresponding exception class
      throw "Input type should be VIN or IIN" ;
   }
   // 5. adjust the order of num and den
   int minOrder = tmp_num[0].first > tmp_den[0].first ? tmp_den[0].first : tmp_num[0].first;
   int tmp_num_size = tmp_num.size() , tmp_den_size = tmp_den.size() ;
   for(int i = 0 ; i < tmp_num_size ; ++ i)
      tmp_num[i].first -= minOrder ;
   for(int i = 0 ; i < tmp_den_size ; ++ i)
      tmp_den[i].first -= minOrder ;
   // 6. store num
   store(tmp_num , num) ;
   // 7. store den
   store(tmp_den , den) ;
}

void Simulator::simulate(SimulateConfig& config) {
   // first, find out the formula of circuit
   vector<double> num ;
   vector<double> den ;
   this->findFormula(num , den) ;
   // TODO complete the rest part of simulation
}
