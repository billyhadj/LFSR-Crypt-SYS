#include <stdio.h>
#include <stdlib.h>
#include"lfsr.h"

//Defintion de la fonction int lfsr_container_init (container_t *, const maxlen_t, maxlen_t)
int lfsr_container_init (container_t* cont , const maxlen_t reg, maxlen_t pol){
  cont->regist = reg ;
  cont->poly = pol ;
  return 1 ;
}

//Defintion de la fonction int lfsr_init()
int lfsr_init (lfsr_t * lfsr1, const byte_t zero, const byte_t len, container_t
    * cont){
  if(zero <= MAXLEN && len <= MAXLEN && (zero+len-1)<=MAXLEN){
    lfsr1->zero = zero ;
    lfsr1->len = len ;
    lfsr1->cont = cont ;
    return 1 ;
  }else{
    return 0;
  }
}

void lfsr_inc (lfsr_t * lfsr ){
  maxlen_t tmp = ((lfsr->cont->regist)&LFSR_MASK(lfsr));
  maxlen_t xor = tmp & lfsr->cont->poly;
  int res_bite = 0;
  for(int i = 0 ; i < lfsr->len ; i++){
    res_bite^= ((xor>>(i + lfsr->zero))&1);
  }
  lfsr->cont->regist &= ~LFSR_MASK(lfsr);
  lfsr->cont->regist ^= (((tmp>>1)^(res_bite<<(lfsr->len+lfsr->zero-1)))&LFSR_MASK(lfsr)) ;

}
int is_here(maxlen_t a, maxlen_t* tab, int size){
  for(int i = 0 ; i<size ; i++){
    if(a==tab[i])
    return 1 ;
  }
  return 0;
}

int lfsr_period(lfsr_t * lfsr){
  maxlen_t *tab =malloc(sizeof(maxlen_t)*256);
  tab[0] =lfsr->cont->regist ;
  lfsr_inc(lfsr);
  int i = 1 ;
  while(!is_here(lfsr->cont->regist,tab , 250)){
    tab[i] =lfsr->cont->regist ;
    lfsr_inc(lfsr);
    i++ ;
  }
  return i ;
}
