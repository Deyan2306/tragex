#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
const int BUFF = 100;

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

int main(void) {
	
	// test print
	printf("Yup, we runninga!\n");

	StackNode * test = initStackNode(5, "abc");

	printf("The value of the variable is: %d\n", test->value);
	printf("The variable name is: %s\n", test->variable_name);

	// deallocate, plaese xd
	free(test->variable_name);
	free(test);
	return 0;
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
