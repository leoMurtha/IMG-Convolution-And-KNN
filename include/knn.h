#ifndef _KNN_H_
#define _KNN_H_

typedef struct kneighbors{
	int index,frequency;
	double t_class,distance;
}KNN;

void sort(KNN*,int);

double findClass(KNN*,int);

KNN *index_kneighbors(double *ftest,int size, PGM *train, int K_neighbors, int ntrain,double *classes);

void kneighbors(PGM *,int,PGM *,int,int ,double *);

#endif