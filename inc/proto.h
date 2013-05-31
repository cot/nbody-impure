/**
 * \author François Rué
 * \version 0.1
 * \date 11 avril 2013
 * \file proto.h
 * \brief fichier d'entête pour les prototypes de fonctions contenant les variables globales utiles au problème et quelques operateurs
 * \param npart nombre de particules
 * \param niter nombre d'iterations
 * \param deltat pas de temps du probleme (pour l'evaluation de la vitesse puis du deplacement)
 * \param dim dimension spatiale du probleme
 */
#include <unistd.h>
#include "mpi.h"

#include <emmintrin.h>

#define npart 100000
#define niter 1
#define deltat 0.001 

#define dim 3 

typedef double adble __attribute__ ((__aligned__(8)));

struct position{
        double X;
        double Y;
        double Z;
        double M;
};

struct force{
        double X;
        double Y;
        double Z;
};

struct vitesse{
        double X;
        double Y;
        double Z;
};


