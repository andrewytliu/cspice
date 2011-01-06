%{
#include <iostream>
#include <cstdlib>
#include "parseYY.hpp"
using namespace std ;

#define E_RLC 1
#define E_IV  2
#define E_GM  3
#define E_OUT 4
#define S_FREQ 5
#define S_TIME 6

#define yylval parselval
#define yyerror parseerror

int current= 0;
int amount = 0;
const char * escape(const char * text) ;
extern void yyerror(const char *) ;

%}
%option yylineno

EOL   \n
WS    [ \t]
EOT   ({WS}|{EOL})
ALPHA [A-Z]
alpha [a-z]
DIGIT [0-9]

DOUBLE  ([-+]?{DIGIT}+(\.)({DIGIT}*))
INTEGER ([-+]?{DIGIT}+)

MILLI m
MICRO u
NANO  n
PICO  p
FEMTO f

KILO  [Kk]
MEGA  M
GIGA  G
TERA  T

PREFIX ({MILLI}|{MICRO}|{NANO}|{PICO}|{FEMTO}|{KILO}|{MEGA}|{GIGA}|{TERA})

TIME  ((TIME)|(time))
FREQ  ((((((FREQ)U?)E?)N?)C?)Y?)

R     R
L     L
C     C
V     V
I     I
GM    G
NID   ({DIGIT}+)
EID   (({ALPHA}|{alpha}|{DIGIT})+)

FNAME (([0-9a-zA-Z\.\-_])+\.eps)

%s INT

%s VALUE

%s R_PREFIX
%s R_FNAME
%s R_SRC

%%
#.*$ {
   /* any thing after '#' is a comment */
}

{WS}  {
  /*  Ignore */
}

{EOL} {
   /*printf("\n");*/
   /* reset */
   BEGIN INITIAL ;
   amount = current = 0;
   return '\n' ;
}

<INITIAL>^{GM}{EID}/{WS} {
   /*printf("[VCCS(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INT;
   amount = 4 ;
   current = E_GM;
   return T_GID;
}

<INITIAL>^{V}{EID}/{WS} {
   /*printf("[Voltage(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INT;
   amount = 2;
   current = E_IV ;
   return T_VSRC;
}

<INITIAL>^{I}{EID}/{WS} {
   /*printf("[Current(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INT ;
   amount = 2;
   current = E_IV ;
   return T_ISRC;
}

<INITIAL>^{R}{EID}/{WS} {
   /*printf("[Resistor(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INT;
   amount = 2;
   current = E_RLC ;
   return T_RID;
}

<INITIAL>^{L}{EID}/{WS} {
   /*printf("[Inductor(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INT;
   amount = 2;
   current = E_RLC ;
   return T_LID;
}

<INITIAL>^{C}{EID}/{WS} {
   /*printf("[Capacitor(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INT;
   amount = 2;
   current = E_RLC ;
   return T_CID;
}

<INITIAL>^OUT/{WS} {
   /*printf("[OUT]");*/
   BEGIN INT;
   amount = 2 ;
   current = E_OUT ;
   return T_OUT;
}

<R_SRC>({V}|{I}){EID}/{WS} {
   /*printf("[Source(%s)]",yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN R_FNAME;
   return T_SRC;
}

<INT>{INTEGER} {
   /*printf("INTEGER(%s)",yytext);*/
   yylval.iv = atoi(yytext) ;
   if ((-- amount) == 0) {
      switch(current) {
         case E_RLC :
            BEGIN VALUE ; amount = 1 ; break ;
         case E_IV  :
            BEGIN VALUE ; amount = 2 ; break ;
         case E_GM  :
            BEGIN VALUE ; amount = 1 ; break ;
         case E_OUT :
            BEGIN INITIAL;current = 0 ; break ;
      }
   }
   return T_INTEGER;
}

<VALUE>{INTEGER}/{EOT} {
   /*printf("INTEGER(%s)",yytext);*/
   yylval.iv = atoi(yytext) ;
   if ((-- amount) == 0) {
      switch(current) {
         case E_RLC :
         case E_IV  :
         case E_GM  :
         case E_OUT :
            BEGIN INITIAL;current = 0 ; break ;
         case S_FREQ:
            BEGIN R_SRC; break ;
         case S_TIME:
            BEGIN R_FNAME ; break ;
      }
   }
   return T_INTEGER;
}

<VALUE>{DOUBLE}/{EOT} {
   /*printf("DOUBLE(%s)",yytext);*/
   yylval.dv = atof(yytext) ;
   if ((-- amount) == 0) {
      switch(current) {
         case E_RLC :
         case E_IV  :
         case E_GM  :
         case E_OUT :
            BEGIN INITIAL;current = 0 ; break ;
         case S_FREQ:
            BEGIN R_SRC; break ;
         case S_TIME:
            BEGIN R_FNAME ; break ;
      }
   }
   return T_DOUBLE;
}

<VALUE>({DOUBLE}|{INTEGER})/{PREFIX}{EOT} {
   /*printf("DOUBLE(%s)",yytext);*/
   yylval.dv = atof(yytext) ;
   BEGIN R_PREFIX;
   return T_DOUBLE;
}

<R_PREFIX>{PREFIX} {
   /*printf("PREFIX(%s)",yytext);*/
   switch(*yytext) {
      case 'm': yylval.dv = 1e-3; break;
      case 'u': yylval.dv = 1e-6; break;
      case 'n': yylval.dv = 1e-9; break;
      case 'p': yylval.dv = 1e-12; break;
      case 'f': yylval.dv = 1e-15; break;
      case 'k': yylval.dv = 1e+3; break;
      case 'K': yylval.dv = 1e+3; break;
      case 'M': yylval.dv = 1e+6; break;
      case 'G': yylval.dv = 1e+9; break;
      case 'T': yylval.dv = 1e+12; break;
   }
   if ((-- amount) == 0) {
      switch(current) {
         case E_RLC :
         case E_IV  :
         case E_GM  :
         case E_OUT :
            BEGIN INITIAL;current = 0 ; break ;
         case S_FREQ:
            BEGIN R_SRC ; amount = 1; break ;
         case S_TIME:
            BEGIN R_FNAME ; amount = 1; break ;
      }
   } else {
      BEGIN VALUE ;
   }
   return T_PREFIX;
}

<INITIAL>^{TIME}/{WS} {
   /*printf("[Time]");*/
   BEGIN VALUE ;
   current = S_TIME ;
   amount = 3 ;
   return T_TIME;
}

<INITIAL>^{FREQ}/{WS} {
   /*printf("[Frequency]");*/
   BEGIN VALUE ;
   amount = 3;
   current = S_FREQ;
   return T_FREQ;
}

<R_FNAME>{FNAME} {
   /*printf("File(%s)" , yytext);*/
   yylval.sv = new char[yyleng] ;
   strcpy(yylval.sv , yytext) ;
   BEGIN INITIAL ;
   return T_FNAME;
}

.  {
      /* rest cases */
      char msg[40] ;
      sprintf(msg , "Unexpected character: '%s'" , escape(yytext)) ;
      yyerror(msg) ;
      return yytext[0] ;
   }
%%

int yywrap(void) {
   return 1;
}

const char * escape(const char * text) {
   switch(text[0]) {
      case '\n':
         return "\\n" ;
      case '\t':
         return "\\t" ;
      case '\b':
         return "\\b" ;
      case '\a':
         return "\\a" ;
      case '\\':
         return "\\\\" ;
      case '\'':
         return "\\'" ;
      default:
         return text ;
   }
}
