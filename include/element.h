#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <string>
#include <iostream>

#include "smartObj.h"
using namespace std;
/*
 * This is the base type of Resistor, Capacitor, Inductor
 */
class Element : public SmartObj {
public:
   enum ElementType {
      T_CAPACITOR,
      T_INDUCTOR,
      T_RESISTOR,
      T_VCCS,
      T_DUMMY,
   } ;

   friend class SmartPtr<Element> ;
   Element(const string& name, double value) :SmartObj(), _name(name), _value(value) {}
   virtual ~Element() {}
   virtual ElementType type() const = 0 ; // Get the type of this element
   virtual string formula() const   = 0 ; // the formula of Y (admittance), without sign
   virtual int order() const        = 0 ; // R: 0, L: -1, C: 1
   virtual double value() const     = 0 ; // the admittance = s^(order()) * value() * sign()
   string name() const { return this->_name; }

   const Element * clone() const {
      return (const Element *)this->SmartObj::clone() ;
   }

   virtual char sign() const {
      return 1 ;
   }

   friend ostream& operator<<(ostream& out , const Element& element) ;
protected:
   string _name ;
   double _value ;
};

class Capacitor : public Element {
public:
   Capacitor(const string& name , double value) : Element(name, value) { }

   virtual ElementType type() const {
      return T_CAPACITOR ;
   }

   virtual string formula() const {
      return "  s" + this->_name + "  " ;
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
   Inductor(const string& name , double value) : Element(name, value) { }

   virtual ElementType type() const {
      return T_INDUCTOR ;
   }

   virtual string formula() const {
      return "1 / s" + this->_name ;
   }

   virtual int order() const {
      return -1;
   }

   virtual double value() const {
      return 1.0 / this->_value ;
   }
};

class Resistor : public Element {
public:
   Resistor(const string name , double value) : Element(name, value) { }

   virtual ElementType type() const {
      return T_RESISTOR ;
   }

   virtual string formula() const {
      return "1 /  " + this->_name ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return 1.0 / this->_value ;
   }
};

class VCCS : public Element {
protected:
   char _sign ; // +1 for positive, -1 for negative
public:
   VCCS(const string name , double value , char sign) : Element(name, value) , _sign(sign){ }

   virtual ElementType type() const {
      return T_VCCS ;
   }

   virtual string formula() const {
      return "   " + this->_name + "  " ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return this->_value ;
   }

   virtual char sign() const {
      return _sign ;
   }
};

class Dummy : public Element {
protected:
   char _sign ; // +1 for positive, -1 for negative
public:
   Dummy(const string name , char sign) : Element(name, 1.0), _sign(sign) { }

   virtual char sign() const {
      return _sign ;
   }

   virtual ElementType type() const {
      return T_DUMMY ;
   }

   virtual string formula() const {
      return " Dummy " ;
   }

   virtual int order() const {
      return 0;
   }

   virtual double value() const {
      return 1.0 ;
   }
};

#endif /* __ELEMENT_H__ */
