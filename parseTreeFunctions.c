#include "functions.h"
#include "parseTreeFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXCAPACITY 10000

Stack *st;
tokenNode *tn;

int isTerminal(char *token)
{
    if (search(token) == NULL)
    {
        return 1;
    }
    return 0;
}
void printStack(Stack* st){
  int count = st->top;
  while (count--){
    // printf("Element %s\n",st->token[count]);
  }
}
int createParseTree(parseTree *t, tokenStream *s, grammar G)
{
    st = createStack(MAXCAPACITY);
    t = newNode("PROGRAM");
    push(st, "PROGRAM"); // push to stack
    int result;
    tokenNode *head = s->head;

    // while (!isEmpty(st))
        result = checkTree(&G, head, t);
    return result; // result= 1 for successful tree creation, else -1
}

int checkTree(grammar *G, tokenNode *tn, parseTree *parent)
{


    char *tkn;
    tkn = peek(st);
    int flag = 1;

    // printf("tn- %s\n", tn->token);
    // printf("tkn= %s\n", tkn);
    if (isTerminal(tkn))
    {
        pop(st);

        // printf("\nTerminal found- %s\n", tkn);
        // printf("\nToken stream- %s\n", tn->lexeme);
        // printf("\nTring Compare %d\n",strcmp(tkn, tn->lexeme) );
        if (!strcmp(tkn, tn->lexeme))
        {
          // printf("And it matches the stream\n");
            parseTree *node = newNode(tn->lexeme);
            addChild(parent, node);
            tn = tn->next;
            printf("Next token to be checked- %s\n", tn->token);
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        // printf("\nStack top- %s\n", peek(st));

        tokenNode* temp = tn;
        parseTree *currNode;
        push(st, "@$@$");
        // puts("**********************************");
        // printStack(st);
        // puts("**********************************");

        mapNode *rules_list = search(tkn);
        //printf("\nStack top- %s\n", peek(st));
        while (rules_list && flag)
        {
            char *grm_rule;

            Node *tmp = &(G->rules[rules_list->value]);
            // printf("%d ", rules_list->value);

            currNode = newNode(tmp->name);
            int cnt= 0;

            Stack *temp_stack = createStack(20);
            tmp= tmp->next;
            while (tmp)
            {
                push(temp_stack, tmp->name);
                cnt++;
                tmp = tmp->next;
            }
            // printf("\nTemp Stack top- %s\n", peek(temp_stack));
            while (!isEmpty(temp_stack))
            {
                // printf("TempStack- %s\n", peek(temp_stack));
                push(st, pop(temp_stack));
            }
            // printf("\nStack top later- %s\n", peek(st));
            free(temp_stack);
            //printf("tn- %s\n", tn->token);
            int correctness_flag = 1;
            for (int i = 0; i < cnt; i++)
            {
                int res = checkTree(G, tn, currNode);
                if (res == -1)
                {
                    tn = temp;
                    freeChildren(parent);
                    correctness_flag = 0;

                    while (!strcmp(peek(st), "@$@$"))
                    {
                        pop(st);
                    }
                    // if(rules->next != NULL)
                        rules_list = rules_list->next;
                    // printf("\n%s\n",(&G->rules[rules_list->value])->name);
                    break;
                }
                else{
                  //printf("\ntn is equal to %s\n",tn->lexeme);
                    tn= tn->next;
                    //printf("\ntn is equal to %s\n",tn->lexeme);

                }
            }
            if (correctness_flag == 1)
            {
                addChild(parent, currNode);

                while (!strcmp(peek(st), "@$@$"))
                {
                    pop(st);
                }
                if(!strcmp(peek(st), "@$@$"))
                    pop(st);
                pop(st);
                flag = 0;
            }
        }
    }
    if (flag == 0)
        return 1;
    else
        return -1;
}

void traverseParseTree(parseTree * root, typeExpressionTable* te){
    parseTree* stmts=root->child;
    stmts=stmts->sibling->sibling->sibling; //start of statements
    parseTree* declist=stmts->child;
    parseTree* asslist=declist->sibling;
    puts("Traversing declaration tree...\n");
    traverseDeclarationTree(declist, te);
    puts("Traversing assignment tree...\n");
    traverseAssignmentTree(asslist, te);
}

void traverseDeclarationTree(parseTree * root, typeExpressionTable * te){
    parseTree* curDec=root->child;
    parseTree* declist=root->child->sibling;

}

void traverseAssignmentTree(parseTree * root, typeExpressionTable * te){

}
