#define MAX_LENGTH 25
extern int grammarSize;
extern int typeSize;
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
typedef enum category{Primitive,Rectangular,Jagged}category;
typedef union typeExpression{char * primitive;char * rectangular;char * jagged;}typeExpression;
typedef struct type{
	char * field1;//var name
	category field2;
	char * field3;//rectangular array type or not_applicable
	typeExpression field4;	
}Type;
typedef struct table{
	Type ** arr;
}typeExpressionTable;
typeExpressionTable* newTable();
Type * newType(char *,category,char *,typeExpression);
typeExpression newTypeExpression(char *,category);
void addType(typeExpressionTable *,Type *);
void printTab(typeExpressionTable *);

