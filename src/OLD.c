#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "../inc/proto.h"
#include "../inc/attraction.h"

typedef char * caddr_t;

#ifdef PAPI
#include <papi.h>
#define NUMEVENTS 2
#endif

/**
 *\author François Rué
 *\version 0.1
 *\date 11 avril 2013
 *\file nbody.c
 *\brief Probleme a n-corps multidimensionnel pour le cas d'effet gravitationnel
 */

/*! \fn int main()
 *  \brief Programme principal initialisant les champs (position, masse)
 *  \param void
 *  \return Les points d'evolution
 */


int main() {

	int i,j,k;

	struct position* __attribute__ ((aligned (32))) r;
        struct position* __attribute__ ((aligned (32))) rzero;
	struct phibase** __attribute__ ((aligned (32))) phi;
	struct vitesse*  __attribute__ ((aligned (32))) v;

/* Structure de la Matrice */
	double m_val[2*npart] __attribute__ ((aligned (32))) ;
	double m_i[2*npart] __attribute__ ((aligned (32))) ;
	double m_j[npart] __attribute__ ((aligned (32))) ;

	struct position u;
	double res;
	double vit;
	
        struct timeval start, end;
#ifdef PAPI
	int retval,event_set=PAPI_NULL;
	long_long   values[NUMEVENTS];
	int events[NUMEVENTS] = {PAPI_FP_INS,PAPI_L2_DCM};

	/* Initialize PAPI Library */
	if(PAPI_library_init(PAPI_VER_CURRENT)!=PAPI_VER_CURRENT) exit(1);

	/* Create the eventset */
	if(PAPI_create_eventset(&event_set)!=PAPI_OK) exit(1);

	PAPI_add_events(event_set, events, 2);

#endif

	r 	= (struct position *)calloc(npart,sizeof(struct position));
        rzero   = (struct position *)calloc(npart,sizeof(struct position));
	v 	= (struct vitesse *)calloc(npart,sizeof(struct vitesse));

	gettimeofday(&start, NULL);
        for (i=0;i<npart;i++) {
                r[i].M = 1.89  ;                          
                for (j=0;j<dim;j++) {
                        r[i].X = (10.2 * i);
                        rzero[i].X = (10.2 * i);
                        r[i].Y = (10.2 * i);
                        rzero[i].Y = (10.2 * i);
                        r[i].Z = (10.2 * i);
                        rzero[i].Z = (10.2 * i);
		}
	}
	printf("_________________________________________________________________________\n\n");
	vit = (double) 0.0;
	for(k=0;k<niter;k++) {
		for(i=0;i<npart;i++) {
			for(j=0;j<npart;j++) {

#ifdef PAPI
				if(PAPI_start_counters(events,NUMEVENTS)!=PAPI_OK) exit(1);
#endif
				u.X = r[i].X - r[j].X ;
				u.Y = r[i].Y - r[j].Y ;
				u.Z = r[i].Z - r[j].Z ;

				res = (double) 1.0 / sqrt(u.X * u.X + u.Y * u.Y + u.Z * u.Z);
/* Remplissage de la matrice creuse CSR => remplissage de (i,j,val)*/
				m_val[2*j]   = res;
				m_val[2*j+1] = - res;
				m_i[2*j]     = j;
				m_i[2*j+1]   = i;
				m_j[j]       = 2*j;

				vit = vit + r[i].M * r[j].M * res;

				FX = r[i].M * r[j].M * res * res * res * u.X;
				FY = r[i].M * r[j].M * res * res * res * u.Y;
				FZ = r[i].M * r[j].M * res * res * res * u.Z;
#ifdef PAPI
				/* Write the number of cache misses for the Loop */
				if(PAPI_read_counters(values,NUMEVENTS)!=PAPI_OK) exit(1);

				printf("\tPAPI_FP_INS =%lli\n ",values[0]);
				printf("\tPAPI_L2_DCM =%lli\n ",values[1]);
				/* Stop counting */
				if(PAPI_stop_counters(values,NUMEVENTS) != PAPI_OK) exit(1);
#endif
			}
		}
	}

/*
	for(i=0;i<npart;i++) {
		printf("r[%i].X = %g a t=0 \n",i,r[i].X);
		printf("r[%i].Y = %g a t=0 \n",i,r[i].Y);
		printf("r[%i].Z = %g a t=0 \n",i,r[i].Z);
		printf("________________________\n");
	}
*/

#ifdef PAPI
	/* reset the PAPI counters */
	if(PAPI_reset(event_set)!=PAPI_OK) exit(1);
#endif
        gettimeofday(&end, NULL);
	printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
	printf("everything's good \n");
	printf("_________________________________________________________________________\n\n");
        free(r);
        free(rzero);
        free(v);
        free(phi);

	return EXIT_SUCCESS;

}
