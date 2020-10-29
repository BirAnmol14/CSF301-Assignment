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
    while (count >= 0)
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

int predict(Node *tmp, tokenNode *tn)
{

    if(!strcmp(tmp->name, "DECLARATIONS")){
        int flag= 0;
        while(tn){
            if(!strcmp(tn->lexeme, "jagged")){
                flag= 1;
                break;
            }
            else if(!strcmp(tn->lexeme, "array")){
                flag= 2;
                break;
            }
            else if(!strcmp(tn->lexeme, ";"))
                break;
            tn= tn->next;
        }
        if(!strcmp(tmp->next->name, "PRIMITIVEDECLARATION") && flag== 0){
            return 0;
        }
        if(!strcmp(tmp->next->name, "ARRAYDECLARATION") && flag== 2){
            return 0;
        }
        if(!strcmp(tmp->next->name, "JAGGEDDECLARATION") && flag== 1){
            return 0;
        }
        return 1;
    }

    if (!strcmp(tmp->name, "PRIMITIVEDECLARATION") || !strcmp(tmp->name, "ARRAYDECLARATION") || !strcmp(tmp->name, "J2D") || !strcmp(tmp->name, "J3D") )
    {
        if (strcmp(tmp->next->next->name, "list") && !strcmp(tn->next->lexeme, "list"))
        {
            return 1;
        }
        return 0;
    }

    if (!strcmp(tmp->name, "ID_LIST"))
    {
        if (tmp->next->next && !strcmp(tmp->next->next->name, "ID_LIST") && strcmp(tn->next->token, "ID"))
        {
            return 1;
        }
        return 0;
    }

    if (!strcmp(tmp->name, "EXPRESSION"))
    {
        tmp= tmp->next;
        int flag=0;
        while(tmp){
            if(!strcmp(tmp->name, "OPERATOR")){
                flag= 1;
                break;
            }
            tmp= tmp->next;
        }
        if(flag)
        while(tn){
            printf("lexeme- %s\n", tn->lexeme);
            if(!strcmp(tn->token, "OPERATOR")){
                return 0;
            }
            else if(!strcmp(tn->lexeme, ";")){
                    return 1;
            }
            tn= tn->next;
        }
        return 0;
    }

    if (!strcmp(tmp->name, "BOOLEXPRESSION"))
    {
        tmp= tmp->next;
        int flag=0;
        while(tmp){
            if(!strcmp(tmp->name, "BOOLOP")){
                flag= 1;
                break;
            }
            tmp= tmp->next;
        }
        if(flag)
        while(tn){
            printf("lexeme- %s\n", tn->lexeme);
            if(!strcmp(tn->token, "BOOLOP")){
                return 0;
            }
            else if(!strcmp(tn->lexeme, ";")){
                    return 1;
            }
            tn= tn->next;
        }
        return 0;
    }

    if(!strcmp(tmp->name, "DIMENSION")){
        tmp= tmp->next;
        int flag=0;
        int flag2= 0;
        while(tmp){
            if(!strcmp(tmp->name, "DIMENSION")){
                flag= 1;
                break;
            }
            tmp= tmp->next;
        }
        if(!flag)
            return 0;
        printf("Flag= %d and 5th lex= %s\n", flag, tn->next->next->next->next->next->lexeme);
        if((!strcmp(tn->next->next->next->next->next->lexeme, "["))){
            flag2= 1;
        }
        if(flag== 1 && flag2== 1){
            return 0;
        }       

        return 1;
    }

    if(!strcmp(tmp->name, "STAT_LIST")){
        tmp= tmp->next;
        int flag= 0;
        while(tmp){
            if(!strcmp(tmp->name, "STAT_LIST")){
                flag= 1;
                break;
            }
            tmp= tmp->next;            
        }
        if(flag== 0)
            return 0;

        if(!strcmp(tn->token, "STAT_INT"))
            return 0;
        return 1;
    }

    if(!strcmp(tmp->name, "ASSIGNMENT") || !strcmp(tmp->name, "SUBEX")){
        tmp= tmp->next;
        int flag= 0;
        while(tmp){
            if(!strcmp(tmp->name, "ARR_DEF")){
                flag= 1;
                break;
            }
            tmp= tmp->next;            
        }

        int flag2=0;
        if(!strcmp(tn->next->lexeme, "[")){
            flag2=1;
        }
        
        if((flag && flag2) || (!flag && !flag2) )
            return 0;
        return 1;
    }

    if(!strcmp(tmp->name, "ROW3D")){
        tmp= tmp->next;
        int flag= 0;
        while(tmp){
            if(!strcmp(tmp->name, "ROW3D")){
                flag= 1;
                break;
            }
            tmp= tmp->next;            
        }

        int flag2=0;
        tn= tn->next;
        while(tn){
            if(!strcmp(tn->lexeme, "R1")){
                flag2= 1;
                break;
            }
            else if(!strcmp(tn->lexeme, "declare") || !strcmp(tn->token, "ID"))
                break;
            tn= tn->next;
        }
        
        if((flag && flag2) || (!flag && !flag2) )
            return 0;
        return 1;        
    }

    if(!strcmp(tmp->name, "STAT_SIZE")){
        tmp= tmp->next;
        int flag= 0;
        while(tmp){
            if(!strcmp(tmp->name, "STAT_SIZE")){
                flag= 1;
                break;
            }
            tmp= tmp->next;            
        }

        int flag2=0;
        if(!strcmp(tn->next->lexeme, ";")){
            flag2=1;
        }
        printf("%d %d %s\n", flag, flag2, tn->next->lexeme);
        if((flag && flag2) || (!flag && !flag2) )
            return 0;
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
    int zcnt = 0;
    int result;
    tokenNode *head = s->head;
    tn = s->head;

    result = expand(&G, t, &zcnt);
    return result; // result= 1 for successful tree creation, else -1
}

int expand(grammar *G, parseTree *parent, int *zcnt)
{

                    if(!tn->next && !strcmp(tn->lexeme, "}")){
                        while(!isEmpty(st))
                            pop(st);
                        printStack(st);
                        return 1;
                    }

    char *tkn = pop(st);
    int flag = 1;

    tokenNode *temp_tn = tn;

    if (isT(tkn))
    {
        if (!strcmp(tkn, tn->lexeme) || (!strcmp(tn->token, "ID") && !strcmp(tkn, "id")) || (!strcmp(tn->token, "STAT_INT") && !strcmp(tkn, "int")))
        {
            tn = tn->next;

            while (*zcnt > 1)
            {
                if (!strcmp(peek(st), z0))
                {
                    pop(st);
                    *zcnt = *zcnt - 1;
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
        if(!strcmp(tkn, "EPSILON")){
            while(!strcmp(peek(st), z0))
                pop(st);
            return 1;
        }

        push(st, z0);
        *zcnt = *zcnt + 1;
        mapNode *rules_list = search(tkn);
        while (rules_list)
        {
            Node *tmp = &(G->rules[rules_list->value]);
            int cnt = 0;


            if(predict(tmp, tn)){
                rules_list= rules_list->next;                    
                continue;
            }


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
                    if (isEmpty(st))
                        return -1;

                    while (strcmp(peek(st), z0))
                    {
                        pop(st);
                    }
                    pop(st);

                    if(!strcmp(tn->token, "OPERATOR") || !strcmp(tn->token, "BOOLOP") || !strcmp(tn->lexeme, "array") || !strcmp(tn->lexeme, "jagged"))
                        push(st, z0);

                    correctness_flag = 0;
                    break;
                }
            }

            if (!correctness_flag)
            {
                if(!strcmp(tkn, "DIT"))
                    push(st, z0);                
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