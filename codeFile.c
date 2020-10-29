#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "parseTreeFunctions.h"
grammar g;
tokenStream * ts=NULL;
typeExpressionTable * tab=NULL;
parseTree * root;
int flag1, flag2, flag3;
void printOptions();
void testOption(int);
void option1();
void option2();
void option3();
void option4();
int main() {
	flag1= 0;
	flag2= 0;
	flag3= 0;
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
	if(!flag1)
	{
	puts("\nOption 1\n");
	readGrammar("grammar2.txt",&g);
	
	// printGrammar(&g);
	
	// if(ts==NULL){
		ts=newTokenStream();
	// }

	puts("\nTokenizing Source Code");
	tokeniseSourcecode("sampleprogram.txt",ts);
	// printTokenStream(ts);


	root= newTree();
	if(createParseTree(root , ts, g))
		printf("\nParse Tree created successfully\n");
	else
		printf("\nTree creation failed\n");
	}
	else{
		printf("\nParse Tree created successfully\n");	
	}
	
	flag1= 1;
}
void option2(){
	if(!flag2){
	option1();
	puts("option 2");
	puts("Traversing Parse Tree and Creating Type Expression Table");
	tab=newTable();

	//Populating Primitive variables
	addType(tab,newType("abc",Primitive,NULL,newPrimTypeExpression(getType("integer"))));
	addType(tab,newType("abc1",Primitive,NULL,newPrimTypeExpression(getType("real"))));
	addType(tab,newType("abc2",Primitive,NULL,newPrimTypeExpression(getType("boolean"))));

	//Populating Rectangular array
	//1D
	RectArr * rarr=newRectArr(1);
	populateRectArr(rarr,0,"3","7");
	addType(tab,newType("abc3",Rectangular,"static",newRectTypeExpression(rarr)));
	//2D - dynamic as you add variable names
	rarr=newRectArr(2);
	populateRectArr(rarr,0,"abc","7");
	populateRectArr(rarr,1,"25","_xvar");
	addType(tab,newType("abc4",Rectangular,"dynamic",newRectTypeExpression(rarr)));
	//3D
	rarr=newRectArr(3);
	populateRectArr(rarr,0,"1","7");
	populateRectArr(rarr,1,"25","31");
	populateRectArr(rarr,2,"100","1011");
	addType(tab,newType("abc6",Rectangular,"static",newRectTypeExpression(rarr)));

	//2D Jagged Example from sample in assignment given under type expression heading
	JagArr* jarr=newJagArr(2,"3","8");
	populateJagArr(jarr,0,"3",0);
	populateJagArr(jarr,1,"6",0);
	populateJagArr(jarr,2,"2",0);
	populateJagArr(jarr,3,"4",0);
	populateJagArr(jarr,4,"1",0);
	populateJagArr(jarr,5,"5",0);
	addType(tab,newType("abc7",Jagged,NULL,newJagTypeExpression(jarr)));

	//3D Jagged Example from sample in assignment given under type expression heading
	jarr=newJagArr(3,"4","7");
	populateJagArr(jarr,0,"3",1);
	populateJagArrSubrange(jarr,0,0,"5");
	populateJagArrSubrange(jarr,0,1,"3");
	populateJagArrSubrange(jarr,0,2,"5");
	populateJagArr(jarr,1,"2",1);
	populateJagArrSubrange(jarr,1,0,"3");
	populateJagArrSubrange(jarr,1,1,"5");
	populateJagArr(jarr,2,"3",1);
	populateJagArrSubrange(jarr,2,0,"5");
	populateJagArrSubrange(jarr,2,1,"4");
	populateJagArrSubrange(jarr,2,2,"3");
	populateJagArr(jarr,3,"4",1);
	populateJagArrSubrange(jarr,3,0,"2");
	populateJagArrSubrange(jarr,3,1,"5");
	populateJagArrSubrange(jarr,3,2,"4");
	populateJagArrSubrange(jarr,3,3,"4");
	addType(tab,newType("abc8",Jagged,NULL,newJagTypeExpression(jarr)));
	}
	else{
		puts("option 2");
	puts("Traversing Parse Tree and Creating Type Expression Table");
	}
	flag2= 1;
}
void option3(){

	if(!flag3){

	option2();
	puts("option 3- \n Printing level order tree: \n\n\n");
	printf("%-10s%-25s%5s", "Level", "Token", "isTerminal\n\n");
	printParseTree(root);
	}
	else{
	puts("option 3- \n Printing level order tree: \n\n\n");
	printf("%-10s%-25s%5s", "Level", "Token", "isTerminal\n\n");
	printParseTree(root);		
	}

	flag3= 1;
}
void option4(){
	option3();
	puts("option 4- \n ");
	puts("Printing Type Expression Table \n\n\n");
	printTypeExpressionTable(tab);
}
