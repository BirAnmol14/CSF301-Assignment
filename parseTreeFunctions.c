#include "functions.h"
#include "parseTreeFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXCAPACITY 10000

Stack *st;
tokenNode *tn;
char *z0;

int isTerminal(char *token)
{
    if(!strcmp(token, "c")){            // JUGAAD
        return 1;
    }
    if(!strcmp(token, "C")){            // JUGAAD
        return 1;
    }
    if(!strcmp(token, "Q")){            // JUGAAD
        return 1;
    }
    printf("\n%s passed to function\n", token);
    if (search(token) == NULL)
    {
        printf("\n%s identified as terminal\n", token);
        return 1;
    }
    return 0;
}
void printStack(Stack* st){
  int count = st->top;
  while (count--){
    printf("Element %s\n",st->token[count]);
  }
}
int createParseTree(parseTree *t, tokenStream *s, grammar G)
{
    z0= (char *)malloc(sizeof(char)*5);
    strcpy(z0, "@$@$");
    char * baseKwd= (char *)malloc(sizeof(char)*8);
    strcpy(baseKwd, "PROGRAM");
    st = createStack(MAXCAPACITY);
    t = newNode(baseKwd);
    push(st, baseKwd); // push to stack
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

        // puts("**********************************");
        // printStack(st);
        // puts("**********************************");

    if (isTerminal(tkn))
    {
        pop(st);

        if (!strcmp(tkn, tn->lexeme))
        {
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
        tokenNode* temp = tn;
        parseTree *currNode;
        push(st, z0);

        printf("Current token- %s\n", tkn);
        printf("Current lexeme- %s\n", tn->lexeme);

        // puts("**********************************");
        // printStack(st);
        // puts("**********************************");

        mapNode *rules_list = search(tkn);

        while (rules_list && flag)
        {
            // push(st, z0);

            char *grm_rule;

            Node *tmp = &(G->rules[rules_list->value]);

            currNode = newNode(tmp->name);
            int cnt= 0;

            Stack *temp_stack = createStack(50);
            printf("First- %s\n", tmp->name);
            tmp= tmp->next;
            while (tmp)
            {
                printf("First- %s\n", tmp->name);
                push(temp_stack, tmp->name);
                cnt++;
                tmp = tmp->next;
            }
            while (!isEmpty(temp_stack))
            {
                printf("TempStack- %s\n", peek(temp_stack));
                push(st, pop(temp_stack));
            }
            free(temp_stack);
            int correctness_flag = 1;
            for (int i = 0; i < cnt; i++)
            {
                int res = checkTree(G, tn, currNode);
                if (res == -1)
                {
                    tn = temp;
                    freeChildren(parent);
                    correctness_flag = 0;
                    printf("FLGA1 %s\n", peek(st));
                    while (strcmp(peek(st), "@$@$"))
                    {
                        pop(st);
                    }
                    printf("FLAG2\n");
                    // pop(st);

                    printf("Line no at end= %d\n", rules_list->value);
                    rules_list = rules_list->next;

                    if(!rules_list){
                        printf("\nNULL after Z\n");
                        while (strcmp(peek(st), "@$@$"))
                        {
                            pop(st);
                        }
                        pop(st);
                        pop(st);
                        pop(st);                
                    }

                    break;
                }
                else{
                    tn= tn->next;
                }
            }
            if (correctness_flag == 1)
            {
                addChild(parent, currNode);

                while (strcmp(peek(st), "@$@$"))
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
