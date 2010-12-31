%{
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "parser.h"
#include "element.h"

#define yyin parsein

using namespace std;

extern int yylex(void) ;
extern int yyparse(void) ;
//extern YYSTYPE yylval ;

void yyerror(const char *str) {
   cerr << "error: " << str << endl ;
}

int yywrap() {
   return 1;
}

Parser * currentParser ;
%}

%union {
   int iv;
   double dv;
   const char * sv;
};

%token <sv> T_RID T_LID T_CID T_VSRC T_ISRC
%token <iv> T_INTEGER
%token <dv> T_DOUBLE T_PREFIX
%token <sv> T_FNAME T_GID
%token T_FREQ T_TIME T_OUT T_EOL 

%type <dv> value double

%%

netlist:
   |  netlist line
   ;

line:
      element T_EOL
   |  simulate T_EOL
   |  T_EOL
   ;

element:
      resistor
   |  inductor
   |  capacitor
   |  vsrc
   |  isrc
   |  vccs
   |  out
   ;

simulate:
      freq
   |  time
   ;

out:
      T_OUT T_INTEGER T_INTEGER {
         printf("T_OUT(%d,%d)\n" , $2,$3);
         currentParser->addOut($2 , $3) ;
      }
   ;

resistor:
      T_RID T_INTEGER T_INTEGER value {
         printf("T_R(%d,%d,%lf)\n" , $2,$3,$4);
         currentParser->addR($1 , $2 , $3 , $4) ;
      }
   ;

inductor:
      T_LID T_INTEGER T_INTEGER value {
         printf("T_L(%d,%d,%lf)\n" , $2,$3,$4);
         currentParser->addL($1 , $2 , $3 , $4) ;
      }
   ;

capacitor:
      T_CID T_INTEGER T_INTEGER value {
         printf("T_C(%d,%d,%lf)\n" , $2,$3,$4);
         currentParser->addC($1 , $2 , $3 , $4) ;
      }
   ;

vccs:
      T_GID T_INTEGER T_INTEGER T_INTEGER T_INTEGER value {
         printf("T_VCCS(%d,%d,%d,%d,%lf)\n",$2,$3,$4,$5,$6) ;
         currentParser->addG($1 , $2 , $3 , $4 , $5 , $6) ;
      }
   ;

vsrc:
      T_VSRC T_INTEGER T_INTEGER value       {
         printf("T_VSRC(%d,%d,%lf,%lf)\n",$2,$3,$4,$4);
         currentParser->addV($1 , $2 , $3 , $4 , $4) ;
      }
   |  T_VSRC T_INTEGER T_INTEGER value value {
         printf("T_VSRC(%d,%d,%lf,%lf)\n",$2,$3,$4,$5);
         currentParser->addV($1 , $2 , $3 , $4 , $5) ;
      }
   ;

isrc:
      T_ISRC T_INTEGER T_INTEGER value       {
         printf("T_ISRC(%d,%d,%lf,%lf)\n",$2,$3,$4,$4);
         currentParser->addI($1 , $2 , $3 , $4 , $4) ;
      }
   |  T_ISRC T_INTEGER T_INTEGER value value {
         printf("T_ISRC(%d,%d,%lf,%lf)\n",$2,$3,$4,$5);
         currentParser->addI($1 , $2 , $3 , $4 , $5) ;
      }
   ;

double:
      T_INTEGER  {$$ = (double) $1;}
   |  T_DOUBLE   {$$ = $1;}
   ;

value:
      double   {$$ = $1;}
   |  double T_PREFIX {$$ = $1 * $2 ;}
   ;

freq:
      T_FREQ value value T_INTEGER T_FNAME {
         printf("T_FREQ(%lf,%lf,%d,%s)\n",$2,$3,$4,$5);
         currentParser->addFreqSwap($2 , $3 , $4 , $5) ;
      }
   ;

time:
      T_TIME value value value T_FNAME {
         printf("T_TIME(%lf,%lf,%lf,%s)\n",$2,$3,$4,$5);
         currentParser->addTimeSwap($2 , $3 , $4 , $5) ;
      }
   ;

%%
Parser::Parser(const char * fname = NULL) {
   extern FILE * yyin ;
   extern int yyparse() ;
   extern int parseErrorCount ;

   if (fname != NULL) {
      if (!(yyin = fopen(fname, "r"))) {
         cerr << '"' << fname << '"' << " can't be opened." << endl ;
      }
   }

   currentParser = this ;

   if (yyparse() != 0 || parseErrorCount != 0) {

   }
}

void Parser::addFreqSwap(double start , double end , int pointPerDecade , const char * fname) {

}

void Parser::addTimeSwap(double start , double end , double step , const char * fname) {

}

void Parser::setRLC(Element * element , int n1 , int n2) {
   Node * node1 = circuit.getNodeById(n1) ;
   Node * node2 = circuit.getNodeById(n2) ;

   circuit.pushElement(element) ;
   node1->setConnect(node2 , element) ;
   node2->setConnect(node1 , element) ;
}

void Parser::addR(const char * name , int n1 , int n2 , double v) {
   this->setRLC(new Resistor(name , v) , n1 , n2) ;
}

void Parser::addL(const char * name , int n1 , int n2 , double v) {
   this->setRLC(new Inductor(name , v) , n1 , n2) ;
}

void Parser::addC(const char * name , int n1 , int n2 , double v) {
   this->setRLC(new Capacitor(name , v) , n1 , n2) ;
}

void Parser::addV(const char * name , int n1 , int n2 , double v1 , double v2) {
   if(n1 == n2) {
      if(v1 != 0.0 || v2 != 0.0) {
         throw ParseError("Go Die") ;
      }
   }

   Node * node1 = circuit.getNodeById(n1) ;
   Node * node2 = circuit.getNodeById(n2) ;

   Element * element = new VSRC(name , v1) ;
   Element * rev_ele = new VSRC(name , v1) ;

   circuit.pushElement(element) ;
   circuit.pushElement(rev_ele) ;

   node1->setConnect(node2 , element);
   node2->setConnect(node1 , rev_ele);
}

void Parser::addI(const char * name , int n1 , int n2 , double v1 , double v2) {
   if(n1 == n2) {
      return ;
   }

   Node * node1 = circuit.getNodeById(n1) ;
   Node * node2 = circuit.getNodeById(n2) ;

   Element * element = new ISRC(name , v1) ;
   Element * rev_ele = new ISRC(name , v1) ;

   circuit.pushElement(element) ;
   circuit.pushElement(rev_ele) ;

   node1->setConnect(node2 , element);
   node2->setConnect(node1 , rev_ele);
}

void Parser::addG(const char * name , int n1 , int n2 , int n3 , int n4 , double v) {
   Element * element = new VCCS(name , v , 1) ;
   Element * rev_ele = new VCCS(name , v ,-1) ;

   Node * node1 = circuit.getNodeById(n1) ;
   Node * node2 = circuit.getNodeById(n2) ;
   Node * node3 = circuit.getNodeById(n3) ;
   Node * node4 = circuit.getNodeById(n4) ;

   circuit.pushElement(element) ;
   circuit.pushElement(rev_ele) ;

   if(n3 != n2) node3->setConnect(node2 , element) ;
   if(n3 != n1) node3->setConnect(node1 , rev_ele) ;
   if(n4 != n2) node4->setConnect(node2 , rev_ele) ;
   if(n4 != n1) node4->setConnect(node1 , element) ;
}

void Parser::addOut(int n1 , int n2) {
   circuit.outputHighId = n1 ;
   circuit.outputLowId = n2 ;
}
