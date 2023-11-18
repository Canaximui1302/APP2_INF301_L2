#ifndef PILE_H
#define PILE_H
#include "listes.h"



struct cellule_pile {
    int type; // si type = 0, la cellule est un entier, sinon elle est un bloc de code
    int val;
    sequence_t *seq; // // la sequence contenant le bloc de code
    struct cellule_pile *suivant;
};
typedef struct cellule_pile cellule_p;

struct pile_p{
    cellule_p *tete;
};
typedef struct pile_p pile;


void empiler_val(pile *p, int val);

void empiler_bloc(pile *p, sequence_t *seq);

int depiler_val(pile *p);

void depiler_bloc(pile *p, sequence_t *seq);

void depiler(pile *p, int *val, sequence_t *seq);

void afficher_pile(pile *p);

#endif