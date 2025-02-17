#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void readGrammar(char * filename,grammar * g){
	newGrammar(filename,g);
	puts("Reading Grammar");
	FILE * fp= fopen(filename, "r");
	
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
				{newLHS(&g->rules[line],tk);i++;add(tk,line);}
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
void createParseTree(parseTree * pt, tokenStream * ts, grammar g){
	newStack();
	push("PROGRAM",0);
	pt->start=newTreeNode(0,"PROGRAM",0);
	tokenNode * lookup=ts->head;
	expandChild(pt->start,lookup,g);
}
int expandChild(treeNode * tn,tokenNode * lookup,grammar g){
	char * top=pop(0);
	printf("Searching: %s Level: %d Stack Top: %s\n",lookup->lexeme,tn->level,top);
	if(top){
		if(isNonTerminal(top)){
			mapNode * possibleRules = search(top);
			while(possibleRules){
				Node * rule=&(g.rules[possibleRules->value]);
				int i=0;
				if(strcmp(rule->name,top)==0){
					if(isValidVarId(lookup->lexeme) && strcmp(lookup->token,top)==0){
						addChild(tn,newTreeNode(tn->level+1,lookup->lexeme,0));
						return 1;
					}
					if(strcmp("ID",top)!=0){
							rule=rule->next;
						while(rule){
							push(rule->name,1);
							addChild(tn,newTreeNode(tn->level+1,rule->name,0));
							i++;
							rule=rule->next;
						}
						while(i>0){
							push(pop(1),0);
							i--;
						}
						if(expandSibling(tn->child,lookup,g)){
							return 1;
						}else{
							deleteChild(tn);
						}
					}
					
				}
				possibleRules=possibleRules->next;
			}
		}else{
			if(strcmp(top,lookup->lexeme)==0){
				return 1;
			}else{
				return 0;
			}
		}
	}else{
		puts("Stack Empty");
		return 1;
	}
}
int expandSibling(treeNode * tn,tokenNode * lookup,grammar g){
		int i=0;
		treeNode * t=tn;
		while(t){
			i=expandChild(t,lookup,g);
			if(i){
				t=t->sibling;
				lookup=lookup->next;
			}
			else{
				return 0;
			}
		}
		return i;
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
	if(strcmp(lexeme,"()")==0){return "BRACKETS";}
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
