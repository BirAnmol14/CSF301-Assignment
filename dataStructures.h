#define MAX_LENGTH 25
extern int grammarSize;
typedef struct node{
	char * name;
	struct node * next;
}Node;
typedef struct Grammar{
	Node *rules;
}grammar;
void newGrammar(char *,grammar *);
void newLHS(Node *,char *);
void newRHS(Node *,char *);
void printGrammar(grammar *);
typedef struct tokenNode{
	char * lexeme;
	char * token;
	int line;
	struct tokenNode * next;
}tokenNode;
typedef struct tokenStream{
	tokenNode * head;
	tokenNode * tail;
}tokenStream;
tokenStream * addTokenNode(tokenStream *,char *,char *,int);
tokenStream * newTokenStream();
tokenNode * newTokenNode(char *,char *,int);
void printTokenStream(tokenStream *);
