#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataStructures.h"
void newGrammar(grammar *g){
	for(int i=0;i<Arr_Size;i++){
		(g->rules[i]).name=malloc(MAX_LENGTH*sizeof(char));
		(g->rules[i]).next=NULL;
	}
}
void newLHS(Node *h,char * str){
	strcpy(h->name,str);
}
void newRHS(Node * head,char * str){
	Node * tmp=head;
	while(tmp->next!=NULL){
		tmp=tmp->next;
	}
	tmp->next=malloc(1*sizeof(Node));
	tmp=tmp->next;
	tmp->name=malloc(MAX_LENGTH*sizeof(char));
	tmp->next=NULL;
	strcpy(tmp->name,str);
}
tokenStream * newTokenStream(){
	tokenStream * tmp=malloc(1*sizeof(tokenStream));
	tmp->head=NULL;
	tmp->tail=NULL;
	return tmp;
}
tokenNode * newTokenNode(char * lexeme,char * token,int line){
	tokenNode * tmp=malloc(1*sizeof(tokenNode));
	tmp->lexeme=malloc((strlen(lexeme)+1)*sizeof(char));
	strcpy(tmp->lexeme,lexeme);
	tmp->token=malloc((strlen(token)+1)*sizeof(char));
	strcpy(tmp->token,token);
	tmp->line=line;
	tmp->next=NULL;
	return tmp;
}
tokenStream * addTokenNode(tokenStream * s,char * lexeme,char * token,int line){
	if(s->head==NULL){
		s->head=newTokenNode(lexeme,token,line);
		s->tail=s->head;
		return s;
	}
	tokenNode * tmp=s->tail;
	tmp->next=newTokenNode(lexeme,token,line);
	tmp=tmp->next;
	s->tail=tmp;
	return s;
}
void printTokenStream(tokenStream * ts){
	tokenNode * tmp=ts->head;
	while(tmp){
		printf("%s %s %d\n",tmp->lexeme,tmp->token,tmp->line);
		tmp=tmp->next;
	}
}
