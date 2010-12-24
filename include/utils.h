#ifndef __UTILS_H__
#define __UTILS_H__
#include<complex>

using namespace std ;

extern void printFormula(vector<vector<Element*> > collection , ostream& fout) ;
extern complex<double , double> evalFormula(vector<vector<Element*> > collection , double freq);

#endif /** __UTILS_H__
