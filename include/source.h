#ifndef __SOURCE_H__

class Source {
   public:
      enum SourceType {
         VSRC,
         ISRC
      } ;

      Source() :
         _name() ,
         _prevValue(0.0) ,
         _nextValue(0.0) ,
         _n1(NULL) ,
         _n2(NULL) { }

      Source(const char * name , const int n1 , const int n2 , const double pv , const double nv) :
         _name(name) ,
         _prevValue(pv) ,
         _nextValue(nv) ,
         _n1(n1) ,
         _n2(n2) { }
      /*
       * I don't think we need a copy constructor
       */
      virtual ~Source() { }
      virtual SourceType type () const = 0 ;

      double prevValue() const { return _prevValue ; }
      double nextValue() const { return _nextValue ; }
      double pulseValue() const { return _nextValue - _prevValue; }
      string name() const { return _name ; }
      int node1() const { return _n1 ; }
      int node2() const { return _n2 ; }

   private:
      string _name ;
      double _prevValue ;
      double _nextValue ;
      int _n1 ;
      int _n2 ;
} ;

class VoltageSource : public Source {
   public:
      VoltageSource(const char * name , const int n1 , const int n2 , const double pv , const double nv) :
         Source(name , n1 , n2 , pv , nv) { }
      virtual SourceType type() const {
         return VSRC ;
      }
};

class CurrentSource : public Source {
   public:
      CurrentSource(const char * name , const int n1 , const int n2 , const double pv , const double nv) :
         Source(name , n1 , n2 , pv , nv) { }
      virtual SourceType type() const {
         return ISRC ;
      }
};

#endif // __SOURCE_H__
