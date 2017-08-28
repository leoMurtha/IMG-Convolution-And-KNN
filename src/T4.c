#include <stdlib.h>
#include <stdio.h>
#include <filematrix.h>
#include <cv.h>
#include <knn.h>
#include <opmath.h>

void T4(){
	CTNT *T;
	PGM *train;
	PGM *test;
	double *classes;
	int ntrain,ntest,nclasses,i;

		/* Lendo a entrada */
		readInput(&T);
		
		/* Lendo as imagens do arquivo de treinamento */
		train = readIMGs(T->ftrain,&ntrain);
		
		/* Lendo as imagens do arquivo de test */
		test = readIMGs(T->ftest,&ntest);
		
		/* Lendo as classes do aruqivo de classes */
		classes = readClass(T->fclass,&nclasses);

		/* Convolucao e depois criacao do feature vector das imagens de treinamento */
		for(i=0;i < ntrain;i++){
			convolution(&train[i],T);
			train[i].fv = getFV(train[i],&train[i].fv_size,T->numOfmasks);
			/* Liberar todas as imagens pois ja temos o feature vector */
			destroyPGM(&train[i],T->numOfmasks);
		}

		/* Convolucao e depois criacao do feature vector das imagens de teste */
		for(i=0;i<ntest;i++){	
			convolution(&test[i],T);
			test[i].fv = getFV(test[i],&test[i].fv_size,T->numOfmasks);
			/* Liberar todas as imagens pois ja temos o feature vector */
			destroyPGM(&test[i],T->numOfmasks);
		}

		/* Rodando o algoritimo de K vizinhos */
		kneighbors(test,ntest,train,ntrain,T->k_Neighbors,classes);

		/* Liberando os feature vectors */
		freeFV(train,ntrain);
		freeFV(test,ntest);

		/* Liberando as classes */
		free(classes);

		/* Liberando os ponteiros de PGM */
		free(train);
		free(test);

		/* Liberando o input */
		freeCTNT(&T);
}

int main(int argc,char *argv[]){
	T4();
	return 0;
}