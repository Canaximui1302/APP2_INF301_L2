#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"
#include "pile.h"
#include "interprete.h"
#include <string.h>

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

commande transform(char c)
{
    switch(c)
    {
        case 'G':
        return GAUCHE;
        case 'D':
        return DROITE;
        case 'A':
        return AVANCER;
        case 'M':
        return MESURE;
        case 'P':
        return POSER;
        case '+':
        return ADDITION;
        case '-':
        return SOUSTRACTION;
        case '*':
        return MULTIPLICATION;
        case '?':
        return IF;
        case 'X':
        return ECHANGE;
        case '!':
        return EXECUTION;
        case 'B':
        return BOUCLE;
        case 'C':
        return CLONE;
        case 'I':
        return IGNORER;
        case 'R':
        return ROTATION;
        default:
        if (c >= '0' && c <= '9')
        return NOMBRE;
        return COMMANDE_INCORRECTE;
    }
}

int interprete_seq (sequence_t* seq, bool debug, pile *p, sequence_t *a_detruire, int prog_principal)
{
    char cmd;
    int x, y, n;
    cellule_t *c = seq->tete;
    while (c!=NULL)
    {
        sequence_t **s1 = malloc(sizeof(sequence_t*)), **s2 = malloc(sizeof(sequence_t*));
        if(c->type == 0)
        {
            commande instruction;
            cmd = c->command;
            instruction = transform(cmd);
            int ret;
            switch (instruction) 
            {
                case GAUCHE:
                    gauche();
                    break;
                case DROITE:
                    droite();
                    break;
                case AVANCER:
                    ret = avance();
                    if (ret == VICTOIRE)
                    {
                        free(s1);
                        free(s2);
                        return VICTOIRE; /* on a atteint la cible */
                    }
                    if (ret == RATE)
                    {
                        free(s1);
                        free(s2);
                        return RATE; /* tombé dans l'eau ou sur un rocher */
                    }
                    break;
                case MESURE:
                    x = depiler_val(p);
                    y = mesure(x);
                    empiler_val(p, y);
                    break;
                case POSER:
                    x = depiler_val(p);
                    pose(x);
                    break;
                case ADDITION:
                    y = depiler_val(p);
                    x = depiler_val(p);
                    empiler_val(p, x+y);
                    break;
                case SOUSTRACTION:
                    y = depiler_val(p);
                    x = depiler_val(p);
                    empiler_val(p, x-y);
                    break;
                case MULTIPLICATION:
                    y = depiler_val(p);
                    x = depiler_val(p);
                    empiler_val(p, x*y);
                    break;
                case NOMBRE:
                    empiler_val(p, cmd-'0');
                    break;
                case IF:
                    depiler_bloc(p, s1);
                    depiler_bloc(p, s2);
                    x = depiler_val(p);
                    
                    if(x == 0)
                    {
                        y = interprete_seq(*s1, debug, p, a_detruire, 0);
                        switch(y)
                        {
                            case VICTOIRE:
                                free(s1);
                                free(s2);
                                return VICTOIRE;
                            case RATE:
                                free(s1);
                                free(s2);
                                return RATE;
                        }
                    }
                    else
                    {
                        y = interprete_seq(*s2, debug, p, a_detruire, 0);
                        switch(y)
                        {
                            case VICTOIRE:
                                free(s1);
                                free(s2);
                                return VICTOIRE;
                            case RATE:
                                free(s1);
                                free(s2);
                                return RATE;
                        }
                    }
                    break;
                case ECHANGE:
                    depiler(p, &x, s1);
                    depiler(p, &y, s2);
                    if(x == -1)
                    {
                        empiler_bloc(p, *s1);
                    }
                    else
                    {
                        empiler_val(p, x);
                    }
                    if(y == -1)
                    {
                        empiler_bloc(p, *s2);
                    }
                    else
                    {
                        empiler_val(p, y);
                    }
                    break;
                case EXECUTION:
                    depiler_bloc(p, s1);
                    y = interprete_seq(*s1, debug, p, a_detruire, 0);
                    switch(y)
                    {
                        case VICTOIRE:
                            free(s1);
                            free(s2);
                            return VICTOIRE;
                        case RATE:
                            free(s1);
                            free(s2);
                            return RATE;
                    }
                    break;
                case BOUCLE:
                    x = p->tete->val;
                    *s1 = p->tete->suivant->seq;
                    while(x != 0)
                    {
                        y = interprete_seq(*s1, debug, p, a_detruire, 0);
                        switch(y)
                        {
                            case VICTOIRE:
                                free(s1);
                                free(s2);
                                return VICTOIRE;
                            case RATE:
                                free(s1);
                                free(s2);
                                return RATE;
                        }
                        p->tete->val = --x;
                    }
                    if(x == 0)
                    {
                        depiler_val(p);
                        depiler_bloc(p, s1);
                    }
                    break;
                case CLONE:
                    if(p->tete->type == 0)
                    {
                        empiler_val(p, p->tete->val);
                    }
                    else
                    {
                        empiler_bloc(p, p->tete->seq);
                    }
                    break;
                case IGNORER:
                    depiler(p, &x, s1);
                    break;
                case ROTATION:
                    x = depiler_val(p);
                    n = depiler_val(p);
                    if(n == x || x == 0) break;
                    cellule_p *a1 = p->tete;
                    cellule_p *anx;
                    cellule_p *an = p->tete, *an1;
                    for(int i = 1; i <= n; i++)
                    {
                        if(i == n - x)
                        {
                            anx = an;
                        }
                        if(i != n)
                        an = an->suivant;
                    }
                    p->tete = anx->suivant; // a(n-x+1)
                    an1 = an->suivant;
                    an->suivant = a1;
                    anx->suivant = an1;
                    afficher_pile(p);
                    break;
                case COMMANDE_INCORRECTE:
                    printf("Erreur: commande '%c' introuvable.", cmd);
                    break;
                default:
                    printf("Fonctionnalite non declaree");
                    break;
            }

            c = c->suivant;
        }
        else
        {
            empiler_bloc(p, c->bloc);
            if(prog_principal)
            {
                ajoute_debut_bloc(a_detruire, c->bloc);
            }
                
            c = c->suivant;
        }
        if(!silent_mode)
        {
            afficher_pile(p);
            afficherCarte();
            printf ("Programme:");
            afficher(seq);
            printf ("\n");
            if (debug) stop();
        }
        free(s1);
        free(s2);
    }
    return CIBLERATEE;
}


int interprete (sequence_t* seq, bool debug)
{
    pile *p = malloc(sizeof(pile));
    sequence_t *a_detruire = malloc(sizeof(sequence_t));
    a_detruire->tete = NULL;
    int x;
    p->tete = NULL;
    x = interprete_seq(seq, debug, p, a_detruire, 1);
    detruire_sequence_nonrec(seq);
    detruireSequence(&a_detruire);
    detruire_pile_nonrec(&p);
    return x;
}
