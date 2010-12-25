#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "smartObj.h"
#include <string>
using namespace std;
/*
 * This is the base type of Resistor, Capacitor, Inductor
 */
class Element : public SmartObj {
public:
   Element(string name, double value) : SmartObj(), _name(name), _value(value) {}
   virtual string type() const      = 0 ; // nothing special, might be used when debugging
   virtual string formula() const   = 0 ; // the formula of Y (admittance)
   virtual int order() const        = 0 ; // R: 0, L: -1, C: 1
   virtual double value() const     = 0 ; // the admittance = s^(order()) * value()
   string name() const { return this->_name; }
protected:
   double _value ;
   string _name ;
};

class Capacitor : public Element {
public:
   Capacitor(string name , double value) : Element(name, value) { }

   virtual string type() const {
      return "Capacitor" ;
   }

   virtual string formula() const {
      return "s * " + this->_name ;
   }

   virtual int order() const {
      return 1;
   }

   virtual double value() const {
      return this->_value ;
   }
};

class Inductor : public Element {
public:
   Inductor(string name , double value) : Element(name, value) { }
   virtual string type() const {
      return "Inductor" ;
   }

   virtual string formula() const {
      return "1 / (s * " + this->_name + ")" ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return 1.0 / this->_value ;
   }
};

class Resistor : public Element {
public:
   Resistor(string name , double value) : Element(name, value) { }
   virtual string type() const {
      return "Resistor" ;
   }

   virtual string formula() const {
      return "1 / " + this->_name ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return 1.0 / this->_value ;
   }
};

class VCCS : public Element {
public:
   VCCS(string name , double value) : Element(name, value) { }
   virtual string type() const {
      return "Voltage Controlled Current Source" ;
   }

   virtual string formula() const {
      return this->_name ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return this->_value ;
   }
};

class Dummy : public Element {
public:
   Dummy(string name , double value) : Element(name, value) { }
   virtual string type() const {
      return "Dummy" ;
   }

   virtual string formula() const {
      return this->_name ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return this->_value ;
   }
};

#endif /* __ELEMENT_H__ */

