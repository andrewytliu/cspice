#include <stdio.h>
#include <stdlib.h>

const int loopcnt = 7;

inline double frand() {
   return 1.0;
   return (double)rand()/RAND_MAX;
}

int main(void)
{
   int i;
   int id=0;
   srand(514);
   freopen("infiniteRH.out","w",stdout);
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
