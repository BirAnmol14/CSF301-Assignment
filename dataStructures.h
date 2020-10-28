#define MAX_LENGTH 25
extern int grammarSize;
extern int typeSize;

extern int decl_count;
extern int assign_count;

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
typedef enum primitive{Int,Real,Bool} primitive;
primitive getType(char *);
typedef struct arrRange{
	char * low;
	char * high;
} arrRange;
typedef struct rectangularArr{
	char * type;
	char * dimensions;
	arrRange * range;
	char * basic;//set integer only
}RectArr;
typedef struct jagRange{
	char * size;
	char ** subRange;
}jagRange;
typedef struct jaggedArr{
	char * type;
	char * dimensions;
	arrRange r1;//R1 must
	jagRange * range;//R2 onwards
	char * basic;//set integer only
}JagArr;
typedef union typeExpression{primitive primType;RectArr * rectangular;JagArr * jagged;}typeExpression;
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
typeExpression newRectTypeExpression(RectArr *);
typeExpression newPrimTypeExpression(primitive);
typeExpression newJagTypeExpression(JagArr *);
void addType(typeExpressionTable *,Type *);
RectArr * newRectArr(int);
void populateRectArr(RectArr *,int,char *,char *);
JagArr * newJagArr(int,char *,char *);
void populateJagArr(JagArr *,int,char *,int);
void populateJagArrSubrange(JagArr * jarr,int,int,char * sz);
char * printRectArr(RectArr *);
char * printPrimType(primitive);
char * printJagArr(JagArr *);
void itoa(int,char *);
void reverse(char *);

//Hash Map
typedef struct mapNode
{
    int value;
	char * key;
    struct mapNode *next;
} mapNode;

// Parse Tree
typedef struct parsetree{
 char* token;
 struct parsetree * child;
struct parsetree * sibling;
 typeExpression type;
}parseTree;

typedef struct Stack {
    int top;
    unsigned capacity;
    char ** token;
}Stack;

Stack* createStack(unsigned capacity);
int isEmpty(Stack* stack);
int isFull(Stack* stack);
void push(Stack* stack, char *);
char * peek(Stack* stack);
char * pop(Stack* stack);

void newHashMap(int size);
int hashValue(char * key);
void add(char * key,int value);
mapNode * search(char * key);
void printMap();

void mapToLL();
typedef struct ntNode{
	char * val;
	struct ntNode * next;
}ntNode;
typedef struct allNonTerminals{
	ntNode * head;
	ntNode * tail;
}allNonTerminals;
void addNonTerminals(char *);
int isNonTerminal(char *);
void findHashes();
