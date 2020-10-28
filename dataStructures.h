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
typedef union variant{Type * type;char * other;}variant; 
typedef struct treeNode{
	int level;
	char * token;
	struct treeNode * child;
	struct treeNode * sibling;
	int hasType;
	variant v;
}treeNode;
typedef struct paseTree{
	treeNode * start;
}parseTree;
parseTree * newTree();
treeNode * newTreeNode(int,char *,int);
void addSibling(treeNode *,treeNode *);
void addChild(treeNode *,treeNode *);
void deleteChild(treeNode *);
typedef struct stack{
	char ** symbol;
	int top;
	int size;
}Stack;
void newStack();
void push(char *,int);
char * pop(int);
int isEmpty(int);
int isFull(int);