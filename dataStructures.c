#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataStructures.h"
int grammarSize=0;
int typeSize=0;
mapNode ** map=NULL;
allNonTerminals * nt=NULL;
Stack * stack0=NULL;
Stack * stack1=NULL;
int mapSize=0;
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
	newHashMap(5*grammarSize);
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
		printf("%d ",i+1);
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
typeExpression newRectTypeExpression(RectArr * rarr){
	typeExpression *te=malloc(1*sizeof(typeExpression));
	te->rectangular=rarr;
	return *te;
}
typeExpression newPrimTypeExpression(primitive p){
	typeExpression *te=malloc(1*sizeof(typeExpression));
	te->primType=p;
	return *te;
}
typeExpression newJagTypeExpression(JagArr * jarr){
	typeExpression *te=malloc(1*sizeof(typeExpression));
	te->jagged=jarr;
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
RectArr * newRectArr(int dim){
		RectArr * tmp=malloc(1*sizeof(RectArr));
		tmp->type=malloc(sizeof(char) * (strlen("rectangularArray")+1));
		strcpy(tmp->type,"rectangularArray");
		tmp->dimensions=malloc(20*sizeof(char));
		itoa(dim,tmp->dimensions);
		tmp->basic="integer";
		tmp->range=malloc(dim*sizeof(arrRange));//Manually populate ranges after this
		return tmp;
}
void populateRectArr(RectArr * rarr,int index,char *lo,char *hi){
	(rarr->range[index]).low=lo;
	(rarr->range[index]).high=hi;
}
char * printRectArr(RectArr * rarr){
	char * string=malloc(1024*sizeof(char));
	char * buff = malloc(20 * sizeof(char));
	strcpy(string,"<type=");
	strcat(string,rarr->type);
	strcat(string,", dimensions=");
	strcat(string,rarr->dimensions);
	int dim=atoi(rarr->dimensions);
	for(int i=0;i<dim;i++){
		itoa(i+1,buff);
		strcat(string,", range_R");
		strcat(string,buff);
		strcat(string,"=(");
		strcat(string,rarr->range[i].low);
		strcat(string,",");
		strcat(string,rarr->range[i].high);
		strcat(string,")");
	}
	strcat(string,", basicElementType=");
	strcat(string,rarr->basic);
	strcat(string,">");
	free(buff);
	return string;
}
char * printPrimType(primitive p){
	char * string=malloc(1024*sizeof(char));
	strcpy(string,"<type=");
	if(p==Int){
		strcat(string,"integer");
	}else if(p==Real){
		strcat(string,"real");
	}
	else if(p==Bool){
		strcat(string,"boolean");
	}	
	strcat(string,">");
	return string;
}
JagArr * newJagArr(int dim,char * lo,char *hi){
		JagArr * tmp=malloc(1*sizeof(JagArr));
		tmp->type=malloc(sizeof(char) * (strlen("jaggedArray")+1));
		strcpy(tmp->type,"jaggedArray");
		tmp->dimensions=malloc(20*sizeof(char));
		itoa(dim,tmp->dimensions);
		tmp->basic="integer";
		(tmp->r1).low=lo;
		(tmp->r1).high=hi;
		int h=atoi(hi);
		int l=atoi(lo);
		tmp->range=malloc((h-l+1)*sizeof(jagRange));//Manually populate ranges after this
		return tmp;
}
void populateJagArr(JagArr * jarr,int index,char * sz,int list){
	(jarr->range[index]).size=sz;
	if(list==0){
		(jarr->range[index]).subRange=NULL;
	}else{
		(jarr->range[index]).subRange=malloc(atoi(sz)*sizeof(char *)); //Manually loop and fill values
	}
}
void populateJagArrSubrange(JagArr * jarr,int index,int subindex,char * val){
	(jarr->range[index]).subRange[subindex]=val;
}
char * printJagArr(JagArr * jarr){
	char * string=malloc(1024*sizeof(char));
	char * buff = malloc(20 * sizeof(char));
	strcpy(string,"<type=");
	strcat(string,jarr->type);
	strcat(string,", dimensions=");
	strcat(string,jarr->dimensions);
	strcat(string,", range_R1=(");
	strcat(string,(jarr->r1).low);
	strcat(string,",");
	strcat(string,(jarr->r1).high);
	strcat(string,")");
	int h=atoi((jarr->r1).high);
	int l=atoi((jarr->r1).low);
	strcat(string,", range_R2=( ");
	for(int i=0;i<h-l+1;i++){
			strcat(string,(jarr->range[i]).size);
			if(jarr->range[i].subRange!=NULL){
				strcat(string,"[ ");
				int sz=atoi((jarr->range[i]).size);
				for(int j=0;j<sz;j++){
					strcat(string,(jarr->range[i]).subRange[j]);
					if(j!=sz-1){
						strcat(string,",");
					}
					else{
						strcat(string," ");
					}
				}
				strcat(string,"]");
			}
			if(i!=h-l){
				strcat(string,", ");
			}else{
				strcat(string," ");
			}
	}
	strcat(string,")");
	strcat(string,", basicElementType=");
	strcat(string,jarr->basic);
	strcat(string,">");
	free(buff);
	return string;
}
primitive getType(char *s){
	if(strcmp(s,"integer")==0){
		return Int;
	}
	if(strcmp(s,"boolean")==0){
		return Bool;
	}
	if(strcmp(s,"real")==0){
		return Real;
	}
}	
void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  
         n = -n;          
     i = 0;
     do {     
         s[i++] = n % 10 + '0';   
     } while ((n /= 10) > 0);     
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 
 //HashMap
 void newHashMap(int size)
{
    if(map!=NULL){free(map);}
	map=malloc(size*sizeof(mapNode *));
    for(int i = 0; i < size; i++){
		map[i] = NULL;
	} 
	mapSize=size;
}
int hashValue(char * key){
	int hash = 31;
	for (int i = 0; i < strlen(key); i++) {
		hash = hash*7 + key[i]*(i+1);
	}
    hash%=mapSize;
	return hash>0?hash:-hash;
}
void add(char * key,int value)
{
	
    mapNode *tmp = malloc(sizeof(mapNode));
	tmp->key=NULL;
    tmp->value = value;
    tmp->next = NULL;
	int hash=hashValue(key);
    if(map[hash] == NULL){
		tmp->key=malloc((strlen(key)+1)*sizeof(char));
		strcpy(tmp->key,key);
		map[hash] = tmp;
	}
    else
    {
        mapNode *h = map[hash];
        while(h->next)
        {
            h = h->next;
        }
        h->next = tmp;
    }
}
mapNode * search(char * key)
{
    int hash = hashValue(key);
    mapNode *temp = map[hash];
    return temp;
}

void printMap()
{
    for(int i = 0; i < mapSize; i++)
    {
        mapNode *temp = map[i];
        if(temp!=NULL){
			printf("map[%d] with key %s\n",i,temp->key);
			
			while(temp)
			{
				printf("%d -->",temp->value);
				temp = temp->next;
			}
			printf("NULL\n");
		}
    }
}
void mapToLL(){
	if(map!=NULL){
		 for(int i = 0; i < mapSize; i++)
		{
			if(map[i]!=NULL){
				addNonTerminals(map[i]->key);
			}
		}
	}
}
void addNonTerminals(char * s){
		if(nt==NULL){
			nt=malloc(1*sizeof(allNonTerminals));
			nt->head=malloc(1*sizeof(ntNode*));
			nt->tail=nt->head;
			nt->head->val=s;
			nt->head->next=NULL;
		}
		if(nt->tail){
			nt->tail->next=malloc(1*sizeof(ntNode*));
			nt->tail=nt->tail->next;
			nt->tail->val=s;
			nt->tail->next=NULL;
		}
}
int isNonTerminal(char *s){
	ntNode * tmp=nt->head;
	while(tmp){
		if(strcmp(tmp->val,s)==0){
			return 1;
		}
		tmp=tmp->next;
	}
	return 0;
}

parseTree * newTree(){
	parseTree * pt=malloc(1*sizeof(parseTree));
	pt->start=NULL;
	return pt;
}
treeNode * newTreeNode(int level,char * token,int hasType){
	treeNode * tn=malloc(1*sizeof(treeNode));
	tn->level=level;
	tn->token=token;
	tn->hasType=hasType;
	tn->child=NULL;
	tn->sibling=NULL;
	if(hasType==0){
		(tn->v).other=malloc(sizeof(char)*(strlen("none")+1));
		strcpy((tn->v).other,"none");
	}else{
		(tn->v).type=NULL;// manully add Type
	}
	return tn;
}
void addSibling(treeNode * tn,treeNode *sib){
	treeNode * tmp=tn;
	while(tn->sibling){
		tn=tn->sibling;
	}
	tn->sibling=sib;
}
void addChild(treeNode *tn, treeNode *chi){
	if(tn->child){
		addSibling(tn->child,chi);
	}
	else{
		tn->child=chi;
	}
}
void deleteChild(treeNode *tn){
	treeNode * t=tn->child;
	if(t){
		free(t);
	}
	tn->child=NULL;
}
void newStack(){
	stack0=malloc(1*sizeof(Stack));
	stack0->top=-1;
	stack0->size=5;
	stack0->symbol=malloc(5*sizeof(char *));
	stack1=malloc(1*sizeof(Stack));
	stack1->top=-1;
	stack1->size=5;
	stack1->symbol=malloc(5*sizeof(char *));
}
void push(char * s,int i){
	if(i==0){
		if(isFull(i)){
		stack0->size=2*stack0->size;
		stack0->symbol=realloc(stack0->symbol,stack0->size*sizeof(char *));
		}
		stack0->top++;
		(stack0->symbol)[stack0->top]=s;
	}else if(i==1){
		if(isFull(i)){
		stack1->size=2*stack0->size;
		stack1->symbol=realloc(stack1->symbol,stack1->size*sizeof(char *));
		}
		stack1->top++;
		(stack1->symbol)[stack1->top]=s;
	}	
}
char * pop(int i){
	if(i==0){
		if(!isEmpty(i)){
			char * s=stack0->symbol[stack0->top];
			stack0->top--;
			return s;
		}
		else{
			return NULL;
		}
	}else if(i==1){if(!isEmpty(i)){
			char * s=stack1->symbol[stack1->top];
			stack1->top--;
			return s;
		}
		else{
			return NULL;
		}
	}
	
}
int isEmpty(int i){
	if(i==0){
		return stack0->top==-1?1:0;
	}else if(i==1){
		return stack1->top==-1?1:0;
	}
}
int isFull(int i){
	if(i==0){
		if(stack0->top==stack0->size-1){
			return 1;
		}
		return 0;
	}else if(i==1){
		if(stack1->top==stack1->size-1){
			return 1;
		}
		return 0;
	}
}