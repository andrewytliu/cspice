#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {

  if(argc != 3) {
    printf("USAGE: %s [POINT COUNT] [OUTPUT FILE]", argv[0]);
    return 0;
  }

  freopen(argv[2], "w", stdout);

  printf("IIN 1   0   0 1m\n");
  printf("OUT 4   0\n");
  printf("L1  1   2   3.0\n");
  printf("R1  2   3   1.5\n");
  printf("C1  3   0   2.0\n");
  printf("GM  1   0   4   0   10.0\n");
  printf("R2  4   0   10.0\n");
  printf("L2  4   0   2.0\n");
  printf("C2  4   0   3.0\n");

  printf("FREQ 0.02 0.2 %d IIN freq.eps\n", 40 * atoi(argv[1]));
  printf("TIME 0.00 25.0 0.5 time.eps\n");

  return 0;
}

