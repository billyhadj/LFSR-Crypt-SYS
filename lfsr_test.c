#include <inttypes.h>
#include<stdio.h>
#include<stdlib.h>
#include"lfsr.h"


int main (int argc, char* argv[]){
printf("Début de programme bien défini\n");
  container_t *cont = malloc(sizeof(container_t)) ;
  lfsr_t *lfsr1 =malloc(sizeof(lfsr_t));
  lfsr_container_init(cont, 0x80,0x71);
  lfsr_init(lfsr1,0,8,cont) ;
  printf("%lx \n", lfsr1->cont->regist);
  for(int i = 0; i< 20 ; i++){
      lfsr_inc(lfsr1);
      printf("%lx \n", lfsr1->cont->regist);
  }
  printf("\n%d\n",lfsr_period(lfsr1));

  return 0;
}
