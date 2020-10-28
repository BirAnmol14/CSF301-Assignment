#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char * a= (char *) malloc(sizeof(char)*20);
    strcpy(a, "JARR_EXPRESSION");
    mapNode* n= search(a);
    printf("%d",n->value);
}