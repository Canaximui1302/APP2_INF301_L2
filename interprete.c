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

void stop(void)
{
    char enter = '\0';
    printf("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
}

//La fonction transform facilite l'utilisation de switch dans l'interprete
commande transform(char c)
{
    switch (c)
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

/*la fonction prend en argument une sequence de routine, une pile pour contenir les elements a executer,
et une sequence a_detruire pour contenir tous les elements que l'on doit liberer a la fin */
int interprete_seq(sequence_t *seq, bool debug, pile *p, sequence_t *a_detruire)
{
    char cmd;
    int x, y, n;
    cellule_t *c = seq->tete;
    afficher(seq);
    while (c != NULL)
    {
        sequence_t *s1, *s2;
        if (c->type == 0)
        {
            commande instruction;
            cmd = c->command;
            instruction = transform(cmd);
            int ret; //Pour recuperer le resultat de l'interpretation de test
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
                    return VICTOIRE; /* on a atteint la cible */
            if (ret == RATE)
                return RATE; /* tombé dans l'eau ou sur un rocher */
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
            empiler_val(p, x + y);
            break;
            case SOUSTRACTION:
            y = depiler_val(p);
            x = depiler_val(p);
            empiler_val(p, x - y);
            break;
            case MULTIPLICATION:
            y = depiler_val(p);
            x = depiler_val(p);
            empiler_val(p, x *y);
            break;
            case NOMBRE:
            empiler_val(p, cmd - '0');
            break;
            case IF:
            s1 = malloc(sizeof(sequence_t));
            s2 = malloc(sizeof(sequence_t));
            depiler_bloc(p, s1); // Le bloc F

            depiler_bloc(p, s2); // Le bloc V

            x = depiler_val(p); // La valeur n pour savoir quel bloc va etre excute
            if (!silent_mode)
                afficher_pile(p);

        // sequence_t *seq_a_detruire;
        if (x == 0) // execute bloc F
        {
            // exec s1
            // detruireSequence(&s2);
            ajoute_debut_bloc(a_detruire, s2);

        // if(q1 != NULL)
        // {
        //     q1->suivant = seq->tete;
        //     seq->tete = s1->tete;
        // }
        y = interprete_seq(s1, debug, p, a_detruire);
        // free(s2);
        switch (y)
        {
        case VICTOIRE:
            return VICTOIRE;
        case RATE:
            return RATE;
        }
                }
                else //sinon execute bloc V 
                {
                    // exec s2

                    // if(q2 != NULL)
                    // {
                    //     q2->suivant = seq->tete;
                    //     seq->tete = s2->tete;
                    // }
                    // printf("HELLLOOUU @@@\n");
                    ajoute_debut_bloc(a_detruire, s1);
                    switch (interprete_seq(s2, debug, p, a_detruire))
                    {
                    case VICTOIRE:
                        return VICTOIRE;
                    case RATE:
                        return RATE;
                    }
                }
                // if(efface_seq)
                // {
                //     cellule_t *c = seq_a_detruire->tete;
                //     free(seq_a_detruire);
                //     while(c != NULL)
                //     {
                //         cellule_t *suiv = c->suivant;
                //         detruireCellule(c);
                //         c = suiv;
                //     }
                // }
                break;
            case ECHANGE:
                s1 = malloc(sizeof(sequence_t));
                s2 = malloc(sizeof(sequence_t));
                depiler(p, &x, s1);
                depiler(p, &y, s2);
                printf("HELLO\n");
                if (x == -1) //si -1 on empile un bloc de code 
                {
                    afficher(s1);
                    printf(": s1\n");
                    empiler_bloc(p, s1);
                }
                else // sinon on empile une valeur(int)
                {
                    free(s1);
                    empiler_val(p, x);
                }
                if (y == -1) //de meme pour y
                {
                    afficher(s2);
                    printf(": s2\n");
                    empiler_bloc(p, s2);
                }
                else
                {
                    free(s2);
                    empiler_val(p, y);
                }
                break;
            case EXECUTION:
                s1 = malloc(sizeof(sequence_t));
                depiler_bloc(p, s1);
                // if(q1 != NULL)
                //     {
                //         q1->suivant = seq->tete;
                //         seq->tete = s1->tete;
                //     }
                ajoute_debut_bloc(a_detruire, s1);
                switch (interprete_seq(s1, debug, p, a_detruire))
                {
                case VICTOIRE:
                    return VICTOIRE;
                case RATE:
                    return RATE;
                }
                break;
            case BOUCLE:
                s1 = malloc(sizeof(sequence_t));
                x = p->tete->val;
                s1->tete = p->tete->suivant->seq->tete;
                while (x != 0)
                {
                    switch (interprete_seq(s1, debug, p, a_detruire))
                    {
                    case VICTOIRE:
                        return VICTOIRE;
                    case RATE:
                        return RATE;
                    }
                    p->tete->val = x - 1; //decremente n
                }
                if (x == 0) //si n = 0 on enleve n et cmd et B
                {
                    depiler_val(p);
                    depiler_bloc(p, s1);
                    ajoute_debut_bloc(a_detruire, s1);
                }
                break;
            case CLONE:
                if (p->tete->type == 0)
                {
                    empiler_val(p, p->tete->val);
                }
                else
                {
                    // s1 = malloc(sizeof(sequence_t));
                    // copier(p->tete->seq, s1, q1);
                    empiler_bloc(p, p->tete->seq);
                }
                break;
            case IGNORER:
                s1 = malloc(sizeof(sequence_t));
                depiler(p, &x, s1);
                if (x == -1)
                {
                    ajoute_debut_bloc(a_detruire, s1);
                }
                break;
            case ROTATION:
                x = depiler_val(p); // 1. nombre de pas
                n = depiler_val(p); // 2. nombre d'elements en haut de la pile
                if (n == x || x == 0)
                    break;
                cellule_p *a1 = p->tete; //element en haut de la pile
                cellule_p *anx; // n-x ieme element
                cellule_p *an = p->tete, *an1; // n-ieme element et l'element apres
                for (int i = 1; i <= n; i++)
                {
                    if (i == n - x)
                    {
                        anx = an; // on echange le pointeur de n-x ieme element et n-ieme element
                    }
                    if (i != n)
                        an = an->suivant; // on parcourit jusqu'au n-ieme element
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
            // if(efface_seq)
            // {
            //     detruireCellule(c);
            //     c = seq->tete;
            // }
            // else
            c = c->suivant;
        }
        else
        {
            empiler_bloc(p, c->bloc);
            // free(c);
            c = c->suivant;
        }
        if (!silent_mode)
        {
            afficher_pile(p);
            afficherCarte();
            printf("Programme:");
            afficher(seq);
            printf("\n");
            if (debug)
                stop();
        }
    }
    return CIBLERATEE;
}

int interprete(sequence_t *seq, bool debug)
{
    pile *p = malloc(sizeof(pile));
    sequence_t *a_detruire = malloc(sizeof(sequence_t));
    a_detruire->tete = NULL;
    int x;
    p->tete = NULL;
    x = interprete_seq(seq, debug, p, a_detruire);
    printf("On detruit seq\n");
    detruireSequence(seq);
    printf("On detruit a_detruire\n");
    // detruireSequence(a_detruire);
    free(p);
    return x;
}
