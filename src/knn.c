#include <stdlib.h>
#include <stdio.h>
#include <filematrix.h>
#include <string.h>
#include <cv.h>
#include <knn.h>
#include <limits.h>
#include <opmath.h>

/* Insertion sort O(n*n) */
void sort(KNN *vector,int size){
	int i,j;
	KNN element;

		for(i=1;i<size;i++){
			element.distance = vector[i].distance;
			element.t_class = vector[i].t_class;
			element.index = vector[i].index;
			element.frequency = vector[i].frequency;
			j=i-1;
			
			while(j>=0 && element.distance < vector[j].distance){
				vector[j+1].distance = vector[j].distance;
				vector[j+1].t_class = vector[j].t_class;
				vector[j+1].index = vector[j].index;
				vector[j+1].frequency = vector[j].frequency;				
				j--;
			}
			vector[j+1].distance = element.distance;
			vector[j+1].t_class = element.t_class;
			vector[j+1].index = element.index;
			vector[j+1].frequency = element.frequency;
		}
}

/* Pega os K vizinhos */
KNN *index_kneighbors(double *ftest,int size, PGM *train, int K_neighbors, int ntrain,double *classes){
	KNN *K=(KNN*)malloc(sizeof(KNN)*K_neighbors),*aux=(KNN*)malloc(sizeof(KNN)*ntrain);
	int i;

		/* Calculando as distancias */
		for(i=0; i < ntrain ; i++){
			aux[i].distance = euclidian_double_distance(ftest,train[i].fv,size,train[i].fv_size);
			aux[i].index = i;
			aux[i].t_class = classes[i];
			aux[i].frequency = 0;
		}

		/* Ordena */
		sort(aux,ntrain);
		
		/* Pegando os K vizinhos com a menor distancia */
		for(i=0; i < K_neighbors ; i++) memcpy(&K[i],&aux[i],sizeof(KNN));
		free(aux);
	
	return K;	
}	

/* Funcao que retorna a classe da imagem test */
double findClass(KNN *K,int n){
	int i,j,index,greatest = INT_MIN;

		for(i=0 ; i < n; i++){
				K[i].frequency=0;
			for(j=0 ; j < n ; j++){
				if(K[i].t_class == K[j].t_class) K[i].frequency++;
			}
			if(K[i].frequency > greatest){
				greatest = K[i].frequency;
				index = i;
			}
		}
		
	return K[index].t_class;	
}

/* KNN -> K Nearest Neighbors */
void kneighbors(PGM *test,int ntest,PGM *train,int ntrain,int K_neighbors,double *classes){
	int i;
	KNN *K;

		for(i=0;i < ntest ; i++){
			K = index_kneighbors(test[i].fv,test[i].fv_size,train,K_neighbors,ntrain,classes);
			printf("%.3lf\n",findClass(K,K_neighbors));
			free(K);
		}
}