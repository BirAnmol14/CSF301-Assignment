int isTerminal(char* token);
int createParseTree(parseTree *t, tokenStream *s, grammar G);
int checkTree(grammar *G, parseTree *t);
parseTree * newNode(char *); // create and return new node for parsetree
parseTree * addChild(parseTree * parent, parseTree * child);
void freeChildren(parseTree *parent);