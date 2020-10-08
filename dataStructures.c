#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataStructures.h"
void newGrammar(grammar *g){
	for(int i=0;i<Arr_Size;i++){
		(g->arr[i]).name=malloc(MAX_LENGTH*sizeof(char));
		(g->arr[i]).next=NULL;
	}
}
void newTerminal(Node *h,char * str){
	strcpy(h->name,str);
}
void newNonTerminal(Node * head,char * str){
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
