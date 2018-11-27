#ifndef LFSR_H
#define LFSR_H
/* déclarations posix pour les types d'entiers */
#include <stdint.h>

/* type des "petits entiers", index ... */
typedef uint8_t byte_t;

/*
Le registre d'un lfsr est représenté par un entier de 8, 16, 32 ou 64
bits. Soit MAXLEN la taille de l'entier (en bits).

Le polynôme de connexion est représenté par un entier de même taille que
le registre. L'ordre des coefficients est tel qu'un simple xor donne le
résultat.

La longueur est la taille réelle du registre. C'est au plus MAXLEN.

L'entier zero est le numéro du bit de l'entier représentant le registre
à partir duquel le registre du lfsr est stocké. Les bits réellement
utilisés sont les bits du registre dont la position va de zero à zero+len-1.
*/
#define WITH_64BITS

#if defined(WITH_8BITS)
typedef uint8_t maxlen_t;
#define MAXINT 0xff
#elif defined(WITH_16BITS)
typedef uint16_t maxlen_t;
#define MAXINT 0xffff
#elif defined(WITH_32BITS)
typedef uint32_t maxlen_t;
#define MAXINT 0xffffffff
#elif defined(WITH_64BITS)
typedef uint64_t maxlen_t;
#define MAXINT 0xffffffffffffffffULL
#else
#error("Erreur : taille du registre non définie.")
#endif

/* MAXLEN : constante = longueur maximale, *en bits*, d'un registre. */
#define MAXLEN (sizeof(maxlen_t) << 3)

typedef struct {
  maxlen_t regist;
  maxlen_t poly;
}container_t;

typedef struct {
  byte_t zero; // position du premier bit du registre (numérotée à partir de 0
  byte_t len;  // longueur du registre
  container_t *cont;
}lfsr_t;

/* MASK(a,b) : cette macro calcule un entier machine (type maxlen_t)
comportant b bits consécutifs à 1 à partir du bit d'indice a, tous les
autres étant à 0 (comptés à partir du poids faible).  L'indice du
premier bit est 0. Par exemple en 8 bits, MASK(2,3) : 00011100

N'utiliser *que* des opérations bit à bit et décalages.
*/
#define MASK(a,b) ( (~(MAXINT << b))<<(a))

#define LFSR_MASK(lfsr) MASK(lfsr->zero, lfsr->len)

/* Initialisation du conteneur, ne dépend pas de la taille du lfsr, un
   même conteneur peut contenir les registres de plusieurs lfsr. */
int lfsr_container_init (container_t *, const maxlen_t, maxlen_t);

/* La fonction lfsr__init(lfsr, zero, len, cont)
initialise le lfsr, et vérifie que les données sont cohérentes ;
zero <= MAXLEN
len <= MAXLEN
zero+len-1<=MAXLEN
 */
int lfsr_init (lfsr_t *, const byte_t, const byte_t, container_t *);

/* Incrémentation du LFSR, ne modifie *que* les bits affectés au LFSR */
void lfsr_inc (lfsr_t *);

/* fonction de calcul de la plus petit période d'un lfsr engendré par une suite */
int lfsr_period(lfsr_t *);

/*fonction auxilère qui permet de vérifier l'appartenance à un tableau */
int is_here(maxlen_t , maxlen_t*, int);
#endif /* LFSR_H */
