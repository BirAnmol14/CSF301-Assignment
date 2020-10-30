/* 
 Group number- 40

 Group Members:-

 		NAME 						ID
 1. Bir Anmol Singh 		2018A7PS0261P
 2. Gitansh Pujari 			2018A7PS0163P
 3. Rahul Barigidad 		2018A7PS0220P
 4. Tejas Tiwari 			2018A7PS0280P
*/

int isTerminal(char* token);
int createParseTree(parseTree *t, tokenStream *s, grammar G);
int expand(grammar *G, treeNode * p, int * z);