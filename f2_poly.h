#ifndef F2_POLY_H
#define F2_POLY_H

#include <stdio.h>
#include <inttypes.h>
#include "arithm.h"
typedef uint64_t f2_poly_t;

// l'ordre des coefficients polynomiaux est du poids fort au poids faible.

#define F2_VARN 'X' // représentation des polynômes
#define SIZE (8) // sizeof(f2_poly_t)
#define LEN (SIZE << 3)
// Pour pouvoir tirer un polynôme au hasard
#define RANDOMFILE "/dev/urandom"
// Masque pour un polynôme de degré < d, calcule aussi 2^d - 1
#define MASK(d) (  ~((f2_poly_t)0xffffffffffffffffULL << (d)) )
// Masque pour les coefficients de degré impair (utile par ex. pour dériver)
#define UNZERO 0xaaaaaaaaaaaaaaaaULL

// le type des prédicats sur les polynômes
typedef int(*f2_poly_pred)(f2_poly_t);

/* Les polynômes sur F2 de degré < 64 sont représentés
 par des entiers de 64 bits, les coefficients sont ordonnés du poids
 fort vers le poids faible. Ex. 0x82 : X^7+X.
*/

/* Calcul sur F_2[X], pour des polynômes de degré au plus 63 */

size_t ////////////////////////////////////////////////////////////////////////
f2_poly_deg(f2_poly_t);
// degré du polynôme arg1 (qui est le poids maximum d'un bit à 1,
// c'est-à-dire le rang du "MSB", "most significant bit", ou encore,
// arg1 étant vu comme un entier la partie entière par valeur inférieure
// de log(arg1), log en base 2

int
f2_poly_print(f2_poly_t, char, FILE *); //////////////////////////////////////////////////////////
// écriture polynomiale usuelle, avec pour variable le second argument

f2_poly_t
f2_poly_rem(f2_poly_t, f2_poly_t); ////////////////////////////////////////////////////////////////
// reste de arg1 par arg2 (renvoie 0 même si le diviseur est nul)
// algorithme de l'école primaire

int
f2_poly_div(f2_poly_t *,f2_poly_t *, f2_poly_t, f2_poly_t);//////////////////////////////////////////
// (arg1, arg2) = (quotient, reste) de arg3 par arg4, gestion des erreurs
// algorithme de l'école primaire

f2_poly_t
f2_poly_gcd(f2_poly_t, f2_poly_t);/////////////////////////////////////////////////////////////
// pgcd(arg1, arg2) (algorithme d'Euclide)

/* Pour tous les calculs modulo P, on peut  quotienter par un
polynôme P de degré jusqu'à 63. */

f2_poly_t
f2_poly_xtimes(f2_poly_t, f2_poly_t);///////////////////////////////////////////////////////
// retourne X*arg1 mod (arg2)

f2_poly_t
f2_poly_times(f2_poly_t, f2_poly_t, f2_poly_t);///////////////////////////////////////////////
// retourne arg1 * arg2 modulo  arg3
// utiliser l'algorithe de l'école primaire (pas de retenue !),
// calculer le reste modulo arg3 à chaque étape.

f2_poly_t
f2_poly_x2n(size_t, f2_poly_t);/////////////////////////////////////////
// retourne X^{2^arg1} modulo arg2

f2_poly_t
f2_poly_xn(size_t, f2_poly_t);
// retourne X^{arg1} modulo arg2
// utiliser un algorithme en log n itérations en utilisant que :
// Si n = Sum a_i * 2^i, X^n = Prod (X^{2^i})^a_i

f2_poly_t
f2_poly_parity(f2_poly_t);////////////////////////////////////////////////////
//retourne le reste de la division par X+1 (xor des bits) Il est
// possible de "paralléliser", par exemple cette transformation conserve
// la parité (cas de 64 bits) :
// polyP ^= (polP >> (LEN<< 1)); polP &= 0x00000000ffffffffULL
// (il suffit ensuite d'itérer, et le "&=" est en fait inutile : il peut
// se faire une seule fois en fin de calcul)

f2_poly_t
f2_poly_derive(f2_poly_t);//////////////////////////////////////////////////////
//retourne le polynôme dérivé (sur F2) de arg1

f2_poly_t
f2_poly_recip(f2_poly_t);//////////////////////////////////////////////////////
// Retourne le polynôme réciproque (nécessite de calculer le degré !)
// Il est possible de paralléliser pour renverser l'ordre des bits, par
// exemple sur 64 bits cette transformation échange les bits de rang
// pair et impair :
//     polP = ((polP & 0x5555555555555555ULL) << 1)
//	  ^ ((polP & 0xaaaaaaaaaaaaaaaaULL) >> 1);
// Itérer ensuite en échangeant par paquet de 2, de 4 etc.
// Il faut ensuite décaler en fonction du degré.

int
f2_poly_irred(f2_poly_t);//////////////////////////////////////////////////////
// vérifie si le polynôme arg1 est irréductible (cf. feuille de TD)
// Il est possible d'optimiser en vérifiant d'abord
// que le polynôme n'est divisible ni par X, ni par X+1

int
f2_poly_primitive(f2_poly_t);
// vérifie si le polynôme arg1 est primitif (cf. feuille de TD)

size_t
f2_poly_irred_order(f2_poly_t);
// renvoie 0 si le polynôme arg1 n'est pas irréductible,
// l'ordre multiplicatif d'une racine, soit de  X modulo arg1 sinon
// Si cet ordre égale 1 << f2_poly-deg(P), le polynôme est primitif.

f2_poly_t
f2_poly_random_inf(size_t);//////////////////////////////////////////////////
// retourne un polynôme tiré au hasard parmi les polynômes de degré < arg

f2_poly_t
f2_poly_random(size_t);////////////////////////////////////////////////////////
// retourne un polynôme tiré au hasard parmi les polynômes de degré = arg

f2_poly_t
f2_poly_prop_random(size_t, f2_poly_pred);
// retourne un polynôme tiré au hasard parmi les polynômes de degré = arg1
// vérifiant la propriété arg2
// Pour les tests :
// * tirer un polynôme irréductible au hasard de degré donné
// * tirer un polynôme primitif au hasard de degré donné

size_t
f2_poly_count(size_t, f2_poly_pred, FILE *, FILE *);
// compte le nombre de polynômes de degré arg1 vérifiant la propriété arg2
// si arg3 != 0 ces polynômes sont sauvés sur arg3 au format binaire
// si arg4 != 0 ces polynômes sont sauvés sur arg4 au format texte





////***************************************************************************
size_t
f2_poly_count_recip(size_t, f2_poly_pred,FILE *, FILE *);
// compte le nombre de polynômes de degré arg1 vérifiant la propriété arg2
// optimisé quand la propriété est stable par passage au réciproque
// et fausse pour les polynômes de coeff. constant nul
// si arg3 != 0 ces polynômes sont sauvés sur arg3 au format binaire
// si arg4 != 0 ces polynômes sont sauvés sur arg4 au format texte

size_t
f2_poly_irred_count(size_t,FILE *);

size_t
f2_poly_primitive_count(size_t,FILE *);

#endif /* F2_POLY_H */
