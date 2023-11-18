#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de compiler avec la macro 'SILENT' définie, en 
 * faisant 'make silent'
 */

#ifdef SILENT

/* Desactive tous les affichages */
#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)
/* Desactive les 'Appuyer sur entrée pour continuer' */
#define getchar() ('\n')

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;


struct cellule {
    int type; // si type = 0, la cellule est une commande executable, sinon elle est un bloc de code
    char command; 
    struct sequence *bloc; // la sequence contenant le bloc de code
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;

cellule_t* nouvelleCellule (void);

void detruireCellule (cellule_t*);

void detruireSequence(sequence_t *seq);

void ajoute_debut_val(sequence_t *seq, char val);

void ajoute_debut_bloc(sequence_t *seq, sequence_t *bloc);

void ajoute_apres_val(cellule_t *c, char val);

void ajoute_apres_bloc(cellule_t *c, sequence_t *bloc);

cellule_t* conversion (char *texte, sequence_t *seq);

cellule_t* enlever_premier_element(sequence_t *seq);

//void copier(sequence_t *seq, sequence_t *copie);

void afficher(sequence_t* seq);


#endif
