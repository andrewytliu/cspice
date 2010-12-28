#include <iomanip>
#include <iostream>
#include <vector>

#include "utils.h"
#include "simulator.h"

using namespace std ;

static void store(const vector<pair<int , double> >& mapping,
      vector<double>& array) {
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
      order ++ ;
   }
   return ;
}

Simulator::Simulator(Circuit * circuit, ofstream& f) : _circuit(circuit), _fout(f) , num() , den() {
   // _circuit is a pointer
   findFormula(this->num , this->den) ;
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
      // 1. back up and remove all the elements leaving input_high
      vector<Connection> backUp ;
      backUp = iH->connections;
      iH->connections.clear() ;
      // 2. find den:
      //    a. add a dummy cell (value = 1) from input_high to input_low
      iH->setConnect(iL , SmartPtr<Element>(new Dummy("Dummy element for DEN (+1)" , 1))) ;
      //    b. get all spanning trees (input_low is the reference node)
      denSpanningTrees = _circuit->enumTree(iL) ;
      //    c. remove dummy cell
      iH->connections.clear() ;
      // 3. find num:
      //    a. add a dummy cell (value =-1) from input_high to output_high
      iH->setConnect(oH , SmartPtr<Element>(new Dummy("Dummy element for NUM (-1)" ,-1))) ;
      //    b. add a dummy cell (value = 1) from input_high to output_low
      iH->setConnect(oL , SmartPtr<Element>(new Dummy("Dummy element for NUM (+1)" , 1))) ;
      //    c. get all spanning trees (input_low is the reference node)
      numSpanningTrees = _circuit->enumTree(iL) ;
      //    d remove dummy cell
      iH->connections.clear() ;
      // 4. restore the elements leaving input_high
      iH->connections = backUp ;
   } else if(_circuit->getInputType() == IIN) {
      // 1. find den:
      //    a. get all spanning trees (input_low is the reference node)
      denSpanningTrees = _circuit->enumTree(iL) ;
      // 2. back up and remove all the elements leaving input_high
      vector<Connection> backUp = iH->connections ;
      iH->connections.clear() ;
      // 3. find num:
      //    a. add a dummy cell (value =-1) from input_high to output_high
      iH->setConnect(oH , SmartPtr<Element>(new Dummy("Dummy element for NUM (-1)" ,-1))) ;
      //    b. add a dummy cell (value =+1) from input_high to output_low
      iH->setConnect(oL , SmartPtr<Element>(new Dummy("Dummy element for NUM (+1)" , 1))) ;
      //    c. get all spanning trees (input_low is the reference node)
      numSpanningTrees = _circuit->enumTree(iL) ;
      //    d remove dummy cell
      iH->connections.clear() ;
      // 4. restore the elements leaving input_high
      iH->connections = backUp ;
   } else {
      throw SimulateException("Input type should be VIN or IIN") ;
   }

   // DEBUG: print out formula
   cout << "======== Print out Den Trees ========" << endl ;
   printFormula(denSpanningTrees , cout) ;
   cout << endl ;
   cout << "======== Print out Num Trees ========" << endl ;
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
   // vector<double> num ;
   // vector<double> den ;
   // this->findFormula(num , den) ;

   if (config.type == FREQ) {
      vector<pair<double,complex<double> > > result ;
      double ratio = exp(log(10.0) / config.step) ;

      for (double freq = config.start ; freq <= config.end ; freq *= ratio) {
         result.push_back(pair<double , complex<double> >(freq , evalFormula(num , freq) / evalFormula(den , freq))) ;
      }
      plotFreq(result, config);
   } else if (config.type == TIME) {
      // this is for unit step response, that is,
      // In[t] = 1.0 for t >= 0
      //
      // for convenient, make Num and Den have save size.
      int N = max(den.size() , num.size()) ;
      vector<pair<double , double> > result ;// pair of time and value

      num.resize(N , 0.0) ;
      den.resize(N , 0.0) ;

      double * newU = new double[N] ; // u[k][t + 1]
      double * oldU = new double[N] ; // u[k][t]
      double DEN , step ;

      DEN = 0.0 ;
      for (int k = 0 ; k < N ; ++ k) {
         DEN = DEN * (config.step / 2.0) + den[k] ;
      }

      // Initialize
      for (int i = 0 ; i < N ; ++ i) {
         newU[i] = oldU[i] = 0 ;
      }
      oldU[0] = 1.0 / den[N - 1] ;

      //step = config.step / (N * 1e3); // Real time step is (config.step / (N * 1e3))
      step = config.step / (N << 10); // Real time step is (config.step / (N * 1e3))

      for (double time = config.start + config.step ; time <= config.end ; time += config.step) {
         for(int i = 0 ; i < (N << 10) ; ++ i) { // Real time step is (config.step / (N * 1e3))
            double sum1 , sum2 ;

            newU[0] = 0.0 ;
            sum1 = oldU[0] * (step / 2.0) ;
            sum2 = 0.0 ;
            for (int k = 1 ; k < N ; ++ k) {
               sum2 = sum2 * (step / 2.0)  + oldU[k] ;
               newU[0] += den[N - k - 1] * (sum1 + sum2) ;
               sum1 = (step / 2.0) * (sum1 + oldU[k]) ;
            }
            newU[0] = (1.0 - newU[0]) / DEN ;

            for (int k = 1 ; k < N ; ++ k) {
               newU[k] = oldU[k] + (step / 2.0) * (oldU[k - 1] + newU[k - 1]) ;
            }
            for (int k = 0 ; k < N ; ++ k) {
               oldU[k] = newU[k] ;
            }
         }
         double out = 0.0 ;
         for (int k = 0 ; k < N ; ++ k) {
            out += num[N - k - 1] * newU[k] ;
         }

         result.push_back(pair<double , double>(time , out)) ;
      }

      delete [] newU ;
      delete [] oldU ;
      plotTime(result, config);
   }
}

void Simulator::plotFreq(const vector<pair<double,complex<double> > >& points, const SimulateConfig& config) {
   _fout << "set terminal postscript enhanced color solid" << endl
        << "set output '" << config.filename << "'" << endl
        << "set multiplot layout 2, 1 title 'Frequency response'" << endl
        << "set title 'Magnitude'" << endl
        << "set log xy" << endl
        << "plot '-' title '' with line" << endl;

   for(unsigned i = 0; i < points.size(); ++i)
      _fout << scientific << points[i].first << ' ' << 20.0 * log10(abs(points[i].second)) << endl;

   _fout << "e" << endl << "set title 'Phase'" << endl
        << "set yrange [-180:180]" << endl
        << "unset logscale y" << endl
        << "plot '-' title '' with line" << endl;

   for(unsigned i = 0; i < points.size(); ++i)
      _fout << scientific << points[i].first << ' ' << arg(points[i].second) * 180.0 / acos(-1.0) << endl;

   _fout << "e" << endl
        << "unset multiplot" << endl
        << "set yrange [*:*]" << endl
        << "unset log" << endl;
}

void Simulator::plotTime(const vector<pair<double,double> >& points, const SimulateConfig& config) {
   // TODO: error

   _fout << "set terminal postscript enhanced color solid" << endl
        << "set title 'Time response'" << endl
        << "set xlabel 'Time (s)'" << endl
        << "set ylabel 'Voltage (V)'" << endl
        << "set output '" << config.filename << "'" << endl
        << "plot '-' with lines title ''" << endl;

   for(unsigned i = 0; i < points.size(); ++i)
      _fout << scientific << points[i].first << ' ' << points[i].second << endl;

   _fout << "e" << endl;
}

