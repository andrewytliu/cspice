#ifndef _CAPACITOR_H_
#define _CAPACITOR_H_
#include "circuit.h"

class Capacitor : public Element {
public:
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

#endif //#ifndef _CAPACITOR_H_
