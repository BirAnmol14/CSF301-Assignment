#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void readGrammar(char * filename,grammar * g){
	newGrammar(filename,g);
	puts("Reading Grammar");
	FILE * fp= fopen(filename, "r");
	FILE * fp1 = fopen("LHS.txt","w");
	if(fp){
		char * temp =(char *)malloc(sizeof(char)*500);
		int line=0;
		while(fscanf(fp,"%[^\n]\n",temp)!=EOF){
			char * temp1=(char *)malloc(sizeof(char)*500);
			strcpy(temp1,temp);
			int i=0;
			char * tk=strtok(temp1," \t");
			while(tk){
				if(i==0)
				{newLHS(&g->rules[line],tk);i++;add(tk,line);fprintf(fp1,"%s\n",tk);}
				else{
					if(strcmp(tk,"\\0")==0){newRHS(&g->rules[line],"");}
					else{newRHS(&g->rules[line],tk);}
					i++;
				}
				tk=strtok(NULL," \t");
			}
			free(temp1);
			line++;
		}
		free(temp);
		mapToLL();
		fclose(fp1);

		fclose(fp);
	}else{
		puts("No Such File Exists");
	}
}
void tokeniseSourcecode(char * filename,tokenStream * ts){
	FILE * fp=fopen(filename,"r");
	if(fp){
		char line [500];
		int line_count=0;
		int line_2 =0;
		while(fscanf(fp,"%[^\n]\n",line)!=EOF){
			line_count++;
			printf("%d\t%s\n",line_count,line);
			char line1[500];
			strcpy(line1,line);
			if(strlen(line1)!=0)
			line_2++;
			char * tk=strtok(line1," \t");
			while(tk){
				char* tmp = getToken(tk);
				if(!strcmp(tk,"declare")||!strcmp(tk,"R1"))
				decl_count++;
				ts=addTokenNode(ts,tk,tmp,line_count);
				tk=strtok(NULL," \t");
			}
		}
		assign_count= line_2 -3 -decl_count;
		fclose(fp);
	}else{
		puts("No such File Exists");
	}
}
void printTypeExpressionTable(typeExpressionTable* t){
	for(int i=0;i<typeSize;i++){
		Type * tp=t->arr[i];
		if(tp->field2==Primitive){
			printf("%s Primitive %s %s\n",tp->field1,tp->field3,printPrimType((tp->field4).primType));
		}
		else if(tp->field2==Rectangular){
			printf("%s Rectangular_Array %s %s\n",tp->field1,tp->field3,printRectArr((tp->field4).rectangular));
		}
		else if(tp->field2==Jagged){
			printf("%s Jagged_Array %s %s\n",tp->field1,tp->field3,printJagArr((tp->field4).jagged));
		}
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
	if(strcmp(lexeme,"declare")==0){return "terminal";}
	if(strcmp(lexeme,"program")==0){return "terminal";}
	if(strcmp(lexeme,"boolean")==0){return "terminal";}
	if(strcmp(lexeme,"integer")==0){return "terminal";}
	if(strcmp(lexeme,"real")==0){return "terminal";}
	if(strcmp(lexeme,"size")==0){return "terminal";}
	if(strcmp(lexeme,"list")==0){return "terminal";}
	if(strcmp(lexeme,"of")==0){return "terminal";}
	if(strcmp(lexeme,"variables")==0){return "terminal";}
	if(strcmp(lexeme,"array")==0){return "terminal";}
	if(strcmp(lexeme,"jagged")==0){return "terminal";}
	if(strcmp(lexeme,"values")==0){return "terminal";}
	if(strcmp(lexeme,"..")==0){return "terminal";}
	if(strcmp(lexeme,"R1")==0){return "terminal";}
	if(strcmp(lexeme,"()")==0){return "terminal";}
	if(strcmp(lexeme,")")==0){return "terminal";}
	if(strcmp(lexeme,"(")==0){return "terminal";}
	if(strcmp(lexeme,"]")==0){return "terminal";}
	if(strcmp(lexeme,"[")==0){return "terminal";}
	if(strcmp(lexeme,"}")==0){return "terminal";}
	if(strcmp(lexeme,"{")==0){return "terminal";}
	if(strcmp(lexeme,":")==0){return "terminal";}
	if(strcmp(lexeme,";")==0){return "terminal";}
	if(strcmp(lexeme,"=")==0){return "terminal";}
	if(strcmp(lexeme,"+")==0){return "OPERATOR";}
	if(strcmp(lexeme,"-")==0){return "OPERATOR";}
	if(strcmp(lexeme,"*")==0){return "OPERATOR";}
	if(strcmp(lexeme,"/")==0){return "OPERATOR";}
	if(strcmp(lexeme,"&&&")==0){return "BOOL_OPERATOR";}
	if(strcmp(lexeme,"|||")==0){return "BOOL_OPERATOR";}

	if(isValidVarId(lexeme)){return "ID";}
	int Num=1;
	for(int i=0;i<strlen(lexeme);i++){
		if(!isdigit(lexeme[i])){
			Num=0;
			break;
		}
	}
	if(Num){return "STAT_INT";}
	return "Unidentified Token";
}
