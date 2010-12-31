#ifndef __PARSER_H__
#define __PARSER_H__

#include "circuit.h"
#include "simulator.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
   Parser(const char *) ;
   const Circuit& getCircuit() const { return circuit; }
   const vector<SimulateConfig>& getConfig() const { return config; }
   Circuit& getCircuit() { return circuit; }
   vector<SimulateConfig>& getConfig() { return config; }

   void addFreqSwap(double,double,int,const char *,const char *) ;
   void addTimeSwap(double,double,double,const char *) ;
   void addR(const char *,int,int,double) ;
   void addL(const char *,int,int,double) ;
   void addC(const char *,int,int,double) ;
   void addV(const char *,int,int,double,double) ;
   void addI(const char *,int,int,double,double) ;
   void addG(const char *,int,int,int,int,double) ;
   void addOut(int,int) ;

private:
   void setRLC(Element * , int , int) ;
   Circuit circuit;
   vector<SimulateConfig> config;
};

class ValueParser {
friend istream& operator>>(istream&, ValueParser&);
public:
   ValueParser(double& v) : value(v) {}
private:
   double& value;
};

extern istream& operator>>(istream&, ValueParser&);

class ParseError {
private:
   string _message;
public:
   ParseError(const string& msg) : _message(msg) { }
   const char * message() { return _message.c_str() ; }
};

#endif /* __PARSER_H__ */

