/* 
 Group number- 40

 Group Members:-

 		NAME 						ID
 1. Bir Anmol Singh 		2018A7PS0261P
 2. Gitansh Pujari 			2018A7PS0163P
 3. Rahul Barigidad 		2018A7PS0220P
 4. Tejas Tiwari 			2018A7PS0280P
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "parseTreeFunctions.h"
grammar g;
tokenStream *ts = NULL;
typeExpressionTable *tab = NULL;
parseTree *root;
int flag1, flag2, flag3;
char * filename;
void printOptions();
void testOption(int);
void option1();
void option2();
void option3();
void option4();
int main(int * argc, char * argv[])
{
	flag1 = 0;
	flag2 = 0;
	flag3 = 0;
	//Driver Function
	int option;
	filename= argv[1];
	do
	{
		printOptions();
		scanf("%d", &option);
		testOption(option);
	} while (option != 0);
	puts("Thank You");
	return 0;
}
void printOptions()
{
	puts("");
	puts("0. Exit");
	puts("1. Create Parse Tree");
	puts("2. Traverse Parse Tree and Create Type Expression Table");
	puts("3. Print Parse Tree");
	puts("4. Print Type Expression Table");
	printf("Kindly Choose an option: ");
}
void testOption(int option)
{
	switch (option)
	{
	case 0:
		break;
	case 1:
		option1();
		break;
	case 2:
		option2();
		break;
	case 3:
		option3();
		break;
	case 4:
		option4();
		break;
	default:
		puts("Kindly Choose a valid option");
		break;
	}
}
void option1()
{
	if (!flag1)
	{
		puts("\nOption 1\n");
		readGrammar("grammar.txt", &g);

		// printGrammar(&g);

		// if(ts==NULL){
		ts = newTokenStream();
		// }

		puts("\nTokenizing Source Code");
		tokeniseSourcecode(filename, ts);
		// printTokenStream(ts);

		root = newTree();
		if (createParseTree(root, ts, g))
			printf("\nParse Tree created successfully\n");
		else
			printf("\nTree creation failed\n");
	}
	else
	{
		printf("\nParse Tree created successfully\n");
	}
	printParseTree(root);
	flag1 = 1;
}
void option2()
{
	if (!flag2)
	{
		option1();
		puts("\nOption 2\n");
		puts("Traversing Parse Tree and Creating Type Expression Table");
		tab = newTable();

		traverseParseTree(root, tab);
	}
	else
	{
		puts("\nOption 2\n");
		puts("Traversing Parse Tree and Creating Type Expression Table");
	}
	flag2 = 1;
}
void option3()
{

	if (!flag3)
	{

		option2();
		puts("\nOption 3- \n Printing level order tree: \n\n\n");
		printf("%-25s%-20s%-15s%-12s%10s%10s\n\n\n", "Token", "isTerminal?", "typeExpression", "Lexeme", "Line no", "Depth");
		printParseTree(root);
	}
	else
	{
		puts("\nOption 3- \n Printing level order tree: \n\n\n");
		printf("%-25s%-20s%-20s%-12s%10s%10s\n\n\n", "Token", "isTerminal?", "typeExpression", "Lexeme", "Line no", "Depth");
		printParseTree(root);
	}

	flag3 = 1;
}
void option4()
{
	option3();
	puts("\nOption 4- \n ");
	puts("Printing Type Expression Table \n\n\n");
	printTypeExpressionTable(tab);
}
