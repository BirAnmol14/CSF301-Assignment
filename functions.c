#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void readGrammar(char * filename,grammar * g){
	puts("Reading Grammar");
}
int isValidVarId(char * var){
	if(var==NULL){
		return 0;
	}	
	if(strlen(var)==0 || strlen(var)>20){
		return 0;
	}
	if(isdigit(var[0])){
		return 0;
	}
	int valid=1;
	for(int i=0;i<strlen(var);i++){
		if(isalpha(var[i])||isdigit(var[i])||var[i]=='_'){
			continue;
		}
		valid=0;break;
	}
	return valid;
}
