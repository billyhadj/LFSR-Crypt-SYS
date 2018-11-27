#include <inttypes.h>
#include<stdio.h>
#include<stdlib.h>
#include"lfsr.h"


int main (int argc, char* argv[]){
printf("Début de programme bien défini\n");
  container_t *cont = malloc(sizeof(container_t)) ;
  printf("Déclaration du pointeur sur un container fait avec succes\n");
  lfsr_t *lfsr1 =malloc(sizeof(lfsr_t));
  printf("Déclaration du pointeur sur un lfsr fait avec succes\n");
  lfsr_container_init(cont, 0x80,0x71);
  lfsr_init(lfsr1,0,8,cont) ;
  printf("Initialisation du container et du LFSR avec les paramètre de l'exemple.txt fait avec succes\n");
  printf("Affichage du registre : %lx \n", lfsr1->cont->regist);
  printf("Affichage du polynome : %lx \n", lfsr1->cont->poly);
  printf("Affichage de toutes les valeurs de l'incrémentation du LFSR de l'exemple.txt\n");
  for(int i = 0; i< 255 ; i++){
      lfsr_inc(lfsr1);
      printf("Incrémentation n° : %4d          valeur du registre : %4lx \n",i+1, lfsr1->cont->regist);
  }

  printf("Test de la fonction de période pour l'exemple du cours : %d\n",lfsr_period(lfsr1));

  printf("Fin des tests pour les fonctions :\n-lfsr_container_init()\n-lfsr_init()\n-lfsr_inc()\n-lfsr_period()\n");
  return 0;
}
