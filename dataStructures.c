#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataStructures.h"
int grammarSize=0;
int typeSize=0;
void newGrammar(char * filename,grammar *g){
	FILE * fp=fopen(filename,"r");
	if(fp){
		char * temp=(char *)malloc(sizeof(char)*512);
		int line=0;
		while(fscanf(fp,"%[^\n]\n",temp)!=EOF){
			line++;
		}
		free(temp);
		fclose(fp);
		grammarSize=line;
	}else{
		puts("No Such File Exists");
	}
	g->rules=(Node *)malloc(grammarSize*sizeof(Node));
	for(int i=0;i<grammarSize;i++){
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
void printGrammar(grammar * g){
	if(g==NULL){
		puts("Please Populate Grammar First");
	}
	puts("Printing Grammar");
	for(int i=0;i<grammarSize;i++){
		Node * tmp =&(g->rules[i]);
		printf("%d ",i);
		while(tmp){
			printf("%s ",tmp->name);
			tmp=tmp->next;
		}
		puts("");
	}
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
typeExpressionTable * newTable(){
	typeSize=0;
	typeExpressionTable * t=malloc(1*sizeof(typeExpressionTable));
	t->arr=NULL;
	return t;
}
Type * newType(char * varName,category cat,char * rectSub,typeExpression te){
	Type * t=malloc(1*sizeof(Type));
	t->field1=malloc((strlen(varName)+1)*sizeof(char));
	strcpy(t->field1,varName);
	t->field2=cat;
	if(rectSub==NULL){
		t->field3=malloc(50*sizeof(char));
		strcpy(t->field3,"not_applicable");
	}else{
		t->field3=malloc((strlen(rectSub)+1)*sizeof(char));
		strcpy(t->field3,rectSub);
	}
	t->field4=te;
	return t;
}
typeExpression newTypeExpression(char * s,category c){
	typeExpression *te=malloc(1*sizeof(typeExpression));
	if(c==Primitive){
		te->primitive=malloc((strlen(s)+1)*sizeof(char));
		strcpy(te->primitive,s);
	}else if(c==Rectangular){
		te->rectangular=malloc((strlen(s)+1)*sizeof(char));
		strcpy(te->rectangular,s);
	}else if(c==Jagged){
		te->jagged=malloc((strlen(s)+1)*sizeof(char));
		strcpy(te->jagged,s);
	}
	return *te;
}
void addType(typeExpressionTable * tab,Type * t){
	if(typeSize==0){
		tab->arr=malloc(1*sizeof(Type*));
		tab->arr[0]=t;
		typeSize++;
		return ;
	}
	tab->arr=realloc(tab->arr,(typeSize+1)*sizeof(Type*));
	tab->arr[typeSize]=t;
	typeSize++;
}
