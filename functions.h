#include "dataStructures.h"
void readGrammar(char *,grammar *);
void tokeniseSourcecode(char *,tokenStream *a);
void printTypeExpressionTable(typeExpressionTable *);
char * getToken(char *);
int isValidVarId(char *);
void newError(int line, stmtType, char *op, char *lexeme1, category c1, typeExpression te1, char * lexeme2, category c2, typeExpression te2, int level, char * msg);
void traverseParseTree(parseTree *root, typeExpressionTable *te);
treeNode *traverseDeclarationTree(treeNode *root, typeExpressionTable *te);
typeExpression *searchTET(char *varname, typeExpressionTable *te);
void traverseAssignmentTree(treeNode *root, typeExpressionTable *te);