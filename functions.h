#include "dataStructures.h"
void readGrammar(char *,grammar *);
void tokeniseSourcecode(char *,tokenStream *a);
void printTypeExpressionTable(typeExpressionTable *);
void createParseTree(parseTree *, tokenStream *, grammar );
char * getToken(char *);
int isValidVarId(char *);
int expandChild(treeNode *,tokenNode *,grammar);
int expandSibling(treeNode *,tokenNode *, grammar g);