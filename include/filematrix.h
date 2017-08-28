#ifndef _FILEMATRIX_H_
#define _FILEMATRIX_H_

#define ONE 1

/* Estrutura de Matrizes */
typedef struct matrix{
	double **matrix;
}MT;

/* Estrutura principal (Content) */
typedef struct content{
	char *ftrain,*fclass,*ftest;
	MT *matrices;
	int numOfmasks,maskDimension,k_Neighbors;
}CTNT;

void freeCTNT(CTNT**);

double **moveMatrix(double **, int, int);

void printMatrix(double**, int, int);

void readInput(CTNT **);

double **createMatrix(int, int);

void freeMatrix(double**,int);

#endif