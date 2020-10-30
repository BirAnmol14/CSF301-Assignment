#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void traverseParseTree(parseTree *root, typeExpressionTable *te)
{
    treeNode *stmts = root->start;
    stmts = stmts->child->sibling->sibling->sibling; //start of statements
    treeNode *declist = strcmp(stmts->token, "{") ? stmts : stmts->sibling;
    // treeNode* asslist=declist->sibling;
    puts("Traversing declaration tree...\n");
    treeNode *assList = traverseDeclarationTree(declist, te);
    // puts("Traversing assignment tree...\n");
    traverseAssignmentTree(assList, te);
}

treeNode *traverseDeclarationTree(treeNode *root, typeExpressionTable *te)
{
    treeNode * curDec=root;
    for(int i=0; i< decl_count;i++){
    treeNode *declist = curDec->sibling;
    treeNode *type_node = curDec;
    type_node->hasType = 1;
    curDec = curDec->child;
    if (!strcmp(curDec->token, "PRIMITIVEDECLARATION"))
    { //if primitive declaration...(no possible errors in primitives)
        curDec = curDec->child;
        if (!strcmp(curDec->sibling->token, "ID"))
        { //if single primitive...
            char *idvalue = curDec->sibling->child->token;
            char *datatype = curDec->sibling->sibling->sibling->token;
            // type_node->hasType=1;
            type_node->te= newPrimTypeExpression(getType(datatype));
            addType(te, newType(idvalue, Primitive, NULL, newPrimTypeExpression(getType(datatype))));
        }
        else
        { //if list of primitives...
            treeNode *idlist = curDec->sibling->sibling->sibling->sibling;
            char *datatype = idlist->sibling->sibling->token;
            char *idvalue;
            // char * idvalue=idlist->child->child->token;
            //absorbing first id to enter recursive loop of id sublist
            // idlist->child->v.type=newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype)));
            // idlist->child->hasType=1;
            // addType(te,newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype))));
            idlist = idlist->child;
            while (1)
            {
                idvalue = idlist->child->token;
                // idlist->v.type=newType(idvalue,Primitive,NULL,newPrimTypeExpression(getType(datatype)));
                // idlist->hasType=1;
                addType(te, newType(idvalue, Primitive, NULL, newPrimTypeExpression(getType(datatype))));
                type_node->te= newPrimTypeExpression(getType(datatype));
                if (idlist->sibling)
                {
                    idlist = idlist->sibling->child;
                }
                else
                {
                    break;
                }
            }
        }
    }
    else if (!strcmp(curDec->token, "ARRAYDECLARATION"))
    { //if array declaration...(no poossible errors in arrays)
        //char * datatype="integer";
        curDec = curDec->child;
        if (!strcmp(curDec->sibling->token, "ID"))
        { //if single array...
            char *idvalue = curDec->sibling->child->token;
            treeNode *dimlist = curDec->sibling->sibling->sibling->sibling;
            int dimCount = 0;
            int dynamic = 0;
            arrRange *listofranges = (arrRange *)malloc(3 * sizeof(arrRange));
            while (1)
            {

                dimCount++;
                dimlist = dimlist->child;
                if ((!strcmp(dimlist->child->sibling->token, "ID")) || (!strcmp(dimlist->child->sibling->sibling->sibling->token, "ID")))
                { //if array is dynamically bounded...
                    dynamic = 1;
                }
                char *lo = dimlist->child->sibling->child->token;
                char *hi = dimlist->child->sibling->sibling->sibling->child->token;
                listofranges[dimCount - 1].low = lo;
                listofranges[dimCount - 1].high = hi;

                if (dimlist->sibling)
                {
                    dimlist = dimlist->sibling;
                }
                else
                {
                    break;
                }
            }
            // curDec->child->sibling->hasType=1;
            RectArr *rarr = newRectArr(dimCount);
            for (int i = 0; i < dimCount; i++)
            {
                populateRectArr(rarr, i, listofranges[i].low, listofranges[i].high);
            }
            free(listofranges);
            if (dynamic)
            {
                // curDec->child->sibling->v.type=newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr));
                addType(te, newType(idvalue, Rectangular, "dynamic", newRectTypeExpression(rarr)));
                type_node->te= newRectTypeExpression(rarr);
            }
            else
            {
                // curDec->child->sibling->v.type=newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr));
                addType(te, newType(idvalue, Rectangular, "static", newRectTypeExpression(rarr)));
                type_node->te= newRectTypeExpression(rarr);
            }
        }
        else
        { //if multiple arrays...
            treeNode *idlist = curDec->sibling->sibling->sibling->sibling;
            treeNode *dimlist = curDec->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
            int dimCount = 0;
            int dynamic = 0;
            arrRange *listofranges = (arrRange *)malloc(3 * sizeof(arrRange));
            while (1)
            {

                dimCount++;
                dimlist = dimlist->child;
                if ((!strcmp(dimlist->child->sibling->token, "ID")) || (!strcmp(dimlist->child->sibling->sibling->sibling->token, "ID")))
                { //if array is dynamically bounded...
                    dynamic = 1;
                }
                char *lo = dimlist->child->sibling->child->token;
                char *hi = dimlist->child->sibling->sibling->sibling->child->token;
                listofranges[dimCount - 1].low = lo;
                listofranges[dimCount - 1].high = hi;
                if (dimlist->sibling)
                {
                    dimlist = dimlist->sibling;
                }
                else
                {
                    break;
                }
            }
            char *idvalue;
            // char * idvalue=idlist->child->child->token;
            RectArr *rarr = newRectArr(dimCount);
            for (int i = 0; i < dimCount; i++)
            {
                populateRectArr(rarr, i, listofranges[i].low, listofranges[i].high);
            }
            free(listofranges);

            // // idlist->child->hasType=1;
            // //absorbing first id to enter recursive loop of id sublist
            // if(dynamic){
            //     // idlist->child->v.type=newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr));
            //     addType(te,newType(idvalue,Rectangular,"dynamic",newRectTypeExpression(rarr)));
            // }
            // else{
            //     // idlist->child->v.type=newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr));
            //     addType(te,newType(idvalue,Rectangular,"static",newRectTypeExpression(rarr)));
            // }

            idlist = idlist->child;
            while (1)
            {
                idvalue = idlist->child->token;
                // idlist->v.type=newType(idvalue,Rectangular,dynamic ? "dynamic" :"static",newRectTypeExpression(rarr));
                // idlist->hasType=1;
                addType(te, newType(idvalue, Rectangular, dynamic ? "dynamic" : "static", newRectTypeExpression(rarr)));
                type_node->te= newRectTypeExpression(rarr);
                if (idlist->sibling)
                {
                    idlist = idlist->sibling->child;
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    { //if jagged array...(errors possible here!)(not being handled as of now)
        curDec = curDec->child;
        int errno1 = 0;
        int errno2 = 0;
        int errno3 = 0;

        if (!strcmp(curDec->token, "J2D") && !strcmp(curDec->child->sibling->token, "ID"))
        { //if one 2D jagged array...
            treeNode *jarrdec = curDec->child;
            char *idvalue = jarrdec->sibling->child->token;
            char *lo = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char *hi = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr *jarr = newJagArr(2, lo, hi);
            treeNode *rows = curDec->sibling; //row3d token
            for (int i = 0;; i++)
            {
                if (atoi(rows->child->sibling->sibling->child->token) > atoi(hi)||atoi(rows->child->sibling->sibling->child->token) < atoi(lo))
                {
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                    // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te)
                    errno1 = 1;
                }
                char *size = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                treeNode *rowvals = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr, i, size, 0);

                for (int j = 0; j < atoi(size); j++)
                { //error checking for dim(1)(incomplete)
                    if (!strcmp(rowvals->child->child->token, "EPSILON") || strcmp(rowvals->child->child->sibling->child->token, "EPSILON"))
                    {

                        puts("Error to be reported for 3D rowvals in 2D jagged array.\n");
                        errno2 = 1;
                    }

                    if (rowvals->child->sibling)
                    {
                        rowvals = rowvals->child->sibling->sibling;
                    }
                    else
                    {
                        puts("Error to be reported for lesser than mentioned in dim(1) for 2D jarr,\n");
                        errno3 = 1;
                        break;
                    }
                }
                if (rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling)
                {
                    rows = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                }
                else
                    break;
            }
            addType(te, newType(idvalue, Jagged, NULL, newJagTypeExpression(jarr)));
            type_node->te= newJagTypeExpression(jarr);
            if (errno1)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level," dim(0) out of bounds in 2D jarr")
            }
            if (errno2)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level,"3D rowvals in 2D jagged array")
            }
            if (errno3)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level,"dim(1) too short for 2D jarr")
            }
            // jarrdec->sibling->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            // jarrdec->sibling->hasType=1;
        }
        else if (!strcmp(curDec->token, "J3D") && !strcmp(curDec->child->sibling->token, "ID"))
        { //jagged 3d array...
            int errno1 = 0;
            // int errno2 = 0;
            int errno3 = 0;
            treeNode *jarrdec = curDec->child;
            char *idvalue = jarrdec->sibling->child->token;
            char *lo = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char *hi = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr *jarr = newJagArr(3, lo, hi);
            treeNode *rows = curDec->sibling; //onerow token
            int subsize = 0;
            treeNode *rowvals;
            treeNode *digitlist;
            for (int i = 0;; i++)
            {
                if (atoi(rows->child->sibling->sibling->child->token) > atoi(hi)||atoi(rows->child->sibling->sibling->child->token) < atoi(lo))
                {
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                    errno1 = 1;
                }
                char *size = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                rowvals = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr, i, size, 1);

                for (int j = 0; j < atoi(size); j++)
                {

                    digitlist = rowvals->child; //STAT_LIST
                    while (strcmp(digitlist->child->token, "EPSILON"))
                    { //traverse subrange for subsize
                        subsize++;
                        digitlist = digitlist->child->sibling;
                    }
                    char *buff = (char *)malloc(sizeof(char) * 5);
                    itoa(subsize, buff);
                    populateJagArrSubrange(jarr, i, j, buff);

                    if (rowvals->child->sibling)
                    {
                        rowvals = rowvals->child->sibling->sibling;
                    }
                    else
                    {
                        puts("Error to be reported for lesser than mentioned in dim(1) for 3D jarr,\n");
                        errno3 = 1;
                        break;
                    }
                }
                if (rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling)
                {
                    rows = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                }
                else
                    break;
            }
            addType(te, newType(idvalue, Jagged, NULL, newJagTypeExpression(jarr)));
                        type_node->te= newJagTypeExpression(jarr);

            if (errno1)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level," dim(0) out of bounds in 2D jarr")
            }
            if (errno3)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level,"dim(1) too short for 2D jarr")
            }
            // jarrdec->sibling->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            // jarrdec->sibling->hasType=1;
        }
        else if (!strcmp(curDec->token, "J2D") && strcmp(curDec->child->sibling->token, "ID"))
        {
            int errno1 = 0;
            int errno2 = 0;
            int errno3 = 0;

            treeNode *jarrdec = curDec->child;
            treeNode *idlist = jarrdec->sibling->sibling->sibling->sibling;
            char *lo = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char *hi = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr *jarr = newJagArr(2, lo, hi);
            treeNode *rows = curDec->sibling; //onerow token
            for (int i = 0;; i++)
            {
                if (atoi(rows->child->sibling->sibling->child->token) > atoi(hi)||atoi(rows->child->sibling->sibling->child->token) < atoi(lo))
                {
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                    errno1 = 1;
                }
                char *size = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                treeNode *rowvals = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                populateJagArr(jarr, i, size, 0);

                for (int j = 0; j < atoi(size); j++)
                { //error checking for dim(1)(incomplete)
                    if (rowvals->child->child->sibling)
                    {
                        puts("Error to be reported for 3D rowvals in 2D jagged array.\n");
                        errno2 = 1;
                    }

                    if (rowvals->child->sibling)
                    {
                        rowvals = rowvals->child->sibling->sibling;
                    }
                    else
                    {
                        puts("Error to be reported for lesser than mentioned in dim(1) for 2D jarr,\n");
                        errno3 = 1;
                        break;
                    }
                }
                if (rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling)
                {
                    rows = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                }
                else
                    break;
            }
            char *idvalue;
            //absorbing first id to enter recursive loop of id sublist
            // idlist->child->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            // idlist->child->hasType=1;
            // addType(te,newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
            idlist = idlist->child;
            type_node->te=newJagTypeExpression(jarr);
            while (1)
            {
                idvalue = idlist->child->token;
                // idlist->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
                // idlist->hasType=1;
                addType(te, newType(idvalue, Jagged, NULL, newJagTypeExpression(jarr)));

                if (idlist->sibling)
                {
                    idlist = idlist->sibling->child;
                }
                else
                {
                    break;
                }
            }
            if (errno1)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level," dim(0) out of bounds in 2D jarr")
            }
            if (errno2)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level,"3D rowvals in 2D jagged array")
            }
            if (errno3)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level,"dim(1) too short for 2D jarr")
            }
        }
        else if (!strcmp(curDec->token, "J3D") && strcmp(curDec->child->sibling->token, "ID"))
        {
            int errno1 = 0;
            // int errno2 = 0;
            int errno3 = 0;
            treeNode *jarrdec = curDec->child;
            treeNode *idlist = jarrdec->sibling->sibling->sibling->sibling;
            char *lo = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            char *hi = jarrdec->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
            JagArr *jarr = newJagArr(2, lo, hi);
            treeNode *rows = curDec->sibling; //onerow token
            int subsize = 0;
            treeNode *rowvals;
            treeNode *digitlist;
            for (int i = 0;; i++)
            {
                
                if (atoi(rows->child->sibling->sibling->child->token) > atoi(hi)||atoi(rows->child->sibling->sibling->child->token) < atoi(lo))
                {
                    puts("Error to be reported for dim(0) out of bounds in jarr.\n");
                    errno1 = 1;
                }
                char *size = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->child->token;
                rowvals = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                
                //puts(rowvals->token);
                populateJagArr(jarr, i, size, 1);

                for (int j = 0; j < atoi(size); j++)
                {
                    digitlist = rowvals->child; //STAT_LIST
                    while (strcmp(digitlist->child->token, "EPSILON"))
                    { //traverse subrange for subsize
                        subsize++;
                        digitlist = digitlist->child->sibling;
                    }
                    char *buff = (char *)malloc(sizeof(char) * 5);
                    itoa(subsize, buff);
                    populateJagArrSubrange(jarr, i, j, buff);
                    //puts(rowvals->child->sibling->token);
                    if (rowvals->child->sibling)
                    {
                        rowvals = rowvals->child->sibling->sibling;
                    }
                    else
                    {
                        puts("Error to be reported for lesser than mentioned in dim(1) for 3D jarr,\n");
                        errno3 = 1;
                        break;
                    }
                }
                if (rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling)
                {
                    rows = rows->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
                }
                else
                    break;
            }
            char *idvalue;
            //absorbing first id to enter recursive loop of id sublist
            // idlist->child->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
            // idlist->child->hasType=1;
            // addType(te,newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr)));
            idlist = idlist->child;
            type_node->te=newJagTypeExpression(jarr);
            while (1)
            {
                idvalue = idlist->child->token;
                // idlist->v.type=newType(idvalue,Jagged,NULL,newJagTypeExpression(jarr));
                // idlist->hasType=1;
                addType(te, newType(idvalue, Jagged, NULL, newJagTypeExpression(jarr)));
                
                if (idlist->sibling)
                {
                    idlist = idlist->sibling->child;
                }
                else
                {
                    break;
                }
            }
            if (errno1)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level," dim(0) out of bounds in 2D jarr")
            }
            if (errno3)
            {
                // newerror(rows->child->line,DECLARATION,NULL,NULL,Jagged,type_node->te,NULL,Jagged,type_node->te,curDec->level,"dim(1) too short for 2D jarr")
            }
        }
    }
    curDec=declist;
}
return curDec;
}

