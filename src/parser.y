%{
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "parser.h"
#include "element.h"

#define yyin parsein
#define yylineno parselineno

using namespace std;

extern int yylex(void) ;
extern int yyparse(void) ;
//extern YYSTYPE yylval ;

extern void yyerror(const char *str) {
   //cerr << "error: " << str << endl ;
   extern int yylineno ;
   stringstream msg ;
   msg << "At line " << yylineno << ", " << str << endl ;
   throw ParseError(msg.str());
}

int yywrap() {
   return 1;
}

Parser * currentParser ;
%}

%defines
%error-verbose

%union {
   int iv;
   double dv;
   char * sv;
};

%token <sv> T_RID T_LID T_CID T_VSRC T_ISRC T_FNAME T_GID T_SRC
%token <iv> T_INTEGER
%token <dv> T_DOUBLE T_PREFIX
%token T_FREQ T_TIME T_OUT

%type <dv> value double

%%

netlist:
   |  netlist line
   ;

line:
      element '\n'
   |  simulate '\n'
   |  '\n'
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
//         printf("OUT %d %d \n" , $2,$3);
         currentParser->addOut($2 , $3) ;
      }
   ;

resistor:
      T_RID T_INTEGER T_INTEGER value {
//         printf("%s %d %d %lf\n",$1,$2,$3,$4);
         currentParser->addR($1 , $2 , $3 , $4) ;
         delete [] $1 ;
      }
   ;

inductor:
      T_LID T_INTEGER T_INTEGER value {
//         printf("%s %d %d %lf\n" ,$1,$2,$3,$4);
         currentParser->addL($1 , $2 , $3 , $4) ;
         delete [] $1 ;
      }
   ;

capacitor:
      T_CID T_INTEGER T_INTEGER value {
//         printf("%s %d %d %lf\n" ,$1,$2,$3,$4);
         currentParser->addC($1 , $2 , $3 , $4) ;
         delete [] $1 ;
      }
   ;

vccs:
      T_GID T_INTEGER T_INTEGER T_INTEGER T_INTEGER value {
//         printf("%s %d %d %d %d %lf)\n",$1,$2,$3,$4,$5,$6) ;
         currentParser->addG($1 , $2 , $3 , $4 , $5 , $6) ;
         delete [] $1 ;
      }
   ;

vsrc:
      T_VSRC T_INTEGER T_INTEGER value       {
//         printf("%s %d %d %lf %lf\n",$1,$2,$3,$4,$4);
         currentParser->addV($1 , $2 , $3 , $4 , $4) ;
         delete [] $1 ;
      }
   |  T_VSRC T_INTEGER T_INTEGER value value {
//         printf("%s %d %d %lf %lf\n",$1,$2,$3,$4,$5);
         currentParser->addV($1 , $2 , $3 , $4 , $5) ;
         delete [] $1 ;
      }
   ;

isrc:
      T_ISRC T_INTEGER T_INTEGER value       {
//         printf("%s %d %d %lf %lf\n",$1,$2,$3,$4,$4);
         currentParser->addI($1 , $2 , $3 , $4 , $4) ;
         delete [] $1 ;
      }
   |  T_ISRC T_INTEGER T_INTEGER value value {
//         printf("%s %d %d %lf %lf\n",$1,$2,$3,$4,$5);
         currentParser->addI($1 , $2 , $3 , $4 , $5) ;
         delete [] $1 ;
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
      T_FREQ value value T_INTEGER T_SRC T_FNAME {
//         printf("FREQ %lf %lf %d %s %s\n",$2,$3,$4,$5,$6);
         currentParser->addFreqSwap($2 , $3 , $4 , $5 , $6) ;
      }
   ;

time:
      T_TIME value value value T_FNAME {
//         printf("TIME %lf %lf %lf %s\n",$2,$3,$4,$5);
         currentParser->addTimeSwap($2 , $3 , $4 , $5) ;
      }
   ;

%%

#include "parser.cpp"

