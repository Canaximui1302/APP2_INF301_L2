#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pile.h"
#include "listes.h"

void detruire_pile_nonrec(pile **p)
{
    if(*p == NULL) return;
    if((*p)->tete == NULL)
    {
        free(*p);
        return;
    }
    cellule_p *c = (*p)->tete;
    free(*p);
    while(c!=NULL)
    {
        cellule_p *suiv = c->suivant;
        free(c);
        c = suiv;
    }
}

void empiler_val(pile *p, int val)
{
    cellule_p *c = malloc(sizeof(cellule_p));
    c->type = 0;
    c->val = val;
    c->suivant = p->tete;
    p->tete = c;
}

void empiler_bloc(pile *p, sequence_t *seq)
{
    cellule_p *c = malloc(sizeof(cellule_p));
    c->type = 1;
    c->seq = seq;
    c->suivant = p->tete;
    p->tete = c;
}

int depiler_val(pile *p)
{
    int val = p->tete->val;
    cellule_p *c = p->tete;
    p->tete = p->tete->suivant;
    free(c);
    return val;
}

void depiler_bloc(pile *p, sequence_t **seq)
{
    cellule_p *c = p->tete;
    *seq = p->tete->seq;
    p->tete = p->tete->suivant;
    free(c);
}

void depiler(pile *p, int *val, sequence_t **seq)
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
