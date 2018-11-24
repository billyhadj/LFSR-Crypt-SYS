#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <error.h>
#include "f2_poly.h"
#include "arithm.h"


size_t
f2_poly_deg(f2_poly_t p){// La fonction qui calcule le degré d'un polynome.
  size_t res = LEN-1 ; // Nous prenons un entier égal au degrès maximale que peut avoir un polynome
  while(!((p>>res) & 1)){ // Une boucle qui permet de parcourir les bits
    if(res<0)// jusqu'à arriver au premier bit non nul,
      return -1 ;// Si le polynome est égale à 0, nous retournons -1
    res-- ;
  }
  return res ;
}

int
f2_poly_print(f2_poly_t p, const char c , FILE * fd){
  int i ;
  for(i = LEN -1; i>1 ; i-- ){ // Une boucle qui permet de parcourir les bits
      if((p>>i) & 1){ // à chaque fois qu'un bit est égale à 1
        fprintf(fd," + %c^%d ",c,i); // On l'affiche par le caractere donné argument
      }
  }
  if(p&(1<<i)){ // le cas ou nous arrivons au monome X
    fprintf(fd, " + %c ", c);
  }
  if(p&(1)){ // Le cas ou nous arrivons au monome constant
    fprintf(fd," + 1 ");
  }
  fprintf(fd,"\n");// Pour plus de lisibilité, après un affichage, on revient a la ligne
  return 0;
}

f2_poly_t
f2_poly_rem(f2_poly_t p, f2_poly_t q){
  if (q){ // condition : q is not 0
    f2_poly_t res = p ;
    if(f2_poly_deg(p)< f2_poly_deg(q)){ // Le cas ou le polynome dividande est plus petit que le divseur,
      return p ; // dans ces cas la, le reste de la division euclidienne c'est le diviseur
    }
    else { // Sinon
      size_t deg_diff = f2_poly_deg(p)-f2_poly_deg(q) ; // On stocke la difference des deux polynomes
      while (f2_poly_deg(res)>= f2_poly_deg(q)) { // tant que le polynome dividande est plus grand que le diviseur
        res^=(q<<deg_diff) ; // Un Xor ici équivaut à une soustraction dans F2 (on soustrait le polynome diviseur decalé de q)
        deg_diff = f2_poly_deg(res)-f2_poly_deg(q); // on remet un jour la difference des degré des polynomes
      }
    }
    return res ; // on retourne le reste.
  } else {
    return 0 ;
  }
}


int
f2_poly_div(f2_poly_t * R ,f2_poly_t * Q , f2_poly_t dnd, f2_poly_t der){
  *Q = 0 ;// On initialise Q à 0
  *R = 0 ; // On initialise R à 0 également (la valeur pointée et non le pointeur )
  if (dnd){ // condition : dnd is not 0
    if(f2_poly_deg(dnd)< f2_poly_deg(der)){ // Ici, c'est le cas ou le degré du diviseur est plus grand que celui du dividande
      return dnd ; // on retourne donc uniquement le dividande
    }
    else { // Sinon
      *R = dnd ; // On initialise la valeur pointée par le pointeur R à dnd
      size_t deg_diff = f2_poly_deg(*R);
      while (deg_diff >= f2_poly_deg(der)) { // tant que le degré du dividande est plus grande que celle du diviseur
        if(((*R)>> deg_diff) & 1 ){ // Si le bit a la position deg_diff est égale à 1
          *R^=(der<<(deg_diff-f2_poly_deg(der))) ; // On soustrait à R le diviseur (Un Xor equivaut à une soustraction dans F2)
          *Q^=(1 << (deg_diff -f2_poly_deg(der))) ; // On ajoute à Q un bit à 1 à la position correspondant à la division
        }
        deg_diff-- ; // on met à jour le nouveau degrés
      }
    }
    return 0 ;
  } else {
    return 0 ;
  }
}

f2_poly_t
f2_poly_gcd(f2_poly_t a, f2_poly_t b){
  f2_poly_t d = 0;

  // a et b sont pairs, division par 2 autant que possible et calcul de d
  while (((a&1)==0)&&((b&1)==0)){
      a>>=1; // a/2
      b>>=1; // b/2
      d+=1; // décalage (multiplication par 2 au résultat)
    }

  // un des deux est impair:
  while ((a&1)==0) {
    a>>=1; // a/2 tant que a est pair
  }
  while ((b&1)==0){
    b>>=1; // b/2 tant que b est pair
  }

  // les deux sont impairs
  while (a != b) { // Tant que les deux seront différents, on effectue des soustractions successives
    if (a > b) {a-=b;}
    else b-=a;
  }

  return a<<d;

}

f2_poly_t
f2_poly_xtimes(f2_poly_t a , f2_poly_t b ){ //
  return f2_poly_rem(a>>1 , b); // muliplier par le monome X revient à effectuer un décalage vers la droite
}

f2_poly_t
f2_poly_times(f2_poly_t a, f2_poly_t b , f2_poly_t n){ //
  f2_poly_t res = 0 ;// On initialise le resultat à  0
  size_t len_b = f2_poly_deg(b);// on saisie le degré de b
  for(size_t i = 0 ; i <= len_b ; i++ ){ // Nous allons boucler pour arriver jussqu'au degré du polynome b
    if((b>>i)&1){ // Si nous avons un 1 a la position i, alors
      res ^= f2_poly_rem((a<<(i)),n);// Nous effectuons un decaler de i position vers la droite (mupliplication par X^I) et reduison modulo N
    }
  }
  return res ;// On retourne le resulats
}

