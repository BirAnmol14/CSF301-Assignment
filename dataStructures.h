#define Arr_Size 10
#define MAX_LENGTH 25
typedef struct node{
	char * name;
	struct node * next;
}Node;
typedef struct Grammar{
	Node rules[Arr_Size];
}grammar;
void newGrammar(grammar *);
void newLHS(Node *,char *);
void newRHS(Node *,char *);
