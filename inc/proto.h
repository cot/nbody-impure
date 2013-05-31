/**
 * \author François Rué
 * \version 0.1
 * \date 31 mai 2013
 * \file proto.h
 * \brief fichier d'entête pour les structures de données
 * \param npart nombre de particules
 * \param niter nombre d'iterations
 * \param deltat pas de temps du probleme (pour l'evaluation de la vitesse puis du deplacement)
 * \param dim dimension spatiale du probleme
 */
#include <unistd.h>
#include <emmintrin.h>

#define npart 50000
#define niter 1
#define deltat 0.001 
#define dim 3 

typedef double adble __attribute__ ((__aligned__(8)));

/** @struct position
 *  @brief Cette structure definit les particules par leurs quatres caracteristiques
 *  @var foreignstruct::X
 *  Le membre 'X' contient la position X
 *  @var foreignstruct::Y
 *  Le membre 'Y' contient la position Y
 *  @var foreignstruct::Z
 *  Le membre 'Z' contient la position Z
 *  @var foreignstruct::M
 *  Le membre 'M' contient la masse M
 */
struct position{
        double X;
        double Y;
        double Z;
        double M;
};

/** @struct force
 *  @brief Cette structure definit les forces
 *  @var foreignstruct::X
 *  Le membre 'X' contient la composante suivant X
 *  @var foreignstruct::Y
 *  Le membre 'Y' contient la composante suivant Y
 *  @var foreignstruct::Z
 *  Le membre 'Z' contient la composante suivant Z
 */
struct force{
        double X;
        double Y;
        double Z;
};

/** @struct vitesse
 *  @brief Cette structure definit les vitesse
 *  @var foreignstruct::X
 *  Le membre 'X' contient la composante suivant X
 *  @var foreignstruct::Y
 *  Le membre 'Y' contient la composante suivant Y
 *  @var foreignstruct::Z
 *  Le membre 'Z' contient la composante suivant Z
 */
struct vitesse{
        double X;
        double Y;
        double Z;
};


