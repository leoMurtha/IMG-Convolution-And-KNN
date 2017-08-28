#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opmath.h>

/* Distancia euclidiana entre vetores que retorna um double */
double euclidian_double_distance(double *v1,double *v2,int s1,int s2){
	int i;
	double distance=0;

		for(i=0,distance=0 ; i < s1 ; i++) distance += pow((v1[i] - v2[i]),2);
	return sqrt(distance);		
}