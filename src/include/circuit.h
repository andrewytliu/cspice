#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include<string>
#include<vector>
using namespace std ;

/** Class declaring **/
class Node ;
class Element ;
class Connection ;

/**  Class definitions **/

class Node {
public:
   unsigned nodeId ; // used to identify
   vector<Connection> connections ;
};

class Connection {
public:
   Node * destination ;
   Element * element ;
};

/*
 * This is the base type of Resistor, Capacitor, Inductor
 */
class Element {
public:
   virtual string type() const      = 0 ; // nothing special, might be used when debugging
   virtual string formula() const   = 0 ; // the formula of Z
   virtual int order() const        = 0 ; // R: 0, L: -1, C: 1
   virtual double value() const     = 0 ; // the admittance = s^(order()) * value()
   string name() const { return this->_name; }
protected:
   double _value ;
   string _name ;
};

#endif //#ifndef _CIRCUIT_H_
