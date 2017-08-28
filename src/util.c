#include <stdlib.h>
#include <stdio.h>
#include <util.h>

/* Funcao que concatena duas strings */
char *stringUnion(char *str1,char *str2){
	char *strUnion;
	int i,len1=strlen(str1),len2=strlen(str2);

		strUnion=(char*)malloc(sizeof(char)*(len1+len2+2));
		strcpy(strUnion,str1);
		
		for(i=0;i<len2;i++) strUnion[i+len1]=str2[i];

		strUnion[i+len1]='\0';

	return strUnion;
}

void int_ins_sort(int *vector,int size){
	int i,j,element;

		for(i=1;i<size;i++){
			element=vector[i];
			j=i-1;
			
			while(j>=0 && element > vector[j]){
				vector[j+1] = vector[j];
				j--;
			}
			vector[j+1]=element;
		}
}

char *readline(int *len){
	char *string=NULL;
	int i=0;

		do{
			string=(char*)realloc(string,sizeof(char)*(i+1));
			string[i++]=fgetc(stdin);
			if(string[i-1]==13) scanf("%*c");
		}while(string[i-1]!=ENTER && string[i-1]!=13);
			string[i-1]='\0';	
			if(len!=NULL) (*len)=i;	

	return string;
}