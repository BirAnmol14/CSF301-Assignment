all:
	gcc -o exec -g codeFile.c dataStructures.c functions.c parseTreeFunctions.c
	./exec