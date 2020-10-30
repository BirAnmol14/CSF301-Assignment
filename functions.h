/* 
 Group number- 40

 Group Members:-

 		NAME 						ID
 1. Bir Anmol Singh 		2018A7PS0261P
 2. Gitansh Pujari 			2018A7PS0163P
 3. Rahul Barigidad 		2018A7PS0220P
 4. Tejas Tiwari 			2018A7PS0280P
*/

#include "dataStructures.h"
void readGrammar(char *,grammar *);
void tokeniseSourcecode(char *,tokenStream *a);
void printTypeExpressionTable(typeExpressionTable *);
char * getToken(char *);
int isValidVarId(char *);
void newError(int line, stmtType, char *op, char *lexeme1, category c1, typeExpression te1, char * lexeme2, category c2, typeExpression te2, int level, char * msg);
void traverseParseTree(parseTree *root, typeExpressionTable *te);
treeNode *traverseDeclarationTree(treeNode *root, typeExpressionTable *te);
Type *searchTET(char *varname, typeExpressionTable *te);
void traverseAssignmentTree(treeNode *root, typeExpressionTable *te);
teStruct * ComputeValidExpression(treeNode * id, treeNode * op, treeNode * expr, typeExpressionTable * tb);
teStruct * getTypeExpressionfor(treeNode * node, typeExpressionTable * te);