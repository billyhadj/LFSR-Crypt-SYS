#include "f2_poly.h"
#include<stdlib.h>
#include "arithm.h"

int main(int argc, char const *argv[]) {
  f2_poly_t polAES = 0x11b;     //X^8+X^4+X^3+X+1, irréductible non primitif
  f2_poly_t polA51a = 0xe4001;  //X^19+X^18+X^17+X^14+1, irréductible primitif
  f2_poly_t polA51b = 0x600001; //X^22+X^21+1, irréductible primitif
  f2_poly_t polA51c = 0xe00101; //X^23+X^22+X^21+X^8+1, irréductible primitif
  /*printf("%ld\n",f2_poly_deg(polA51a)) ;
  f2_poly_print(polA51a, 'A', stdout);
  f2_poly_print(f2_poly_rem(polA51a,polAES),'x',stdout);
  f2_poly_print(f2_poly_rem(polA51c,polA51b),'x',stdout);*/
  f2_poly_t* R , *Q ;
  R = malloc(sizeof(f2_poly_t));
  Q = malloc(sizeof(f2_poly_t));
  printf("yoh\n");
  f2_poly_div(Q , R , polA51c , polA51b);
  f2_poly_print(*R ,'Y' , stdout);
  f2_poly_print(*Q , 'X', stdout);
  //f2_poly_print(f2_poly_gcd(polA51c,polA51a), 'x', stdout);
  f2_poly_print(f2_poly_xtimes(polA51c,polA51a), 'x', stdout);
  //f2_poly_div()
  f2_poly_print(polA51b, 'c', stdout);
  f2_poly_print(f2_poly_times(polA51c, polA51b, polA51a),'x',stdout);
  f2_poly_print(f2_poly_x2n(0,polA51b), 'D',stdout);
  f2_poly_print(f2_poly_derive(polA51a),'H',stdout);
  f2_poly_print(f2_poly_random(10),'X',stdout);
  printf("Est ce que le polynome : ");
  f2_poly_print(polA51c,'X',stdout) ;
  printf("est irreductible : %s \n", f2_poly_irred(polA51c)? "yes" : "no" );
  printf("Est ce que le polynome : ");
  f2_poly_print(0xe4001,'X',stdout) ;
  printf("est primitif : %s \n", f2_poly_primitive(0xe4001)? "yes" : "no" );
  return 0;
}
