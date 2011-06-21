#include <stdio.h>
#include <stdlib.h>

inline double frand() {
   return 1.0;
   return (double)rand()/RAND_MAX;
}

int main(int argc, char** argv)
{
   if(argc != 3) {
      printf("USAGE: %s [LOOP COUNT] [OUTPUT FILE]", argv[0]);
      return 0;
   }

   int i, loopcnt = atoi(argv[1]), id = 1;
   srand(514);
   freopen(argv[2],"w",stdout);
   printf("VIN 1 0 0 1m\n");
   printf("OUT 7 0\n");
   for(i=0;i<loopcnt;i++) {
      printf("R%d %d %d %.2lf\n",id++,i*2,i*2+2,frand());
      printf("R%d %d %d %.2lf\n",id++,i*2+1,i*2+3,frand());
      printf("R%d %d %d %.2lf\n",id++,i*2+2,i*2+3,frand());
   }
   printf("FREQ 0.02 0.2 40 VIN freq.eps\n");
   printf("TIME 0.00 25.0 0.5 time.eps\n");
   return 0;
}

