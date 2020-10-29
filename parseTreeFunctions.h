int isTerminal(char* token);
int createParseTree(parseTree *t, tokenStream *s, grammar G);
int expand(grammar *G, treeNode * p, int * z);