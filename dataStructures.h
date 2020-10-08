#define Arr_Size 10
#define MAX_LENGTH 25
typedef struct node{
	char * name;
	struct node * next;
}Node;
typedef struct Grammar{
	Node arr[Arr_Size];
}grammar;
void newGrammar(grammar *);
void newTerminal(Node *,char *);
void newNonTerminal(Node *,char *);
