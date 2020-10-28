#include "functions.h"
#include "parseTreeFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXCAPACITY 10000

Stack *st;
char *z0;
FILE *f;
tokenNode *tn;

void printStack(Stack *st)
{
    f = fopen("out.txt", "a");

    int count = st->top;
    printf("Top- %d\n", count);
    while (count>=0)
    {
        fprintf(f, "Element %s\n", st->token[count]);
        count--;
    }
    fclose(f);
}

int isT(char *token)
{
    f = fopen("out.txt", "a");
    fprintf(f, "#############################################\n");
    fprintf(f, "Lexeme- %s and token- %s Next lexeme- %s\n", tn->lexeme, token, tn->next->lexeme);
    fclose(f);
    printStack(st);
    f = fopen("out.txt", "a");

    fprintf(f, "#############################################\n");
    fclose(f);

    if (!isNonTerminal(token))
    {
        return 1;
    }
    return 0;
}

int createParseTree(parseTree *t, tokenStream *s, grammar G)
{
    z0 = (char *)malloc(sizeof(char) * 5);
    strcpy(z0, "@$@$");
    char *baseKwd = (char *)malloc(sizeof(char) * 8);
    strcpy(baseKwd, "PROGRAM");

    st = createStack(MAXCAPACITY);
    t = newNode(baseKwd);
    push(st, baseKwd); // push to stack
    int zcnt= 0;
    int result;
    tokenNode *head = s->head;
    tn = s->head;

    result = expand(&G, t, &zcnt);
    return result; // result= 1 for successful tree creation, else -1
}

int expand(grammar *G, parseTree *parent, int * zcnt)
{
    char *tkn = pop(st);
    int flag = 1;
    
    tokenNode *temp_tn = tn;

    if (isT(tkn))
    {
        if (!strcmp(tkn, tn->lexeme) || (!strcmp(tn->token, "ID") && !strcmp(tkn, "id")) || (!strcmp(tn->token, "STAT_INT") && !strcmp(tkn, "int")))
        {
            tn = tn->next;
            
            while (*zcnt >1)
            {
                if(!strcmp(peek(st), z0)){
                    pop(st);
                    *zcnt= *zcnt -1;
                }
                else
                    break;
            }

            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        push(st, z0);
        *zcnt= *zcnt + 1;
        mapNode *rules_list = search(tkn);
        while (rules_list)
        {
            Node *tmp = &(G->rules[rules_list->value]);
            int cnt = 0;

            Stack *temp_stack = createStack(100);
            tmp = tmp->next;

            int pqr = decl_count;
            int mnq = assign_count;
            while (tmp)
            {
                if (!strcmp(tmp->name, "STMTS"))
                {

                    while (pqr--)
                    {
                        push(temp_stack, "DECLARATIONS");
                        cnt++;
                    }
                    while (mnq--)
                    {
                        push(temp_stack, "ASSIGNMENT");
                        cnt++;
                    }
                    tmp = tmp->next;
                }

                else
                {
                    push(temp_stack, tmp->name);
                    cnt++;
                    tmp = tmp->next;
                }
            }
            while (!isEmpty(temp_stack))
            {
                push(st, pop(temp_stack));
            }
            free(temp_stack);
            int correctness_flag = 1;
            for (int i = 0; i < cnt; i++)
            {
                int res = expand(G, parent, zcnt);
                if (res == -1)
                {

                    if(isEmpty(st))
                        return -1;

                    while (!strcmp(peek(st), z0))
                    {
                        pop(st);
                    }
                    pop(st);

                    correctness_flag = 0;
                    break;
                }
            }

            if (!correctness_flag)
            {
                rules_list = rules_list->next;
                tn = temp_tn;
            }
            else
            {
                return 1;
            }
        }
        return -1;
    }
}