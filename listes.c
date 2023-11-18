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

cellule_t *nouvelleCellule(void)
{
    /* À compléter (utiliser malloc) */
    // printf("\n>>>>>>>>>>> A Faire : liste.c/nouvelleCellule() <<<<<<<<<<<<<<<<\n");
    cellule_t *c = malloc(sizeof(cellule_t));
    c->type = 0; 
    c->command = 0;
    c->suivant = NULL;
    return c;
}

void detruireCellule(cellule_t *cel) //fonction pour liberer la memoire d'une cellule
{
    /* À compléter (utiliser free) */
    // printf("\n>>>>>>>>>>> A Faire : liste.c/detruireCellule() <<<<<<<<<<<<<<<<\n");
    if (cel->type == 1)
    {
        detruireSequence(cel->bloc);
    }
    free(cel);
}

void detruireSequence(sequence_t *seq) //fonction pour liberer la memoire d'une sequence
{
    if (seq == NULL || seq->tete == NULL)
        return;
    cellule_t *c = seq->tete;
    //afficher(seq);
    //printf("\n");
    while (c != NULL) // boucle while pour detruire chaque cellule dans la sequence
    {
        cellule_t *suiv = c->suivant;
        detruireCellule(c);
        c = suiv;
    }
}

void ajoute_apres_val(cellule_t *c, char val) // ajouter un entier a la fin de sequence
{
    cellule_t *c_nouv = nouvelleCellule();
    c_nouv->type = 0;
    c_nouv->command = val;
    c_nouv->suivant = c->suivant;
    c->suivant = c_nouv;
}

void ajoute_apres_bloc(cellule_t *c, sequence_t *bloc) // ajouter un bloc de code a la fin de sequence
{
    cellule_t *c_nouv = nouvelleCellule();
    c_nouv->type = 1;
    c_nouv->bloc = bloc;
    c_nouv->suivant = c->suivant;
    c->suivant = c_nouv;
}

void ajoute_debut_val(sequence_t *seq, char val) // ajouter un entier au debut de sequence
{

    cellule_t *c = nouvelleCellule();
    c->type = 0;
    c->command = val;
    c->suivant = seq->tete;
    seq->tete = c;
}

void ajoute_debut_bloc(sequence_t *seq, sequence_t *bloc) // ajouter un bloc de code au debut de sequence
{ 
    cellule_t *c = nouvelleCellule();
    c->type = 1;
    c->bloc = bloc;
    c->suivant = seq->tete;
    seq->tete = c;
}

cellule_t *conversion(char *texte, sequence_t *seq)
{
    /* À compléter */
    // printf("\n>>>>>>>>>>> A Faire : liste.c/conversion() <<<<<<<<<<<<<<<<\n");
    seq->tete = NULL;
    cellule_t *c = seq->tete;
    for (int i = 0; texte[i] != '\0'; i++)
    {
        sequence_t *bloc = NULL;
        cellule_t *queue = NULL;
        if (texte[i] == ' ' || texte[i] == '\n')
            continue;
        if (texte[i] == '}') // fin d'un bloc de code
        {
            if (c != NULL)
            {
                if (c->type == 1)
                {
                    c->command = 0; // on met 0 pour savoir la cellule n'est une commande, mais un bloc de code
                }
                c->type = i; // longueur du bloc
            }
            return c;
        }
        if (texte[i] == '{') // debut d'un bloc de code
        {
            bloc = malloc(sizeof(sequence_t));
            bloc->tete = NULL;
            queue = conversion((texte + i + 1), bloc); // appel recursive pour conversion le bloc imbrique
            if (!silent_mode)
                printf("\nElement #%d: %c\n", i, texte[i]); //////DEBUG
            if (queue != NULL)
            {
                i += queue->type + 1;
                if (!queue->command)
                {
                    queue->type = 1; // pour marquer que queue est un bloc
                }
                else
                {
                    queue->type = 0; // queue est une commande
                }
            }
            else
                i++;
        }
        if (c == NULL) // la queue de la sequence ou la sequence vide
        {
            if (texte[i] == '}') // si la cellule est un bloc de code
            {
                ajoute_debut_bloc(seq, bloc);
            }
            else
            {
                ajoute_debut_val(seq, texte[i]);
            }
            if (!silent_mode)
                printf("\nElement #%d: %c\n", i, texte[i]); //////DEBUG
            afficher(seq);
            c = seq->tete;
        }
        else
        {
            if (texte[i] == '}')
            {
                ajoute_apres_bloc(c, bloc);
            }
            else
            {
                ajoute_apres_val(c, texte[i]);
            }
            if (!silent_mode)
                printf("\nElement #%d: %c\n", i, texte[i]); //////DEBUG
            afficher(seq);
            c = c->suivant;
        }
    }
    return NULL;
}

cellule_t *enlever_premier_element(sequence_t *seq) 
{
    cellule_t *c = seq->tete;
    seq->tete = c->suivant;
    c->suivant = NULL;
    return c;
}

// void copier(sequence_t *seq, sequence_t *copie, cellule_t *queue)
// {
//     cellule_t *c = seq->tete;
//     copie->tete = NULL;
//     queue = copie->tete;
//     if(c != NULL)
//     {
//         if(c->type == 0)
//         {
//             ajoute_debut_val(copie, c->command);
//             printf("COPIE: \n");
//             afficher(copie);
//         }
//         else
//         {
//             sequence_t *s1 = malloc(sizeof(sequence_t));
//             cellule_t *q1 = malloc(sizeof(cellule_t));
//             afficher(c->bloc);
//             copier(c->bloc, s1, q1);
//             ajoute_debut_bloc(copie, s1, q1);
//         }
//         queue = copie->tete;
//         c = c->suivant;
//     }
//     while(c != NULL)
//     {
//         if(c->type == 0)
//         {
//             ajoute_apres_val(queue, c->command);
//             printf("COPIE: \n");
//             afficher(copie);
//         }
//         else
//         {
//             sequence_t *s1 = malloc(sizeof(sequence_t));
//             cellule_t *q1 = malloc(sizeof(cellule_t));
//             afficher(c->bloc);
//             copier(c->bloc, s1, q1);
//             ajoute_apres_bloc(queue, s1, q1);
//         }
//         queue = queue->suivant;
//         c = c->suivant;
//     }
// }

void afficher(sequence_t *seq)
{
    if (!silent_mode)
    {
        // assert (seq); /* Le pointeur doit être valide */
        /* À compléter */
        // printf("\n>>>>>>>>>>> A Faire : liste.c/afficher() <<<<<<<<<<<<<<<<\n");
        if (seq == NULL)
            return;
        cellule_t *c = seq->tete;
        while (c != NULL)
        {
            if (c->type == 0)
                printf("%c", c->command);
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
