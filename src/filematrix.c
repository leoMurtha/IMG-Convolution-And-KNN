#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <filematrix.h>
#include <cv.h>
#include <util.h>

/* Funcao que libera o input*/
void freeCTNT(CTNT **T){
	int i;
		for(i=0 ; i < (*T)->numOfmasks ; i++) freeMatrix((*T)->matrices[i].matrix,(*T)->maskDimension);
		free((*T)->matrices);
		free((*T)->ftrain);
		free((*T)->fclass);
		free((*T)->ftest);
		free((*T));
}

/* Desaloca uma regiao de memoria representada por uma matrix */
void freeMatrix(double **matrix,int row){
	int i;
		for(i=0;i<row;i++) free(matrix[i]);
		free(matrix);
}

/* Retorna uma região de memoria representando uma matriz */
double **createMatrix(int row,int col){
	int i;
	double **matrix;

		matrix=(double**)malloc(sizeof(double*)*row);

		for(i=0;i<row;i++) matrix[i]=(double*)malloc(sizeof(double)*col);

	return matrix;		
}

/* Move uma matriz de lugar */
double **moveMatrix(double **matrix,int row,int col){
	int i;
	double **newMatrix = createMatrix(row,col);
		
		for(i=0;i<row;i++) memcpy(newMatrix[i],matrix[i],col);
		freeMatrix(matrix,row);
	
	return newMatrix;				
}

void printMatrix(double **matrix,int row,int col){
	int i,j;

		for(i=0;i<row;i++){
			for(j=0;j<col;j++) printf("%.0lf ",matrix[i][j]);
			printf("\n");
		}
}

void readInput(CTNT **T4){
	char *files;
	int strlen,i,j,k;
	
	/* Inicializando principal estrutura */
	(*T4)=(CTNT*)calloc(ONE,sizeof(CTNT));
	/*-----------------------------------*/
	
	/* Nome do arquivo de treinamento */
		files=readline(&strlen);
		(*T4)->ftrain=(char*)malloc(sizeof(char)*strlen);
		strcpy((*T4)->ftrain,files);
		free(files);
	/*--------------------------------*/

	/* Nome do arquivo de classes  */
		files=readline(&strlen);
		(*T4)->fclass=(char*)malloc(sizeof(char)*strlen);
		strcpy((*T4)->fclass,files);
		free(files);
	/*-----------------------------*/

	/* Nome do arquivo de test */
		files=readline(&strlen);
		(*T4)->ftest=(char*)malloc(sizeof(char)*strlen);
		strcpy((*T4)->ftest,files);
		free(files);
	/*------------------------*/

	/* Número de mascaras */
		scanf("%d",&(*T4)->numOfmasks);
	/*--------------------*/

	/* Dimensão das mascaras */
		scanf("%d",&(*T4)->maskDimension);
	/*-----------------------*/

	/* Inicializar (Alocar) as matrizes */
		(*T4)->matrices = (MT*)malloc(sizeof(MT)*((*T4)->numOfmasks));
		for(k=0;k<(*T4)->numOfmasks;k++) (*T4)->matrices[k].matrix = createMatrix((*T4)->maskDimension,(*T4)->maskDimension);	
	/*--------------------------------------*/

	/* Preencher as matrizes */
		for(k=0;k<(*T4)->numOfmasks;k++){
			for(i=0;i<(*T4)->maskDimension;i++){
				for(j=0;j<(*T4)->maskDimension;j++) scanf("%lf",&(*T4)->matrices[k].matrix[i][j]);
			}
		}
	/*-----------------------*/

	/* Numero de k vizinhos */
		scanf("%d",&(*T4)->k_Neighbors);
	/*-----------------------*/
}