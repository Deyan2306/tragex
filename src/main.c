#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 100
const int BUFF = 100;

// note from the author: remember to free() any space after using push or initStackNode :]

typedef struct {
	int value;
	char * variable_name;
} StackNode;

StackNode * initStackNode(int _value, char * _varuable_name);
StackNode * RIC();

typedef struct {
	StackNode * operations[MAX_SIZE];
	int top;
} Stack;

void initialize (Stack * stack);
int isFull (Stack * stack);
int isEmpty (Stack * stack);
int push (Stack * stack, StackNode * value);
StackNode * pop (Stack * stack);
StackNode * peek (Stack * stack);

bool extensionIsNotCorrect(char * fileName, int size);

int main(int argc, char **argv) {

	// What next?
	// Probably lex the program, idk (how to lex, bro)
	//

	FILE *fptr = fopen(argv[1], "r");
	char readBuffer[1024]; // Buffer to store the read file

	if (fptr == NULL || extensionIsNotCorrect(argv[1], strlen(argv[1]))) {
		fprintf(stderr, "[-] Unable to open the provided file!\n");
		exit(1);
	}

	printf("[+] file to parse is: %s\n\n", argv[1]);
	
	while(fgets(readBuffer, sizeof(readBuffer), fptr) != NULL) {
		printf("%s", readBuffer);
	}


	// printing the file for no reason at all

	// StackNode * test = initStackNode(5, "abc");

	// printf("The value of the variable is: %d\n", test->value);
	// printf("The variable name is: %s\n", test->variable_name);

	// deallocate, plaese xd
	// free(test->variable_name);
	// free(test);

	fclose(fptr);

	return 0;
}

// ==== Main functions ====
bool extensionIsNotCorrect(char * fileName, int size) {
	char check[] = { 't', 'r', 'g', 'x' };

	int i;
	int j;

	for (i = 0, j = size - 4; j < size; i++, j++)
		if (check[i] != fileName[j - 1])
				return false;

	return true;
}

// ==== Stack Node methods ====
StackNode * initStackNode(int _value, char * _variable_name) {
	StackNode * initialized = malloc(sizeof(StackNode));

	if (initialized == NULL) {
		fprintf(stderr, "[!] Memmory allocation failed! -- new stack node\n");
		exit(1);
	}

	initialized->value = _value;
	initialized->variable_name = malloc(BUFF * sizeof(char));

	if (initialized->variable_name == NULL) {
		fprintf(stderr, "[!] Memmory allocation failed! -- variable_name\n");
		exit(1);
	}

	strcpy(initialized->variable_name, _variable_name);

	return initialized;
}

StackNode *  RIC() {
	StackNode * invalidCode = malloc(sizeof(StackNode));
	invalidCode->value = -1;
	invalidCode->variable_name = "RIC";
	return invalidCode;	
}

// ==== Stack Methods ====
void initialize(Stack * stack) {
	stack->top = -1;
}

int isFull(Stack * stack) {
	return (stack->top == MAX_SIZE - 1);
}

int isEmpty(Stack * stack) {
	return (stack->top == -1);
}

int push(Stack * stack, StackNode * value) {
	if (isFull(stack)) {
		fprintf(stderr, "[!] The stack is full!\n");
		return -1; // Error code if the operation was unsucessfull
	}
	stack->top++;
	stack->operations[stack->top] = value;
	return 1; // The operation was successful
}

StackNode * pop(Stack * stack) {
	if (isEmpty(stack)) {
		fprintf(stderr, "[!] The stack is empty, there is nothing to pop!\n");
		return RIC(); // Error code
	}
	StackNode * popped = stack->operations[stack->top];
	stack->top--;
	return popped;
}

StackNode * peek(Stack * stack) {
	if (isEmpty(stack)) {
		fprintf(stderr, "[!] The stack is empty! There is nothing to see here.\n");
		return RIC(); // Error code
	}
	StackNode * peeked = stack->operations[stack->top];

	return peeked;
}

// TODO: free alocated space