f2_poly_t
f2_poly_x2n(size_t pow, f2_poly_t b){
  f2_poly_t x = 2; // Nous allons multiplier par un monome de degrés une puissance de 2, donc on initilise un polynome à x^2
  for(size_t i = 0; i < pow ; i++){ // Nous allons boucler sur la valeur de la puissance de 2
    x = f2_poly_times(x,x,b); // Et nous allons appliquer la fonction de multiplication sur x par x afin d'avoir des puissance de x en puissance de 2
  }
  return x ;
}

f2_poly_t
f2_poly_parity(f2_poly_t P){
  return f2_poly_rem(P,3); // Ici, on aurait pu faire de deux façon différente, Soit on rend le reste de la division euclidienne par X+1
                              // Soit on retourne la somme de tous les bits modulo 2 dans F2
}

f2_poly_t
f2_poly_derive(f2_poly_t P){
  return((P&UNZERO)>>1); // Ici, lorsque l'on dérive un polynome dans F2 c'est equivalent à décaler tous les bit d'une position
}                        // Sans oublier de mettre tous les bits en position pair à 0

f2_poly_t
f2_poly_xn(size_t n, f2_poly_t P){
  f2_poly_t res = 1;
  f2_poly_t tmp = 2;
  for (int i = 0 ; i < n ; i++) {
    if ( n&1 ) {
      res = f2_poly_times(res, tmp, P);
    }
    tmp = f2_poly_times(tmp, tmp, P);
    n >>= 1;
  }
  return res;
}

f2_poly_t
f2_poly_recip(f2_poly_t P){
  size_t taille = f2_poly_deg(P); // On stock la taille du polynome dans une variable
  f2_poly_t res = 0 ; // Pour cette fonction nous devons uniquement lire et stocker ses bits en sens inverse
  for(int i = taille ; i > 0 ; i--){ // On entame donc une boucle sur la taille du polynome
    res ^= P&(1<<i)<<(taille-i) ; // res qui était initialisé à 0 est incrémenté en lisant les bits de P en sens inverse
  }
  return res ; // on retourne le polynome ainsi construit
}


f2_poly_t
f2_poly_random_inf(size_t taille){
  f2_poly_t res = 0; // Nous initialisons le resulats à 0
  int alea ; // nous allons prendre une variable qui va gérer l'aléa
  srand(time(NULL)); // On Génère l'aléa grace a la fonction srand() et la fonction time()
  for(int i = 0 ; i< taille ; i++){ // Nous allons entamer une boucle
    alea = rand(); // choisir une valeur aléaoitre
    res += (alea % 2)<<i; // Et suivant que la variable choisie est pair ou impair (aléatoirement) on met un bit à 0 ou à 1
  }
  return res ; // on retourne ainsi le résultat
}


f2_poly_t // Toute la premiere partie de la fonction est semblable à la précédente
f2_poly_random(size_t taille){
  f2_poly_t res = 0;
  int alea , i;
  srand(time(NULL));
  for( i = 0 ; i<= taille-1 ; i++){
    alea = rand();
    res += (alea % 2)<<i;
  }
  res += 1<<i ; // Pour être sûr que le polynom soit de degré exactement égale à taille, on ajoute le bit à la position "taille" (manuellement)
  return res ;  // On retourne le resultats
}

int
f2_poly_irred(f2_poly_t p){
  int deg_p = f2_poly_deg(p) ;// On Stocke la valeur du degré de p
  if(!(p&&1)){ // ici, le cas ou le polynom est divisble par le monome X
    return 0 ;
  }
  if(!f2_poly_parity(p)){ // Le cas ou le polynom est pair (divisible par X+1)
    return 0;
  }
  uint64_t tmp_n ; // tmp_n et pp sont des variables qui vont nous servir pour les critères de divisibilités
  uint64_t pp ;
  f2_poly_t X2n_X = f2_poly_x2n(deg_p,p) ^ (1<<1); // On construit le polynome X^2^n - X modulo P
  if (X2n_X == 0){ // le cas ou le polynome X^2^n - X est divisible par le polynome P
    tmp_n = deg_p ; //
    while(tmp_n>1){// Dans cette boucle nous allons lister tous les diviseur premiers du degré du polynome
      pp = pp_diviseur_premier(tmp_n);
      f2_poly_t Xpn_X = f2_poly_x2n(deg_p/pp,p) ^(1<<1) ; // tester si le polynome du TD est premier avec le polynome P
      if(f2_poly_gcd(Xpn_X,p)>1){ // Si un moment, nous avons que le GCD est plus grand que 1 alors il renvoie 0 (non irreductible)
        return 0 ;
      }
      tmp_n = tmp_n/pp ;
    }
  }
  return 1 ;// Si le polynome passe tous les tests décrits en TD alors il est irreductible dans F2
}


int
f2_poly_primitive(f2_poly_t p){
  if(!f2_poly_irred(p)){ // on teste d'abord si le polynome est irreductible,
    return 0; //sinon, on retourne directement qu'il n'est pas primitif
  }
  size_t n = f2_poly_deg(p); //
  size_t tmp_n = (1ULL << n) - 1;
  uint64_t q;
  while (tmp_n > 1) { // Nous allons entamer une boucle pour tester chaque diviseur
    q = pp_diviseur_premier(tmp_n);
    if (f2_poly_xn(tmp_n/q, p) == 1) { // on regarde si le polynome
      return 0;
    }
    while (tmp_n%q == 0) {
      tmp_n /= q;
    }
  }
  return 1;
}
