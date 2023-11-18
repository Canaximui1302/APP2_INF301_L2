#ifndef INTERPRETE_H
#define INTERPRETE_H

typedef enum 
{
    GAUCHE,
    DROITE,
    AVANCER,
    MESURE,
    POSER,
    ADDITION,
    SOUSTRACTION,
    MULTIPLICATION,
    NOMBRE,
    IF,
    ECHANGE,
    EXECUTION,
    BOUCLE,
    CLONE,
    IGNORER,
    ROTATION,
    COMMANDE_INCORRECTE
} commande;

int interprete(sequence_t* seq, bool debug);

#endif
