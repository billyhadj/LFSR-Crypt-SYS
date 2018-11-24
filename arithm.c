#include "arithm.h"
#include <math.h>

uint64_t
pp_diviseur_premier(uint64_t n) {
  if (!(n&1)) {
    return 2;
  }
  for (int i = 3; i <= sqrt(n); i+=2) {
    if (n%i == 0) {
      return i;
    }
  }
  return n;
}
