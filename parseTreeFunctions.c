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
    treeNode* stmts=root->start;
    stmts=stmts->sibling->sibling->sibling; //start of statements
    treeNode* declist=stmts->child;
    treeNode* asslist=declist->sibling;
    puts("Traversing declaration tree...\n");
    traverseDeclarationTree(declist, te);
    puts("Traversing assignment tree...\n");
    traverseAssignmentTree(asslist, te);
}

void traverseDeclarationTree(treeNode * root, typeExpressionTable * te){
    treeNode* curDec=root->child->child;
    treeNode* declist=root->child->sibling;
    if(!strcmp(curDec->token,"PRIMITIVEDECLARATION")){  //if primitive declaration...(no possible errors in primitives)
        curDec=curDec->child;
        if(!strcmp(curDec->token,"SINGLEPRIMITIVE")){   //if single primitive...
            char* idvalue=curDec->child->sibling->child->token;
            char* datatype=curDec->child->sibling->sibling->sibling->child->token;
            curDec->child->sibling->hasType=1;
            curDec->child->sibling->v.type=newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype)));
            addType(te,newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype))));
        }
        else{   //if list of primitives...
            treeNode * idlist=curDec->child->sibling->sibling->sibling->sibling;
            char * datatype=idlist->sibling->sibling->child->token;
            char * idvalue=idlist->child->child->token;
            //absorbing first id to enter recursive loop of id sublist
            idlist->child->v.type=newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype)));
            idlist->child->hasType=1;
            addType(te,newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype))));
            idlist=idlist->child->sibling;
            while(1){
                idvalue=idlist->child->token;
                idlist->v.type=newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype)));
                idlist->hasType=1;
                addType(te,newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype))));
                if(idlist->sibling){
                    idlist=idlist->sibling->child;
                }
                else{
                    break;
                }
            }
        }
    }
    else if(!strcmp(curDec->token,"ARRAYDECLARATION")){ //if array declaration...(no poossible errors in arrays)
        //char * datatype="integer";
        if(!strcmp(curDec->child->sibling,"ID")){   //if single array...
            char * idvalue=curDec->child->sibling->child->token;
            treeNode* dimlist=curDec->child->sibling->sibling->sibling->sibling;
            int dimCount=0;
            int dynamic=0;
            arrRange* listofranges=(arrRange*) malloc(3*sizeof(arrRange));
            while(1){
                if(dimlist){
                    dimCount++;
                    dimlist=dimlist->child;
                    if((!strcmp(dimlist->child->sibling->child->token,"ID"))||(!strcmp(dimlist->child->sibling->sibling->sibling->child->token,"ID"))){   //if array is dynamically bounded...
                        dynamic=1;
                    }
                    char * lo=dimlist->child->sibling->child->child->token;
                    char * hi=dimlist->child->sibling->sibling->sibling->child->child->token;
                    arrRange[dimCount-1].low=lo;
                    arrRange[dimCount-1].high=hi;
                    dimlist=dimlist->sibling;
                }
                else{
                    break;
                }
            }
            curDec->child->sibling->hasType=1;
            RectArr* rarr=newRectArr(dimCount);
            for(int i=0; i<dimCount;i++){
                populateRectArr(rarr,i,listofranges[i].low,listofranges[i].high);
            }
            free(listofranges);
            if(dynamic){
                curDec->child->sibling->v.type=newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr));
                addType(te,newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr));
            }
            else{
                curDec->child->sibling->v.type=newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr));
                addType(te,newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr));
            } 
            
        }
        else{   //if multiple arrays...
            treeNode * idlist=curDec->child->sibling->sibling->sibling->sibling;
            treeNode* dimlist=curDec->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
            int dimCount=0;
            int dynamic=0;
            arrRange* listofranges=(arrRange*) malloc(3*sizeof(arrRange));
            while(1){
                if(dimlist){
                    dimCount++;
                    dimlist=dimlist->child;
                    if((!strcmp(dimlist->child->sibling->child->token,"ID"))||(!strcmp(dimlist->child->sibling->sibling->sibling->child->token,"ID"))){   //if array is dynamically bounded...
                        dynamic=1;
                    }
                    char * lo=dimlist->child->sibling->child->child->token;
                    char * hi=dimlist->child->sibling->sibling->sibling->child->child->token;
                    arrRange[dimCount-1].low=lo;
                    arrRange[dimCount-1].high=hi;
                    dimlist=dimlist->sibling;
                }
                else{
                    break;
                }
            }
            char * idvalue=idlist->child->child->token;
            RectArr* rarr=newRectArr(dimCount);
            for(int i=0; i<dimCount;i++){
                populateRectArr(rarr,i,listofranges[i].low,listofranges[i].high);
            }
            free(listofranges);

            idlist->child->hasType=1;
            //absorbing first id to enter recursive loop of id sublist
            if(dynamic){
                idlist->child->v.type=newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr)));
                addType(te,newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr)));
            }
            else{
                idlist->child->v.type=newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr));
                addType(te,newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr));
            }
            
            idlist=idlist->child->sibling;
            while(1){
                idvalue=idlist->child->token;
                idlist->v.type=newType(idvalue,Rectangular,dynamic ? "dynamic" :"static",newRectTypeExpression(rarr));
                idlist->hasType=1;
                addType(te,newType(idvalue,Rectangular,dynamic ? "dynamic" : "static",newRectTypeExpression(rarr)));
                if(idlist->sibling){
                    idlist=idlist->sibling->child;
                }
                else{
                    break;
                }
            }
        }

    }
    else{   //if jagged array...(errors possible here!)(not being handled as of now)
        if(!strcmp(curDec->child->child->token,"JAGGEDSINGLE2D")){  //if one 2D jagged array...
            treeNode * jarrdec=curDec->child->child->child;
            char * idvalue=jarrdec->sibling->child->token;
            char * lo=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char * hi=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr* jarr=newJagArr(2,lo,hi);
            treeNode* rows=curDec->child->sibling->child;   //onerow token
            for(int i=0; ;i++){
                if(atoi(rows->child->sibling->sibling)>atoi(hi)-atoi(lo)){
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                }else{
                char * size=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                treeNode* rowvals=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr,i,size,0);
                }
                for(int j=0;j<atoi(size);j++){  //error checking for dim(1)(incomplete)
                    if(rowvals->child->child->sibling){
                        puts("Error to be reported for 3D rowvals in 2D jagged array.\n");
                    }

                    if(rowvals->child->sibling->sibling){
                        rowvals=rowvals->child->sibling->sibling;
                    }
                    else{
                        puts("Error to be reported for lesser than mentioned in dim(1) for 2D jarr,\n");
                        break;
                    }
                }
                if(rows->sibling){
                    rows=rows->sibling->child;
                }
                else break;
            }
            addType(te, newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
            jarrdec->sibling->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            jarrdec->sibling->hasType=1;

        }
        else if(!strcmp(curDec->child->child->token,"JAGGEDSINGLE3D")){ //jagged 3d array...
            treeNode * jarrdec=curDec->child->child->child;
            int subsize=0;
            char * idvalue=jarrdec->sibling->child->token;
            char * lo=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char * hi=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr* jarr=newJagArr(3,lo,hi);
            treeNode* rows=curDec->child->sibling->child;   //onerow token
            treeNode* rowvals;
            treeNode* digitlist;
            for(int i=0; ;i++){
                if(atoi(rows->child->sibling->sibling)>atoi(hi)-atoi(lo)){
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                }else{
                char * size=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                rowvals=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr,i,size,1);
                }
                for(int j=0;j<atoi(size);j++){

                    digitlist=rowvals->child;
                    while(strcmp(digitlist->child->token,"EPSILON")){   //traverse subrange for subsize
                        subsize++;
                        digitlist=digitlist->child->sibling;
                    }
                    populateJagArrSubrange(jarr,i,j,itoa(subsize));

                    if(rowvals->child->sibling->sibling){
                        rowvals=rowvals->child->sibling->sibling;
                    }
                    else{
                        puts("Error to be reported for lesser than mentioned in dim(1) for 3D jarr,\n");
                        break;
                    }
                }
                if(rows->sibling){
                    rows=rows->sibling->child;
                }
                else break;
            }
            addType(te, newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
            jarrdec->sibling->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            jarrdec->sibling->hasType=1;
        }
        else if(!(strcmp(curDec->child->child->token,"JAGGED2D"))){
            treeNode * jarrdec=curDec->child->child->child;
            treeNode * idlist=jarrdec->sibling->sibling->sibling->sibling;
            char * lo=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char * hi=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr* jarr=newJagArr(2,lo,hi);
            treeNode* rows=curDec->child->sibling->child;   //onerow token
            for(int i=0; ;i++){
                if(atoi(rows->child->sibling->sibling)>atoi(hi)-atoi(lo)){
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                }else{
                char * size=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                treeNode* rowvals=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr,i,size,0);
                }
                for(int j=0;j<atoi(size);j++){  //error checking for dim(1)(incomplete)
                    if(rowvals->child->child->sibling){
                        puts("Error to be reported for 3D rowvals in 2D jagged array.\n");
                    }

                    if(rowvals->child->sibling->sibling){
                        rowvals=rowvals->child->sibling->sibling;
                    }
                    else{
                        puts("Error to be reported for lesser than mentioned in dim(1) for 2D jarr,\n");
                        break;
                    }
                }
                if(rows->sibling){
                    rows=rows->sibling->child;
                }
                else break;
            }
            char * idvalue=idlist->child->child->token;
            //absorbing first id to enter recursive loop of id sublist
            idlist->child->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            idlist->child->hasType=1;
            addType(te,newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
            idlist=idlist->child->sibling;
            while(1){
                idvalue=idlist->child->token;
                idlist->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
                idlist->hasType=1;
                addType(te,newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
                if(idlist->sibling){
                    idlist=idlist->sibling->child;
                }
                else{
                    break;
                }
            }
        }
        else if(!(strcmp(curDec->child->child->token,"JAGGED3D"))){
            treeNode * jarrdec=curDec->child->child->child;
            int subsize=0;
            treeNode * idlist=jarrdec->sibling->sibling->sibling->sibling;
            char * lo=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char * hi=jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr* jarr=newJagArr(3,lo,hi);
            treeNode* rows=curDec->child->sibling->child;   //onerow token
            treeNode* rowvals;
            treeNode* digitlist;
            for(int i=0; ;i++){
                if(atoi(rows->child->sibling->sibling)>atoi(hi)-atoi(lo)){
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                }else{
                char * size=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                rowvals=rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr,i,size,1);
                }
                for(int j=0;j<atoi(size);j++){

                    digitlist=rowvals->child;
                    while(strcmp(digitlist->child->token,"EPSILON")){   //traverse subrange for subsize
                        subsize++;
                        digitlist=digitlist->child->sibling;
                    }
                    populateJagArrSubrange(jarr,i,j,itoa(subsize));

                    if(rowvals->child->sibling->sibling){
                        rowvals=rowvals->child->sibling->sibling;
                    }
                    else{
                        puts("Error to be reported for lesser than mentioned in dim(1) for 3D jarr,\n");
                        break;
                    }
                }
                if(rows->sibling){
                    rows=rows->sibling->child;
                }
                else break;
            }
            char * idvalue=idlist->child->child->token;
            //absorbing first id to enter recursive loop of id sublist
            idlist->child->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            idlist->child->hasType=1;
            addType(te,newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
            idlist=idlist->child->sibling;
            while(1){
                idvalue=idlist->child->token;
                idlist->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
                idlist->hasType=1;
                addType(te,newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
                if(idlist->sibling){
                    idlist=idlist->sibling->child;
                }
                else{
                    break;
                }
            }
        }
    }
    traverseDeclarationTree(declist,te);
    return;
}

void traverseAssignmentTree(treeNode * root, typeExpressionTable * te){

}
