#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void readGrammar(char * filename,grammar * g){
	puts("Reading Grammar");
	FILE * fp= fopen(filename, "r");
	
	char * temp=(char*)malloc(sizeof(char)*MAX_LENGTH);
	char delim;
	
	fscanf(fp,"%[^ \n]s",temp);
	newLHS(&(g->arr[0]),temp);
	
	//printf("%s", temp);	(uncomment for debugging)
	
	for(int i=0; i<Arr_Size;){
		delim=fgetc(fp);
		if(delim==EOF){
			break;
		}
		else if(delim=='\n'){
			i++;
			fscanf(fp,"%[^ \n]s",temp);
			newLHS(&(g->arr[i]),temp);
		}
		else if(delim==' '){
			fscanf(fp,"%[^ \n]s",temp);
			newRHS(&(g->arr[i]),temp);
		}
			
		//printf("%c",delim);	(uncomment for debugging)
		//printf("%s",temp);	(uncomment for debugging)
	}
}
void tokeniseSourcecode(char * filename,tokenStream * ts){
	FILE * fp=fopen(filename,"r");
	if(fp){
		puts("Here");
		char line [500];
		int line_count=0;
		while(fscanf(fp,"%[^\n]\n",line)!=EOF){
			line_count++;
			printf("%d\t%s\n",line_count,line);
			char line1[500];
			strcpy(line1,line);
			char * tk=strtok(line1," \t");
			while(tk){
				ts=addTokenNode(ts,tk,getToken(tk),line_count);
				tk=strtok(NULL," \t");
			}
		}
		fclose(fp);
	}else{
		puts("No such File Exists");
	}
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
char * getToken(char * lexeme){
	if(strcmp(lexeme,"declare")==0){return "DECLARE_KWD";}
	if(strcmp(lexeme,"program")==0){return "PROGRAM_KWD";}
	if(strcmp(lexeme,"boolean")==0){return "TYPE_KWD";}
	if(strcmp(lexeme,"integer")==0){return "TYPE_KWD";}
	if(strcmp(lexeme,"real")==0){return "TYPE_KWD";}
	if(strcmp(lexeme,"size")==0){return "SIZE_KWD";}
	if(strcmp(lexeme,"list")==0){return "LIST_KWD";}
	if(strcmp(lexeme,"of")==0){return "OF_KWD";}
	if(strcmp(lexeme,"variables")==0){return "VARIABLES_KWD";}
	if(strcmp(lexeme,"array")==0){return "ARRAY_KWD";}
	if(strcmp(lexeme,"jagged")==0){return "JAGGED_KWD";}
	if(strcmp(lexeme,"values")==0){return "VALUES_KWD";}
	if(strcmp(lexeme,"..")==0){return "DOTDOUBLE";}
	if(strcmp(lexeme,"R1")==0){return "ROW_INIT";}
	if(strcmp(lexeme,")")==0){return "CLS_RND";}
	if(strcmp(lexeme,"(")==0){return "OPEN_RND";}
	if(strcmp(lexeme,"]")==0){return "CLS_SQ";}
	if(strcmp(lexeme,"[")==0){return "OPEN_SQ";}
	if(strcmp(lexeme,"}")==0){return "CLS_CURL";}
	if(strcmp(lexeme,"{")==0){return "OPEN_CURL";}
	if(strcmp(lexeme,",")==0){return "COMMA";}
	if(strcmp(lexeme,":")==0){return "COLON";}
	if(strcmp(lexeme,";")==0){return "SEMI_COLON";}
	if(strcmp(lexeme,"+")==0){return "OPERATOR";}
	if(strcmp(lexeme,"-")==0){return "OPERATOR";}
	if(strcmp(lexeme,"*")==0){return "OPERATOR";}
	if(strcmp(lexeme,"/")==0){return "DIV_OPERATOR";}
	if(strcmp(lexeme,"&&&")==0){return "BOOL_OPERATOR";}
	if(strcmp(lexeme,"|||")==0){return "BOOL_OPERATOR";}
	if(strcmp(lexeme,"=")==0){return "EQUALS";}
	if(isValidVarId(lexeme)){return "ID";}
	int Num=1;
	for(int i=0;i<strlen(lexeme);i++){
		if(!isdigit(lexeme[i])){
			Num=0;
			break;	
		}
	}
	if(Num){return "STATIC_INT";}
	return "Unidentified Token";
}
