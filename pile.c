#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pile.h"
#include "listes.h"


void empiler_val(pile *p, int val) // empiler un entier
{
    cellule_p *c = malloc(sizeof(cellule_p));
    c->type = 0;
    c->val = val;
    c->suivant = p->tete;
    p->tete = c;
}

void empiler_bloc(pile *p, sequence_t *seq) // empiler un bloc de code
{
    cellule_p *c = malloc(sizeof(cellule_p));
    c->type = 1;
    c->seq = seq;
    c->suivant = p->tete;
    p->tete = c;
    if(c->seq->tete != NULL)
    {
        printf("Empilage de ");
        afficher(c->seq);
        
    }
    else
    {
        printf("Empilage de bloc vide\n");
    }
}

int depiler_val(pile *p) // depiler un entier
{
    int val = p->tete->val;
    cellule_p *c = p->tete;
    p->tete = p->tete->suivant;
    free(c);
    return val;
}

void depiler_bloc(pile *p, sequence_t *seq) // depiler un bloc de code
{
    cellule_p *c = p->tete;
    seq->tete = p->tete->seq->tete;
    p->tete = p->tete->suivant;
    free(c);
}

void depiler(pile *p, int *val, sequence_t *seq) // empiler en utilisant les 2 fonctions ci-dessus
{
    cellule_p *c = p->tete;
    if(c->type == 0)
    {
        *val = depiler_val(p);
    }
    else
    {
        *val = -1;
        depiler_bloc(p, seq);
    }
}


//Affichage de la pile avec la tete de la pile au debut
void afficher_pile(pile *p)
{
    if(!silent_mode)
    {
        printf("Pile : [ ");
        cellule_p *c = p->tete;
        while(c != NULL)
        {
            if(c->type == 0)
            {
                printf("%d ", c->val);
            }
            else
            {
                printf("{ ");
                afficher(c->seq);
                printf(" } ");
            }
            c = c->suivant;
        }
        printf("]\n");
    }
}
