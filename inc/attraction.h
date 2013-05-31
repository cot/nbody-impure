/**
 * \author François Rué
 * \version 0.1
 * \date 11 avril 2013
 * \file attraction.h
 * \brief fichier d'entête pour les prototypes de fonctions physiques
 */

struct phibase Phi(int indice, int k, struct position *r);
struct contribution Somme(int indice,struct phibase **phi);
/*
double SommePhiX(int indice, int level);
double SommePhiY(int indice, int level);
double SommePhiZ(int indice, int level);
double EvalVitesseX(int indice);
double EvalVitesseY(int indice);
double EvalVitesseZ(int indice);
double SommeX(int indice, int iteration);
double SommeY(int indice, int iteration);
double SommeZ(int indice, int iteration);
double EvolutionX(int indice);
double EvolutionY(int indice);
double EvolutionZ(int indice);
*/
