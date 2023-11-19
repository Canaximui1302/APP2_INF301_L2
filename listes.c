#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;


cellule_t* nouvelleCellule (void)
{
    cellule_t *c = malloc(sizeof(cellule_t));
    c->type = 0;
    c->command = 0;
    c->suivant = NULL;
    return c;
}


void detruireCellule (cellule_t* cel)
{
    if(cel->type == 1)
    {
        detruireSequence(&(cel->bloc));
    }
    free(cel);
    cel = NULL;
}


void detruireSequence(sequence_t **seq)
{
    if(*seq == NULL) return;
    if((*seq)->tete == NULL)
    {
        free(*seq);
        return;
    }
    cellule_t *c = (*seq)->tete;
    free(*seq);
    while(c!=NULL)
    {
        cellule_t *suiv = c->suivant;
        detruireCellule(c);
        c = suiv;
    }
}


void detruire_sequence_nonrec(sequence_t *seq)
{
    if(seq == NULL) return;
    if(seq->tete == NULL)
    {
        free(seq);
        return;
    }
    cellule_t *c = seq->tete;
    while(c!=NULL)
    {
        cellule_t *suiv = c->suivant;
        free(c);
        c = suiv;
    }
}


void ajoute_apres_val(cellule_t *c, char val)
{
    cellule_t *c_nouv = nouvelleCellule();
    c_nouv->type = 0;
    c_nouv->command = val;
    c_nouv->suivant = c->suivant;
    c->suivant = c_nouv;
}


void ajoute_apres_bloc(cellule_t *c, sequence_t *bloc)
{
    cellule_t *c_nouv = nouvelleCellule();
    c_nouv->type = 1;
    c_nouv->bloc = bloc;
    c_nouv->suivant = c->suivant;
    c->suivant = c_nouv;
}


void ajoute_debut_val(sequence_t *seq, char val)
{
    
    cellule_t *c = nouvelleCellule();
    c->type = 0;
    c->command = val;
    c->suivant = seq->tete;
    seq->tete = c;
}


void ajoute_debut_bloc(sequence_t *seq, sequence_t *bloc)
{
    cellule_t *c = nouvelleCellule();
    c->type = 1;
    c->bloc = bloc;
    c->suivant = seq->tete;
    seq->tete = c;
}


int conversion (char *texte, sequence_t *seq)
{
    seq->tete = NULL;
	cellule_t* c = seq->tete;
    for(int i = 0; texte[i] != '\0'; i++)
    {
        sequence_t *bloc = NULL;
        int long_bloc;
        if (texte[i] == ' ' || texte[i] == '\n')
        {
            continue;
        }
        if(texte[i] == '}')
        {
            if(c != NULL)
            {
                return i;
            }
            return 0;
        }
        if(texte[i] == '{')
        {
            bloc = malloc(sizeof(sequence_t));
            bloc->tete = NULL;
            long_bloc = conversion((texte+i+1), bloc);
            i += long_bloc + 1;
        }
        if(c == NULL)
        {
            if(texte[i] == '}')
            {
                ajoute_debut_bloc(seq, bloc);
            }
            else
            {
                ajoute_debut_val(seq, texte[i]);
            }
            c = seq->tete;
        }
        else 
        {
            if(texte[i] == '}')
            {
                ajoute_apres_bloc(c, bloc);
            }
            else
            {
                ajoute_apres_val(c, texte[i]);
            }
            c = c->suivant;
        }
    }
    return 0;
}


void afficher (sequence_t* seq)
{
    if(!silent_mode)
    {
        assert (seq); /* Le pointeur doit être valide */
        if(seq == NULL) return;
        cellule_t *c = seq->tete;
        while(c != NULL)
        {
            if(c->type == 0) printf("%c", c->command);
            else
            {
                printf("{");
                afficher(c->bloc);
                printf("}");
            }
            c = c->suivant;
        }
    }
}

