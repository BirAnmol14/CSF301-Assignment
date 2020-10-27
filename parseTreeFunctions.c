#include "functions.h"
#include "parseTreeFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXCAPACITY 10000

Stack *st;

int isTerminal(char *token)
{
    if (search(token) == NULL)
    {
        return 1;
    }
    return 0;
}

int createParseTree(parseTree *t, tokenStream *s, grammar G)
{
    st = createStack(MAXCAPACITY);
    t = newNode("PROGRAM");
    push(st, "PROGRAM"); // push to stack
    int result;
    tokenNode *head = s->head;
    // printf("\n1111111\n");
    while (!isEmpty(st))
        result = checkTree(&G, head, t);
    return result; // result= 1 for successful tree creation, else -1
}

int checkTree(grammar *G, tokenNode *tn, parseTree *parent)
{
    char *tkn;
    tkn= peek(st);
    int flag = 1;

    if (isTerminal(tkn))
    {
        // printf("11111\n");
        // fflush(0);
        pop(st);

        if (!strcmp(tkn, tn->token))
        {
            tn = tn->next;
            parseTree *node = newNode(tn->lexeme);
            // printf("\n22222222\n");
            addChild(parent, node);
            // printf("\n3333333333\n");
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        tokenNode *temp = tn;
        parseTree *currNode;
        push(st, "@$@$");

        mapNode *rules_list = search(tn->token);

        while (rules_list && flag)
        {
            char *grm_rule;


		Node * tmp =&(G->rules[rules_list->value]);
		printf("%d ",rules_list->value);

            // strcpy(grm_rule, (&G->rules[rules_list->value])->name);
            //
            // char *curr_tkn = strtok(grm_rule, " \t");
            currNode = newNode(tmp->name);
            int cnt;

            Stack *temp_stack = createStack(20);
            // temp_stack->token = (char **)malloc(20 * sizeof(char *));

        tmp= tmp->next;
        //                 printf("%s\n", tmp->name);
        // fflush(0);
        while(tmp){
            push(temp_stack, tmp->name);
            cnt++;
			tmp=tmp->next;
		}
            while (!isEmpty(temp_stack))
            {
        //                 printf("22222\n");
        // fflush(0);
                push(st, pop(temp_stack));
            }
            free(temp_stack);

            int correctness_flag = 1;
            for (int i = 0; i < cnt; i++)
            {
                int res = checkTree(G, tn->next, currNode);
                if (res == -1)
                {
                    tn = temp;
                    freeChildren(parent);
                    correctness_flag = 0;

        //         printf("333333\n");
        // fflush(0);

                    while (!strcmp(peek(st), "@$@$"))
                    {
                        pop(st);
                    }
                    rules_list = rules_list->next;
                    break;
                }
            }
            if (correctness_flag == 1)
            {
                addChild(parent, currNode);

        //         printf("444444\n");
        // fflush(0);
        //
        //                 printf("55555\n");
        // fflush(0);
                while (!strcmp(peek(st), "@$@$"))
                {
                    pop(st);
                }
        //         printf("666666\n");
        // fflush(0);
                pop(st);
        //             printf("7777777\n");
        // fflush(0);
                pop(st); // to remove the non-terminal which just completed
                flag = 0;
            }
        }
    }
    if (flag == 0)
        return 1;
    else
        return -1;
}
