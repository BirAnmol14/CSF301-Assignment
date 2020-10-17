#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
grammar g;
tokenStream * ts=NULL;
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
	newGrammar(&g);
	if(ts==NULL){
		ts=newTokenStream();
	}
	tokeniseSourcecode("abc.txt",ts);
	printTokenStream(ts);
	puts("option 1");
}
void option2(){
	option1();
	puts("option 2");
}
void option3(){
	option2();
	puts("option 3");
}
void option4(){
	option3();
	puts("option 4");
}