Type *searchTET(char *varname, typeExpressionTable *te)
{
    for (int i = 0;; i++)
    {
        Type *curtype = te->arr[i];
        if (!strcmp(curtype->field1, varname))
        {
            return curtype;
        }
    }
    return NULL;
}

// ASSIGNMENT ID = EXPRESSION ;
// ASSIGNMENT ID = BOOLEXPRESSION ;
// ASSIGNMENT ARR_DEF = EXPRESSION ;
// ARR_DEF ID [ STAT_LIST ]
// EXPRESSION SUBEX OPERATOR EXPRESSION
// EXPRESSION SUBEX OPERATOR EXPRESSION
// EXPRESSION SUBEX OPERATOR EXPRESSION
// EXPRESSION SUBEX
// SUBEX STAT_INT
// SUBEX ID
// SUBEX ARR_DEF
// BOOLEXPRESSION ID BOOLOP BOOLEXPRESSION
// BOOLEXPRESSION ID 
// BOOLOP &&&
// BOOLOP |||
// OPERATOR +
// OPERATOR -
// OPERATOR *
// OPERATOR /

teStruct * getTypeExpressionfor(treeNode * node, typeExpressionTable * te){
    
    teStruct* tes= (teStruct *)malloc(sizeof(teStruct));

    printf("nd-tkn= %s\n", node->token);
    if(!(node->child)){
        Type * temp_tp= searchTET(node->token, te);
        tes->te= &temp_tp->field4;
        tes->c= temp_tp->field2;
        return tes;
    }
    else if(!strcmp(node->token,"ARR_DEF") || !strcmp(node->token,"STAT_INT")){
        typeExpression * texp= (typeExpression *)malloc(sizeof(typeExpression));
        *texp= newPrimTypeExpression(getType("integer")); 
        node->hasType= 1;
        node->te= *texp;
        
        tes->te= texp;
        tes->c= Primitive;
        return tes;
    }
    else if(!strcmp(node->token,"ID") || !strcmp(node->token,"SUBEX"))
    {
        node->hasType= 1;
        tes= getTypeExpressionfor(node->child,te);
        node->te= *(tes->te);

        return tes;
    }

    if(!node->child->sibling){
        tes= getTypeExpressionfor(node->child, te);
        node->hasType= 1;
        node->te= *(tes->te);
        return tes;
    }
    node->hasType= 1;
    tes= ComputeValidExpression(node->child, node->child->sibling, node->child->sibling->sibling, te);

    return tes;
}

