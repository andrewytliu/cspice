#ifndef __UTILS_H__
#define __UTILS_H__
#include<complex>
#include<vector>
#include<map>

#include "element.h"

using namespace std ;

extern void printFormula(vector<vector<Element*> > collection , ostream& fout) ;
extern complex<double> evalFormula(vector<vector<Element*> > collection , double freq);
extern vector<pair<int , double> > expandFormula(vector<vector<Element*> > collection) ;

#endif /* __UTILS_H__ */

