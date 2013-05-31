/**
 *\author François Rué
 *\version 0.1
 *\date 15 avril 2013
 *\file attraction.c
 *\brief Probleme a n-corps multidimensionnel pour le cas d'effet gravitationnel
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../inc/proto.h"
#include "../inc/attraction.h"

/*! \fn struct phibase Phi(int indice, int k, struct position *r)
 *  \brief Permet d'evaluer chacune des influences des autres astres sur l'astre évalué: PhiX évalue l'influence dans la direction x.
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "k" l'indice de l'astre dont on évalue l'influence
 *  \param[in] "*r" la structure décrivant la position de l'astre observé
 *  \param[out] "phibase" la structure décrivant la force d'attraction exercée
 *  \return phibase
 */
struct phibase Phi(int indice, int k, struct position *r) {
	struct phibase tmp;
	double norm;
	if(k!=indice) {
		norm = Norme(r[indice],r[k]);
		tmp.X = r[k].M * UnCube(norm) * Distance(r[indice].X,r[k].X );
		tmp.Y = r[k].M * UnCube(norm) * Distance(r[indice].Y,r[k].Y );
		tmp.Z = r[k].M * UnCube(norm) * Distance(r[indice].Z,r[k].Z );
	}
	else {
		tmp.X = 0.0;
		tmp.Y = 0.0;
		tmp.Z = 0.0;
	}
return tmp;
}

/*! \fn double Somme(int indice, struct phibase **phi)
 *  \brief Permet de sommer les influences de chacun des astres agissant sur l'astre étudié: SommePhi évalue l'influence globale 
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "**phi" la structure décrivant la force d'attraction exercée
 *  \return double
 */
struct contribution Somme(int indice,struct phibase **phi) {
	int i;
	struct contribution res;
	res.X = 0.0;
	res.Y = 0.0;
	res.Z = 0.0;
	for(i=0;i<npart;i++){
		res.X = res.X + phi[indice][i].X;
		res.Y = res.Y + phi[indice][i].Y;
		res.Z = res.Z + phi[indice][i].Z;
	}
	res.X = (double) (-1.0 * G * res.X);
	res.Y = (double) (-1.0 * G * res.Y);
	res.Z = (double) (-1.0 * G * res.Z);
	return res;
}

/*! \fn double SommePhiY(int indice, int level)
 *  \brief Permet de sommer les influences de chacun des astres agissant sur l'astre étudié: SommePhiY évalue l'influence globale dans la direction y.
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "level" le niveau d'évaluation où l'on se trouve; c'est le paramètre recursif
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double SommePhiY(int indice, int level) {
	double res;
	switch(level) {
		case 0:  return 0;
		default: res = PhiY(indice,level-1) + SommePhiY(indice,level-1);
	}
	return res;
}
*/
/*! \fn double SommePhiZ(int indice, int level)
 *  \brief Permet de sommer les influences de chacun des astres agissant sur l'astre étudié: SommePhiZ évalue l'influence globale dans la direction z.
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "level" le niveau d'évaluation où l'on se trouve; c'est le paramètre recursif
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double SommePhiZ(int indice, int level) {
	double res;
	switch(level) {
		case 0:  return 0;
		default: res = PhiZ(indice,level-1) + SommePhiZ(indice,level-1);
	}
	return res;
}
*/
/*! \fn double EvalVitesseX(int indice)
 *  \brief Permet d'évaluer la vitesse au regard de l'accéleration (somme des influences de chaque astre sur l'astre observé)
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double EvalVitesseX(int indice) {
	return(alpha * SommePhiX(indice, npart));
}
*/
/*! \fn double EvalVitesseY(int indice)
 *  \brief Permet d'évaluer la vitesse au regard de l'accéleration (somme des influences de chaque astre sur l'astre observé)
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double EvalVitesseY(int indice) {
	return(alpha * SommePhiY(indice, npart));
}
*/
/*! \fn double EvalVitesseZ(int indice)
 *  \brief Permet d'évaluer la vitesse au regard de l'accéleration (somme des influences de chaque astre sur l'astre observé)
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double EvalVitesseZ(int indice) {
	return(alpha * SommePhiZ(indice, npart));
}
*/
/*! \fn double SommeX(int indice, int iteration)
 *  \brief Permet d'évaluer la vitesse au pas de temps d'iteration désiré dans la direction X
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "iteration" l'indice de l'iteration terminale
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double SommeX(int indice, int iteration) {
	switch(iteration)
	{
		case niter:  return 0;
		default :    return(EvalVitesseX(indice) + SommeX(indice,iteration+1));
	}
}
*/
/*! \fn double SommeY(int indice, int iteration)
 *  \brief Permet d'évaluer la vitesse au pas de temps d'iteration désiré dans la direction Y
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "iteration" l'indice de l'iteration terminale
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double SommeY(int indice, int iteration) {
	switch(iteration)
	{
		case niter:  return 0;
		default :    return(EvalVitesseY(indice) + SommeY(indice,iteration+1));
	}
}
*/
/*! \fn double SommeZ(int indice, int iteration)
 *  \brief Permet d'évaluer la vitesse au pas de temps d'iteration désiré dans la direction Z
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \param[in] "iteration" l'indice de l'iteration terminale
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double SommeZ(int indice, int iteration) {
	switch(iteration)
	{
		case niter:  return 0;
		default :    return(EvalVitesseZ(indice) + SommeZ(indice,iteration+1));
	}
}
*/
/*! \fn double EvolutionX(int indice)
 *  \brief Permet d'évaluer le deplacement au pas de temps d'iteration désiré dans la direction X
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double EvolutionX(int indice) {
	return( SommeX(indice,0) );
}
*/
/*! \fn double EvolutionY(int indice)
 *  \brief Permet d'évaluer le deplacement au pas de temps d'iteration désiré dans la direction Y
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double EvolutionY(int indice) {
	return( SommeY(indice,0) );
}
*/
/*! \fn double EvolutionZ(int indice)
 *  \brief Permet d'évaluer le deplacement au pas de temps d'iteration désiré dans la direction Z
 *  \param[in] "indice" l'indice de l'astre qu'on observe
 *  \return toutes les fonctions de proto.c sont créées pour n'effectuer que des retours de type double
 */
/*
double EvolutionZ(int indice) {
	return( SommeZ(indice,0) );
}
*/


