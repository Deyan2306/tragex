#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFF = 100;

// Overcomplicating everything by reimplementing a stack xd
struct stackNode {
	int value;
	char * variable_name;

	struct stackNode * prev;
	struct stackNode * next;
};

// Doing this to simplify writing "struct" way too many times
typedef struct stackNode StackNode;

StackNode * initStackNode(int _value, char * _variable_name) {
	StackNode* initialized = malloc(sizeof(StackNode));

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

	initialized->prev = NULL;
	initialized->next = NULL;

	return initialized;
}

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
