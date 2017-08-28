#ifndef _CV_H_
#define _CV_H_

#include <filematrix.h>

#define FEATURES 6

typedef struct conv{
	double **c_matrix;
}CONV;

typedef struct mat{
	double **matrix;
	CONV *mts;
}IMG;

typedef struct image{
	IMG image;
	int row,col,fv_size;
	double v_max,*fv;
}PGM;

double *getFV(PGM,int*,int);

void freeFV(PGM*,int);

PGM *readIMGs(char *,int *);

double *readClass(char *,int *);

int overlap(double **,int, int, int, int, double**, int);

double **pre_overlap(double **,int ,int ,double **,int );

void convolution(PGM*, CTNT*);

PGM readPGM(char *);

void destroyPGM(PGM *,int);

#endif