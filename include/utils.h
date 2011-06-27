#ifndef __UTILS_H__
#define __UTILS_H__
#include<complex>
#include<vector>
#include<map>

#include "element.h"
#include "simulator.h"

using namespace std ;


extern void printTimeElapsed();
extern void printFormula(const vector<vector<const Element*> > & , ostream &) ;
extern complex<double> evalFormula(const vector<double> & , const double&);
extern vector<pair<int , double> > expandFormula(const vector<vector<const Element*> > &) ;
vector<double> numericalIntegration(const vector<double>& times,Simulator::TransferFunction& tf, unsigned shift = 8) ;
extern unsigned long long hash(const char *) ;

#endif /* __UTILS_H__ */

