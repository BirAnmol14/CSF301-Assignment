#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
grammar g;
tokenStream * ts=NULL;
typeExpressionTable * tab=NULL;
void printOptions();
void testOption(int);
void option1();
void option2();
void option3();
void option4();
int main() {
	//Driver Function
	int option;
	do{
		printOptions();
		scanf("%d",&option);
		testOption(option);
	}while(option!=0);
	puts("Thank You");
	return 0;
}
void printOptions(){
	puts("");
	puts("0. Exit");
	puts("1. Create Parse Tree");
	puts("2. Traverse Parse Tree and Create Type Expression Table");
	puts("3. Print Parse Tree");
	puts("4. Print Type Expression Table");
	printf("Kindly Choose an option: ");
}
void testOption(int option){
	switch(option){
		case 0: break;
		case 1: option1();break;
		case 2: option2();break;
		case 3: option3();break;
		case 4: option4();break;
		default: puts("Kindly Choose a valid option");break;
	}
}
void option1(){
	puts("\nOption 1\n");
	readGrammar("grammarTest.txt",&g);
	printGrammar(&g);
	if(ts==NULL){
		ts=newTokenStream();
	}
	puts("\nTokenizing Source Code");
	tokeniseSourcecode("test2.txt",ts);
	printTokenStream(ts);
}
void option2(){
	option1();
	puts("option 2");
	puts("Creating Type Expression Table");
	tab=newTable();
	addType(tab,newType("abc",Primitive,NULL,newTypeExpression("<type=integer>",Primitive)));
	addType(tab,newType("abc3",Rectangular,"static",newTypeExpression("<type=rectangularArray,dimension=1,range_R1=(3,10),basicElementType=integer>",Rectangular)));
	addType(tab,newType("abc2",Rectangular,"dynamic",newTypeExpression("<type=rectangularArray,dimensions=2,range_R1=(3,6),rangeR2=(15,34),basicElementype=integer>",Jagged)));
	addType(tab,newType("abc1",Jagged,NULL,newTypeExpression("<type=jaggedArray,dimensions=2,range_R1=(4,7),rangeR2=(2,4,10,5),basicElementType=integer>",Rectangular)));
}
void option3(){
	option2();
	puts("option 3");	
}
void option4(){
	option3();
	puts("option 4");
	puts("Printing Type Expression Table");
	printTypeExpressionTable(tab);
}
