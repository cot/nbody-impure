#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <emmintrin.h>

#include "../inc/proto.h"

typedef char * caddr_t;


/**
 *\author François Rué
 *\version 0.1
 *\date 31 mai 2013
 *\file nbody.c
 *\brief Probleme a n-corps multidimensionnel 
 */

/*! \fn int main()
 *  \brief Programme principal contenant les 3 implementations : basique, matriciel et sse
 *  \param void
 *  \return Les temps d'excecution et les evaluations des champs
 */


int main(int argc,char *argv[]) {


	int i,j,k,l,m;

/* ------------------------------------------------------------------------ */
/* Structure en mode basique */

        struct position* __attribute__ ((aligned (32))) r;

        struct position u;
        struct force F;
        double res0;
        double vit;

	r  = (struct position *)calloc(npart,sizeof(struct position));


/* ------------------------------------------------------------------------ */
/* Structure en mode matrix */
	/* double vitX, vitY, vitZ; */
	double uX, uY, uZ;
	double res, tmp1X, tmp1Y, tmp1Z;
	int tmp2;

	double  rX[npart];
	double  rY[npart];
	double  rZ[npart];
	double  rM[npart];

	/* Structure de la Matrice */
        double m_val[2*npart-1] __attribute__ ((aligned (32))) ;
        int m_i[2*npart] __attribute__ ((aligned (8))) ;
        int m_j[npart] __attribute__ ((aligned (8))) ;

/* ------------------------------------------------------------------------ */
/* Structure en mode sse */
	adble  rXsse[npart];
	adble  rYsse[npart];
	adble  rZsse[npart];
	adble  rMsse[npart];

	__m128d r1d, r2d, r3d, r4d, r5d, r6d, temp, und, rMd;
	__m128d FXd, FYd, FZd;
	__m128d VXd, VYd, VZd;

	double storedX[2];
	double storedY[2];
	double storedZ[2];

	und = _mm_setr_pd(1.0,1.0);
	double FX, FY, FZ;
	
/* ------------------------------------------------------------------------ */
/* Structure des compteurs temps */
        struct timeval debbase, endbase;
        struct timeval debmat, endmat;
        struct timeval debsse, endsse;

/* ------------------------------------------------------------------------ */
/* Initialisation des structures de donnees */
        for (i=0;i<npart;i++) {
		r[i].M    = 1.89 * ( i + 1 ) ;
		rM[i]     = 1.89 * ( i + 1 ) ;
		rMsse[i]  = 1.89 * ( i + 1 ) ;
	}
        for (i=0;i<npart;i++) {
		r[i].X    = 10.2 * ( i + 1 ) ;
		rX[i]     = 10.2 * ( i + 1 ) ;
		rXsse[i]  = 10.2 * ( i + 1 ) ;
	}
        for (i=0;i<npart;i++) {
		r[i].Y    = 10.3 * ( i + 1 ) ;
		rY[i]     = 10.3 * ( i + 1 ) ;
		rYsse[i]  = 10.3 * ( i + 1 ) ;
	}
        for (i=0;i<npart;i++) {
		r[i].Z    = 10.4 * ( i + 1 ) ;
		rZ[i]     = 10.4 * ( i + 1 ) ;
		rZsse[i]  = 10.4 * ( i + 1 ) ;
	}

	printf("_________________________________________________________________________\n\n");

/* ------------------------------------------------------------------------ */
/* Implementation du probleme en mode basique */
	gettimeofday(&debbase, NULL);
        for(k=0;k<niter;k++) {
                for(i=0;i<npart;i++) {
                        for(j=0;j<npart;j++) {
                                u.X = r[i].X - r[j].X ;
                                u.Y = r[i].Y - r[j].Y ;
                                u.Z = r[i].Z - r[j].Z ;

                                res0 = (double) 1.0 / sqrt(u.X * u.X + u.Y * u.Y + u.Z * u.Z);
                                vit = vit + r[i].M * r[j].M * res0;

                                F.X = res0 * res0 * res0 * u.X;
                                F.Y = res0 * res0 * res0 * u.Y;
                                F.Z = res0 * res0 * res0 * u.Z;
                        }
			r[i].X = r[i].X + deltat * F.X / r[i].M ; 
			r[i].Y = r[i].Y + deltat * F.Y / r[i].M ;
			r[i].Z = r[i].Z + deltat * F.Z / r[i].M ;
                }
        }

	gettimeofday(&endbase, NULL);
	printf("duree en mode basique = \t");
	printf(" %ld\n", ((endbase.tv_sec * 1000000 + endbase.tv_usec) - (debbase.tv_sec * 1000000 + debbase.tv_usec)));
	printf("rX[%i] = %g \n",npart-1,rX[npart-1]);
	printf("rY[%i] = %g \n",1,rY[1]);
	printf("_________________________________________________________________________\n\n");
/* Fin de l'implementation du probleme en mode basique */
/* ------------------------------------------------------------------------ */
/* Implementation du probleme en mode matriciel */
	gettimeofday(&debmat, NULL);
	for(k=0;k<niter;k++) {
		for(i=0;i<npart;i++) {
		FX = 0.0;
		FY = 0.0;
		FZ = 0.0;
			for(j=0;j<npart;j++) {
				if(j!=i) {

					uX  = rX[i] - rX[j] ; 
					uY  = rY[i] - rY[j] ; 
					uZ  = rZ[i] - rZ[j] ; 

					res = sqrt (uX*uX + uY*uY + uZ*uZ) ;
					res = (double) 1.0 / res ;
/* Remplissage de la matrice creuse CSR => remplissage de (i,j,val)*/
					m_val[2*j]   = res;
					m_val[2*j+1] = -1.0 * res;
					m_i[2*j]     = j;
					m_i[2*j+1]   = i;
					m_j[j]       = 2*j;
				}
				else {
                                        m_val[2*j]   = 0.0 ;
                                        m_val[2*j+1] = 0.0 ;
                                        m_i[2*j]     = j;
                                        m_i[2*j+1]   = i;
                                        m_j[j]       = 2*j;
				}
/* Fin de remplissage de la matrice creuse CSR */
			}
			for(l=0;l<npart;l++) {
				tmp1X = 0.0;
				tmp1Y = 0.0;
				tmp1Z = 0.0;
				for(m=m_j[l];m<m_j[l+1];m++) {
				tmp2 = m_i[l];
				tmp1X = tmp1X + m_val[l] * rX[tmp2];
				tmp1Y = tmp1Y + m_val[l] * rY[tmp2];
				tmp1Z = tmp1Z + m_val[l] * rZ[tmp2];
				}
			}
		rX[i] = rX[i] + deltat * FX / rM[i];
		rY[i] = rY[i] + deltat * FY / rM[i];
		rZ[i] = rZ[i] + deltat * FZ / rM[i];
		}
	}
	gettimeofday(&endmat, NULL);
	printf("duree en mode matrix = \t");
	printf(" %ld\n", ((endmat.tv_sec * 1000000 + endmat.tv_usec) - (debmat.tv_sec * 1000000 + debmat.tv_usec)));
	printf("rX[%i] = %g \n",npart-1,rX[npart-1]);
	printf("rY[%i] = %g \n",1,rY[1]);
	printf("_________________________________________________________________________\n\n");
/* Fin de l'implementation du probleme en mode matrix */
/* ------------------------------------------------------------------------ */
/* Implementation du probleme en mode sse */
	gettimeofday(&debsse, NULL);
	for(k=0;k<niter;k++) {
		for(i=0;i<npart;i++) {
		FXd = _mm_setzero_pd();
		FYd = _mm_setzero_pd();
		FZd = _mm_setzero_pd();
			for(j=0;j<npart;j+=2) {
				if(j!=i && (j+1)!=i) {
					r1d = _mm_setr_pd(rXsse[i],rXsse[i]);
					r2d = _mm_setr_pd(rXsse[j],rXsse[j+1]);
					r3d = _mm_setr_pd(rYsse[i],rYsse[i]);
					r4d = _mm_setr_pd(rYsse[j],rYsse[j+1]);
					r5d = _mm_setr_pd(rZsse[i],rZsse[i]);
					r6d = _mm_setr_pd(rZsse[j],rZsse[j+1]);
					temp = _mm_add_pd( _mm_mul_pd( _mm_sub_pd(r1d,r2d), _mm_sub_pd(r1d,r2d) ) , _mm_mul_pd( _mm_sub_pd(r3d,r4d), _mm_sub_pd(r3d,r4d) ));
					temp = _mm_add_pd( temp , _mm_mul_pd( _mm_sub_pd(r5d,r6d), _mm_sub_pd(r5d,r6d) ));
					temp = _mm_div_pd( und,  _mm_sqrt_pd( temp) );
					temp = _mm_mul_pd( _mm_mul_pd (temp,temp), temp);

					FXd  = _mm_add_pd( FXd, _mm_mul_pd( temp, _mm_sub_pd(r1d, r2d)));
					FYd  = _mm_add_pd( FYd, _mm_mul_pd( temp, _mm_sub_pd(r3d, r4d)));
					FZd  = _mm_add_pd( FZd, _mm_mul_pd( temp, _mm_sub_pd(r5d, r6d)));
				}
			}

		_mm_store_pd(&storedX[0],FXd);
		_mm_store_pd(&storedY[0],FYd);
		_mm_store_pd(&storedZ[0],FZd);

		FX = storedX[0] + storedX[1];
		FY = storedY[0] + storedY[1];
		FZ = storedZ[0] + storedZ[1];

		rXsse[i] = rXsse[i] + deltat * FX / rM[i];
		rYsse[i] = rYsse[i] + deltat * FY / rM[i];
		rZsse[i] = rZsse[i] + deltat * FZ / rM[i];
		}
	}

	gettimeofday(&endsse, NULL);
	printf("duree en mode sse = \t");
	printf("%ld\n", ((endsse.tv_sec * 1000000 + endsse.tv_usec) - (debsse.tv_sec * 1000000 + debsse.tv_usec)));
	printf("rXsse[%i] = %g \n",npart-1,rXsse[npart-1]);
	printf("rYsse[%i] = %g \n",1,rYsse[1]);
	printf("_________________________________________________________________________\n\n");
/* Fin de l'implementation du probleme en mode sse */
/* ------------------------------------------------------------------------ */

	return EXIT_SUCCESS;

}
