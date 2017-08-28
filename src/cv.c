#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <math.h>
#include <filematrix.h>
#include <limits.h>

/* Funcao que libera os feature vectors */
void freeFV(PGM *img,int size){
	int i;
		for(i=0; i < size; i++) free(img[i].fv);
}

/* Funcao que cria um feature vector por imagem */
double *getFV(PGM img,int *size,int numOfmasks){
	int i,j,k,t,gzero,lzero,ezero;
	double average,var,entropy,*fv= (double*)calloc(img.row*FEATURES*numOfmasks,sizeof(double));
			
		for(t=0,k=0;t < numOfmasks ; t++){
			for(i=0;i < img.row ; i++){
				for(j=0,average=0,gzero=0,lzero=0,ezero=0,var=0,entropy=0;j < img.col ; j++){
					/* Analisando as condicoes do PDF */
					if(img.image.mts[t].c_matrix[i][j] > 0) gzero++;
					if(img.image.mts[t].c_matrix[i][j] == 0) ezero++;
					if(img.image.mts[t].c_matrix[i][j] < 0) lzero++;
					average += img.image.mts[t].c_matrix[i][j]; 
					/*--------------------------------*/
				}
				
				/* Calculados gzero e lzero e ezero faremos as atribuicoes */
				fv[k++] = gzero;
				fv[k++] = ezero;
				fv[k++] = lzero;
				/*---------------------------------------------------------*/
				
				/* Calculada a media iremos calcular a variancia */
				average /= (img.col*1.0);
				fv[k++] = average;
				/*-----------------------------------------------*/

				/* Calcula a variancia e a entropia e depois as atribui ao feature vector */
				for(j=0;j < img.col ; j++){
					var += pow(img.image.mts[t].c_matrix[i][j] - average,2);
					entropy += img.image.mts[t].c_matrix[i][j] * ((log(fabs(img.image.mts[t].c_matrix[i][j])+1))/log(2));
				}

				var /= (img.col*1.0);
				entropy *= -1;

				fv[k++] = var;
				fv[k++] = entropy;	

				/*-----------------------------------------------------------------------*/
			}
		}

		(*size) = k;	
		
	return fv;	
}

/* Funcao que retorna apenas o valor central de uma convolucao */
int overlap(double **matrix,int rowBound,int colBound , int row, int col,double **mask, int maskDimension){
	int i,j,mid = maskDimension/2;
	int sum;

		for(i=0,sum=0;i < maskDimension;i++){
			for(j=0;j < maskDimension;j++){
				/* Condicao das bordas para que evitemos acesso indevido a memoria */
				if((row-mid+i) >= 0 && (col-mid+j) >= 0 && (row-mid+i) < rowBound && (col-mid+j) < colBound){
					sum += matrix[(row-mid+i)][col-mid+j] * mask[i][j]; 
				}
			}
		}

	return sum;	
}

/* Funcao que chama monta a nova matriz convoluida chamando uma funcao auxiliar que retorna a convulucao de um ponto so */
double **pre_overlap(double **matrix,int row,int col,double **mask,int maskDimension){
	int i,j;	
	double **mr = createMatrix(row,col);

		/* Preenchendo a matriz resposta */
		for(i=0;i<row;i++){
			for(j=0;j<col;j++) mr[i][j] = overlap(matrix,row,col,i,j,mask,maskDimension);
		}

	return mr;	
}

/* Faz a convolucao em apenas uma imagem com todas as mascaras */
void convolution(PGM *img, CTNT *T){
	int i;

		(*img).image.mts = (CONV*)malloc(sizeof(CONV)*T->numOfmasks);

		/* Laco de convolucoes */
		for(i=0;i < T->numOfmasks ;i++){
			/* Sempre operaremos sobre a matriz original */
			(*img).image.mts[i].c_matrix = pre_overlap((*img).image.matrix,(*img).row,(*img).col,T->matrices[i].matrix,T->maskDimension);
		}
}

/* Retorna o vetor de classes obitdos do arquivo */
double *readClass(char *filename,int *n){
	double *classes=NULL;
	FILE *fp=fopen(filename,"r+");
	(*n) = 0;

	/* Enquanto diferente do final do arquivo vai reallocando ponteiro de double */
		while(!feof(fp)){
			if(!feof(fp)){
				classes = (double*)realloc(classes,sizeof(double)*((*n)+1));
				fscanf(fp,"%lf",&classes[(*n)]);
				(*n)++;
			}
		}

		fclose(fp);

	return classes;
}

/* Funcao que le as imagens nos arquivos de test e train */
PGM *readIMGs(char *filename,int *n){
	PGM *images=NULL;
	char aux[40],c;
	FILE *fp=fopen(filename,"r+");
	(*n) = 0;

	/* Enquanto diferente do final do arquivo vai reallocando ponteiro de PGMS */
		while(!feof(fp)){
			fscanf(fp,"%[^\r\n]%c",aux,&c);		
			if(!feof(fp) && c != 13 && c!=EOF){
				images = (PGM*)realloc(images,sizeof(PGM)*((*n)+1));
				images[(*n)] = readPGM(aux);
				(*n)++;
			}
		}

		fclose(fp);

	return images;	
}

/* Destroi uma imagem PGM */
void destroyPGM(PGM *destroy,int numOfmasks){
	int i;
	freeMatrix((*destroy).image.matrix,(*destroy).row);
	
	for(i=0; i < numOfmasks ; i++) freeMatrix((*destroy).image.mts[i].c_matrix,(*destroy).row);
	free((*destroy).image.mts);	
}

/* Le uma imagem PGM a partir de um arquivo */
PGM readPGM(char *filename){
	PGM new;
	FILE *fp=fopen(filename,"r+");
	int i,j;
	unsigned char c=2;

		while(!feof(fp)){
			if(!feof(fp) && c!=13){
				/* Ignorando o Magic Number */
				fscanf(fp,"%*[^\n]");
				
				/* Adquirindo numero de colunas e linhas */
				fscanf(fp,"%d%*c%d",&new.col,&new.row);
				
				/* Valor maximo da imagem */
				fscanf(fp,"%lf",&new.v_max);
				
				/* Criando a matriz da imagem */
				new.image.matrix = createMatrix(new.row,new.col);

				/* Preechendo a matriz */
				for(i=0;i<new.row;i++){
					for(j=0;j<new.col;j++){
						fscanf(fp,"%c",&c);
						new.image.matrix[i][j] = (double)c;
					}
				}
			}
			break;
		}

		fclose(fp);	

	return new;
}