#include <iomanip>
#include <iostream>
#include <vector>

#include "utils.h"
#include "simulator.h"
#ifdef CUDA
#include "integral.h"
#endif

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

Simulator::Simulator(Circuit * circuit, ofstream& f) : _circuit(circuit), _fout(f) , transferFunctions() {
   // _circuit is a pointer
   // First, check circuit
   // this->_circuit->check() ;
   // Second, for each SRC in circuit, find T(s) = (Vout / SRC)
   //    transferFunctions is a map that map <const Source *> to <TransferFunction>
   vector<Source *>& sources = _circuit->sources ;
   int size = sources.size() ;
   for(int i = 0 ; i < size ; ++ i) {
      TransferFunction transferFunction ;
      this->findFormula(sources[i] , transferFunction) ;
      transferFunctions.insert(pair<const Source * , TransferFunction>(sources[i] , transferFunction)) ;
   }
}

void Simulator::findFormula(const Source * src , TransferFunction& tf) {
   vector<pair<int , double> > tmp_den ;
   vector<pair<int , double> > tmp_num ;
   vector<vector<const Element*> > denSpanningTrees ;
   vector<vector<const Element*> > numSpanningTrees ;

   Node *iH , *iL , *oH , *oL ;

   _circuit->currentSource = src ;

   iH = _circuit->getNodeById(src->node1()) ;
   iL = _circuit->getNodeById(src->node2()) ;

   oH = _circuit->   getOutputHigh  () ;
   oL = _circuit->   getOutputLow   () ;

   if (src->type() == Source::VSRC) {
      // 1. back up and remove all the elements leaving input_high
      vector<Connection> backUp ;
      backUp = iH->connections;
      iH->connections.clear() ;
      // 2. find den:
      //    a. add a dummy cell (value = 1) from input_high to input_low
      iH->setConnect(iL , new Dummy("Dummy element for DEN (+1)" , 1)) ;
      //    b. get all spanning trees (input_low is the reference node)
      denSpanningTrees = _circuit->enumTree(iL) ;
      //    c. remove dummy cell
      iH->connections.clear() ;
      // 3. find num:
      //    a. add a dummy cell (value =-1) from input_high to output_high
      iH->setConnect(oH , new Dummy("Dummy element for NUM (-1)" ,-1)) ;
      //    b. add a dummy cell (value = 1) from input_high to output_low
      iH->setConnect(oL , new Dummy("Dummy element for NUM (+1)" , 1)) ;
      //    c. get all spanning trees (input_low is the reference node)
      numSpanningTrees = _circuit->enumTree(iL) ;
      //    d remove dummy cell
      iH->connections.clear() ;
      // 4. restore the elements leaving input_high
      iH->connections = backUp ;
   } else if(src->type() == Source::ISRC) {
      // 1. find den:
      //    a. get all spanning trees (input_low is the reference node)
      denSpanningTrees = _circuit->enumTree(iL) ;
      // 2. back up and remove all the elements leaving input_high
      vector<Connection> backUp = iH->connections ;
      iH->connections.clear() ;
      // 3. find num:
      //    a. add a dummy cell (value =-1) from input_high to output_high
      iH->setConnect(oH , new Dummy("Dummy element for NUM (-1)" ,-1)) ;
      //    b. add a dummy cell (value =+1) from input_high to output_low
      iH->setConnect(oL , new Dummy("Dummy element for NUM (+1)" , 1)) ;
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
   store(tmp_num , tf.num) ;
   // 8. store den
   store(tmp_den , tf.den) ;

   return ;
}

void Simulator::simulate(SimulateConfig& config) {
   if (config.type == FREQ) {
      // [TODO] Since this is freq. swap, only src would effect.
      const Source * p_src = NULL ;
      TransferFunction tf ;

      for (map<const Source * , TransferFunction>::const_iterator it = transferFunctions.begin() ;
            it != transferFunctions.end() ; ++ it) {
         if (it->first->name() == config.srcName) {
            p_src = it->first ;
            tf = it->second ;
         }
      }
      if(p_src == NULL) {
         stringstream msg ;
         msg << "Can't find source \"" << config.srcName << "\"" ;
         throw SimulateException(msg.str()) ;
      }
      vector<pair<double,complex<double> > > result ;
#ifdef CUDA
      freqGpuSimulate(config, tf, result);
#else
      double ratio = exp(log(10.0) / config.step) ;

      for (double freq = config.start ; freq <= config.end ; freq *= ratio) {
         result.push_back(pair<double , complex<double> >(freq , evalFormula(tf.num , freq) / evalFormula(tf.den , freq))) ;
      }
#endif
      plotFreq(result, config);
   } else if (config.type == TIME) {
      vector<pair<double , double> > result ;
      // 1. Find out the prev value of Vout
      // 2. For each src that changes value, find it's unit step response, V(t)
      // 3. Vout = DC + Sum{ Delta * V(t) , for each src }
      double prevVOUT = 0 ;
      vector<double> times ;
      vector<double> vout  ;
      unsigned size = 0;

      if (config.start >= config.end) {
         throw SimulateException("The start point should be before the end point.") ;
      }
      // if config.step is illegal, or,
      // if there are too less node, make config.step be 1 / 20 of total times
      if (config.step <= 0 || (config.end - config.start) / config.step < 40.0 ) {
         config.step = (config.end - config.start) / 40.0 ;
      }

      // generate times, first, from t = 0 to t = config.start
      // step = (config.start - 0) / 80 (I don't want to spend too much time on
      //                                 simulating initial condition)
      if (config.start > 0) {
         for (double time = 0.0 , step = (config.start - 0.0) / 80.0 ; time < config.start ; time += step) {
            vout.push_back(0.0) ;
            times.push_back(time) ;
            ++ size ;
         }
      }

      // generate times from t = config.start to config.end
      for (double time = config.start ; time <= config.end ; time += config.step) {
         times.push_back(time) ;
         vout.push_back(0.0) ;
         ++ size ;
      }

      for (map<const Source * , TransferFunction>::iterator it = transferFunctions.begin() ;
            it != transferFunctions.end() ; ++ it) {
         complex<double> v = evalFormula(it->second.num , 0) / evalFormula(it->second.den , 0) ;
         prevVOUT += abs(v) * it->first->prevValue() ;
         if (abs(it->first->pulseValue()) > 0) {
            vector<double> vout_tmp = numericalIntegration(times , it->second) ;
            for(unsigned i = 0 ; i < size ; ++ i) {
               vout[i] += vout_tmp[i] * it->first->pulseValue() ;
            }
         }
      }

      for (unsigned i = 0 ; i < size ; ++ i) {
         vout[i] += prevVOUT ;
      }

      for (unsigned i = 0 ; i < size ; ++ i) {
         if (times[i] >= config.start) {
            result.push_back(pair<double , double>(times[i] , vout[i])) ;
         }
      }

      plotTime(result, config);
   }
}

void Simulator::plotFreq(const vector<pair<double,complex<double> > >& points, const SimulateConfig& config) {
   _fout << "set terminal postscript eps enhanced color solid" << endl
         << "set output '" << config.filename << "'" << endl
         << "set multiplot layout 2, 1 title 'Frequency response'" << endl
         << "set title 'Magnitude'" << endl
         << "set xlabel 'Frequency (Hz)'" << endl
         << "set ylabel 'Gain (dB)'" << endl
         << "set log x" << endl
         << "plot '-' title '' with line" << endl;

   for(unsigned i = 0; i < points.size(); ++i)
      _fout << scientific << points[i].first << ' ' << 20.0 * log10(abs(points[i].second)) << endl;

   _fout << "e" << endl << "set title 'Phase'" << endl
         << "set xlabel 'Frequency (Hz)'" << endl
         << "set ylabel 'Argument (Degree)'" << endl
         << "set yrange [-180:180]" << endl
         << "plot '-' title '' with line" << endl;

   for(unsigned i = 0; i < points.size(); ++i)
      _fout << scientific << points[i].first << ' ' << arg(points[i].second) * 180.0 / acos(-1.0) << endl;

   _fout << "e" << endl
         << "unset multiplot" << endl
         << "set yrange [*:*]" << endl
         << "unset log" << endl;
}

void Simulator::plotTime(const vector<pair<double,double> >& points, const SimulateConfig& config) {
   _fout << "set terminal postscript eps enhanced color solid" << endl
         << "set title 'Time response'" << endl
         << "set xlabel 'Time (s)'" << endl
         << "set ylabel 'Voltage (V)'" << endl
         << "set output '" << config.filename << "'" << endl
         << "plot '-' with lines title ''" << endl;

   for(unsigned i = 0; i < points.size(); ++i)
      _fout << scientific << points[i].first << ' ' << points[i].second << endl;

   _fout << "e" << endl;
}

