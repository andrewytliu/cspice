#ifndef _INDUCTOR_H_
#define _INDUCTOR_H_
#include "circuit.h"

class Inductor : public Element {
public:
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

#endif //#ifndef _INDUCTOR_H_
