#ifndef _RESISTOR_H_
#define _RESISTOR_H_
#include "circuit.h"

class Resistor : public Element {
public:
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

#endif //#ifndef _RESISTOR_H_
