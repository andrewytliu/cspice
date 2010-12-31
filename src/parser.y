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

#include "parser.cpp"
