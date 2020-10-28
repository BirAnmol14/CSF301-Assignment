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
FILE * f;
tokenNode * temp;
void printStack(Stack *st)
{
        f= fopen("out.txt", "a");

    int count = st->top;
    while (count--)
    {
        fprintf(f, "Element %s\n", st->token[count]);
    }
    fclose(f);
}

int isTerminal(char *token)
{
    f= fopen("out.txt", "a");
    fprintf(f, "#############################################\n");
    // puts("#############################################");
    fprintf(f, "Lexeme- %s and token- %s\n", tn->lexeme, token);
    fclose(f);
    printStack(st);
    // puts("#############################################");
        f= fopen("out.txt", "a");

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

    int result;
    tokenNode *head = s->head;
    tn = s->head;

    result = checkTree(&G, t);
    return result; // result= 1 for successful tree creation, else -1
}

int checkTree(grammar *G, parseTree *parent)
{
    char *tkn;
    tkn = peek(st);
    int flag = 1;

    if (isTerminal(tkn))
    {
        pop(st);
        if (!strcmp(tkn, tn->lexeme))
        {
            parseTree *node = newNode(tn->lexeme);
            addChild(parent, node);
            tn = tn->next;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
      temp = tn;
      parseTree *currNode;
      push(st, z0);


        mapNode *rules_list = search(tkn);

        while (rules_list && flag)
        {

            char *grm_rule;

            Node *tmp = &(G->rules[rules_list->value]);

            currNode = newNode(tmp->name);
            int cnt = 0;
            Stack *temp_stack = createStack(100);

            tmp = tmp->next;
            int pqr = decl_count;
            int mnq = assign_count;
            while (tmp)
            {     if(!strcmp(tmp->name,"STMTS")){

                while(pqr--){
                  push(temp_stack,"DECLARATION");
                  cnt++;
                }
                while(mnq--){
                  push(temp_stack,"ASSIGN_STMT");
                  cnt++;
                }
                tmp=tmp->next;
              }

              else{
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
              tokenNode* ab = tn;
                // if (!strcmp(tn->lexeme, "}"))
                // {
                //     return 1;
                // }

                if (!strcmp(peek(st), "ID"))
                {
                    if (!strcmp(tn->token, "ID"))
                    {
                        addChild(parent, newNode(tn->lexeme));
                        tn = tn->next;
                        pop(st);
                        continue;
                    }
                    else
                    { return -1;  
                      // tn = ab;
                      //   correctness_flag= 0;
                      //   rules_list= rules_list->next;
                      //   while (strcmp(peek(st), "@$@$"))
                      //   {
                      //       pop(st);
                      //   }
                      //   if (!strcmp(peek(st), "@$@$"))
                      //       pop(st);
                      //   // pop(st);
                      //   break;
                    }
                }

                if (!strcmp(peek(st), "STATIC_INT"))
                {
                    if (!strcmp(tn->token, "STATIC_INT"))
                    {
                        addChild(parent, newNode(tn->lexeme));
                        tn = tn->next;
                        pop(st);
                        continue;
                    }
                    else
                    { return -1;
                      // tn = ab;
                      //   rules_list= rules_list->next;
                      //   correctness_flag= 0;
                      //   while (strcmp(peek(st), "@$@$"))
                      //   {
                      //       pop(st);
                      //   }
                      //   if (!strcmp(peek(st), "@$@$"))
                      //       pop(st);
                      //   pop(st);
                      //   break;
                    }
                }

                if (!strcmp(tn->lexeme, "}"))
                {
                    return 1;
                }

                int res = checkTree(G, currNode);
                if (res == -1)
                {
                    tn = temp;
                    freeChildren(parent);
                    correctness_flag = 0;
                    while (strcmp(peek(st), "@$@$"))
                    {
                        pop(st);
                    }
                    // pop(st);

                    rules_list = rules_list->next;

                    if (!rules_list)
                    {
                        while (strcmp(peek(st), "@$@$"))
                        {
                            pop(st);
                        }
                        pop(st);
                        pop(st);
                    }
                    break;
                }
            }
            if (correctness_flag == 1)
            {
                addChild(parent, currNode);

                while (strcmp(peek(st), "@$@$"))
                {
                    pop(st);
                }
                if (!strcmp(peek(st), "@$@$"))
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