teStruct * ComputeValidExpression(treeNode * id, treeNode * op, treeNode * expr, typeExpressionTable * tb){
    teStruct * idte=getTypeExpressionfor(id, tb);
    teStruct * expte=getTypeExpressionfor(expr, tb);

    char * te;
    char * exp;

    if(idte->c== Primitive){
        te= printPrimType(idte->te->primType);
    }
    else if(idte->c== Rectangular){
        te= printRectArr(idte->te->rectangular);
    }
    else if(idte->c== Jagged){
        te= printJagArr(idte->te->jagged);
    }

    if(expte->c== Primitive){
        exp= printPrimType(expte->te->primType);
    }
    else if(expte->c== Rectangular){
        exp= printRectArr(expte->te->rectangular);
    }
    else if(expte->c== Jagged){
        exp= printJagArr(expte->te->jagged);
    }

    teStruct * ts= (teStruct *)malloc(sizeof(teStruct));

    if(!strcmp(te, exp)){
        if(!strcmp(op->child->token,"/") && idte->c== Primitive){
            op->hasType= 1;
            op->te= newPrimTypeExpression(getType("real"));
            ts->c= Primitive;
            ts->te= &(op->te);
        }
        else if(strcmp(op->child->token,"/")){
            op->hasType= 1;
            op->te= *(idte->te);   
            ts->c= idte->c;
            ts->te= idte->te;      
        }
        return ts;
    }
    return NULL;
}

void traverseAssignmentTree(treeNode * root, typeExpressionTable * te){
    treeNode * curAss= root;
    for(int i=0; i<assign_count;i++){

        treeNode * tmp_tn= curAss->child;
        Type * lhs;

        if(!strcmp(tmp_tn->token, "ID")){
            lhs= searchTET(tmp_tn->child->token, te);            
        }
        if(!strcmp(tmp_tn->token, "ARR_DEF")){
            lhs= (Type *)malloc(sizeof(Type));
            lhs->field4= newPrimTypeExpression(getType("integer"));
        }
        curAss->child->hasType= 1;
        curAss->child->te= lhs->field4;
        teStruct * rhs= getTypeExpressionfor(curAss->child->sibling->sibling, te);
        curAss->child->sibling->sibling->hasType= 1;
        curAss->child->sibling->sibling->te= *(rhs->te);
        curAss= curAss->sibling;
    }
    printf("Traversing successful\n");

}
