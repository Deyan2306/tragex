#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE 100
#define MAX_TOKEN_SIZE 1024
const int BUFF = 100;

/*
We encountered a rock bottom...
the lexor is written badly, which makes it not extendable at all. This kind of sucks, really.
So how do we fix this?
    1. I should replace all string checks, that use `==` with strcmp (the actual way of comparing strings).
    2. I should probably rewrite the whole tokenizing function - it shouldn't take as long, i mean, i have already written it
    3. I should probably extract the tokenizing function into a separate one, so the file-reading works

Good luck with that.
*/

typedef struct {
    int value;
    char * variable_name;
} StackNode;

typedef struct {
    StackNode * operations[MAX_SIZE];
    int top;
} Stack;

typedef struct {
    int type;
    char * value;
} Token;

enum TokenType {
    ADDX = 1, // adds a variable
    RMX = 2, // removes a variable
    SHOWX = 3, // console logging
    EXECF = 4, // function declaration
    MA = 5, // main function declaration
    VARIABLE = 6, // Variable name
    VALUE = 7, // Value of a variable
    SPLIT = 8, // --
    COLON = 9, // :
    SQ_BRACES = 10, // [ ]
    TR_BRACES = 11, // < >
    RD_BRACES = 12, // ( )
    SEMI_COLON = 13, // ;
    SUMX = 14, // Sum of two numbers
    SUBX = 15 // Subtraction of two numbers
};

StackNode * initStackNode(int _value, char * _variable_name);
StackNode * RIC();

void initialize(Stack * stack);
int isFull(Stack * stack);
int isEmpty(Stack * stack);
int push(Stack * stack, StackNode * value);
StackNode * pop(Stack * stack);
StackNode * peek(Stack * stack);

Token * initToken(int _type, char * _value);

char * getTokenStringRepresentation(Token * token);
Token * tokenizeProgram(char * fileLocation, int size, int * numberOfTokens);

bool extensionIsNotCorrect(char * fileName, int size);
char * trim(char * str);
bool isNum(char * stringRepresentation);

void runInterpreted();

int main(int argc, char **argv) {

    // TODO: Make a command line interpreter, that tokenizes
    // WAGAHAAAAAA: use strcmp to compare strings and it may work

    if (argc != 2) {
        fprintf(stderr, "Usage: tragex <filename.trgx> -[options]\n");
        return 1;
    }

    if (strcmp(argv[1], "-i") == 0) {
        runInterpreted();  
        return 1;
    }

    int numberOfTokens = 0;
    Token * tokens = tokenizeProgram(argv[1], strlen(argv[1]), &numberOfTokens);

    for (int i = 0; i < numberOfTokens; i++)
        printf("%s\n", getTokenStringRepresentation(&tokens[i]));

    for (int i = 0; i < numberOfTokens; i++)
        free(tokens[i].value);

    free(tokens);
    return 0;
}

void runInterpreted() {
    char * command;
    printf("> ");
    scanf("%s", command);
    while (strcmp(command, "exit") != 0) {

        int numberOfTokens = 0;
        Token * tokens = tokenizeProgram(command, strlen(command), &numberOfTokens);

        for (int i = 0; i < numberOfTokens; i++)
            printf("%s\n", getTokenStringRepresentation(&tokens[i])); // TODO: Thi tokenizes a whole file, not just a string...

        for (int i = 0; i < numberOfTokens; i++)
            free(tokens[i].value);

        free(tokens);
        
        printf("> ");
        scanf("%s", command); 
    }
    return;
}

// ==== Lexer methods ====

Token * tokenizeProgram(char * fileLocation, int size, int * numberOfTokens) {

    FILE *fptr = fopen(fileLocation, "r");
    char readBuffer[1024];
    int count = 0;

    Token ** tokenHolder = malloc(sizeof(Token *) * MAX_TOKEN_SIZE); // Do not forget to deallocate
    
    if (tokenHolder == NULL) {
        fprintf(stderr, "[-] Unable to allocate memory for the Token Holder!\n");
        exit(1);
    }

    if (fptr == NULL || extensionIsNotCorrect(fileLocation, size)) {
        fprintf(stderr, "[-] Unable to open the provided file!\n");
        exit(1);
    }

    int state = 0; // 0 for outside of token; 1 for inside of token
    char currentToken[MAX_TOKEN_SIZE];
    int tokenIndex = 0;
	char currentChar;

    Token * currentTokenPtr = NULL;

    // TODO: extract this to a function
    while((currentChar = fgetc(fptr)) != EOF) {
        if (state == 0) { // Outside token
            if (currentChar == ' ') {
                continue; // Skip space
            } else {
                state = 1; // New token starts
                tokenIndex = 0;
                currentToken[tokenIndex++] = currentChar;
            }
        } else if (state == 1) { // Inside a token
            if (currentChar == ' ' || currentChar == '\0'
                || currentChar == '\n'
                || currentChar == '\r' 
                || currentChar == '\t' 
                || currentChar == ':' || currentChar == ';'
                || currentChar == '[' || currentChar == ']'
                || currentChar == '<' || currentChar == '>'
                || currentChar == '(' || currentChar == ')') {
                
                // End of token
                currentToken[tokenIndex] = '\0'; // Terminate the string
				char * trimmedToken = trim(currentToken);

                if (trimmedToken[0] == '(') {
                    currentTokenPtr = initToken(RD_BRACES, "(");
                    tokenHolder[count++] = currentTokenPtr;
                    trimmedToken++; // Moves the pointer 1 bit ahead, so we cover the braces
                } else if (trimmedToken[0] == '<') {
                    currentTokenPtr = initToken(TR_BRACES, "<");
                    tokenHolder[count++] = currentTokenPtr;
                    trimmedToken++;
                } else if (trimmedToken[0] == '[') {
                    currentTokenPtr = initToken(SQ_BRACES, "[");
                    tokenHolder[count++] = currentTokenPtr;
                    trimmedToken++;
                }

                // XXX

                if (strcmp(currentToken, "addx") == 0) {
                    currentTokenPtr = initToken(ADDX, trimmedToken);
                } else if (strcmp(currentToken, "rmx") == 0) {
                    currentTokenPtr = initToken(RMX, trimmedToken);
                } else if (strcmp(currentToken, "showx") == 0) {
                    currentTokenPtr = initToken(SHOWX, trimmedToken);
                } else if (strcmp(currentToken, "execf") == 0) {
                    currentTokenPtr = initToken(EXECF, trimmedToken);
                } else if (strcmp(currentToken, "ma") == 0) {
                    currentTokenPtr = initToken(MA, trimmedToken);
                } else {
					if ((int)trimmedToken[0] != 0) { // Sometimes it recognizes "" as a token; NOTE: It recognizes the NULL character, so we need to skip it
                        // Check for variable of a number
                        if (isNum(trimmedToken)) {
                            currentTokenPtr = initToken(VALUE, trimmedToken);
                        } else {
                            currentTokenPtr = initToken(VARIABLE, trimmedToken);
                        }
                    }
                }

                tokenHolder[count++] = currentTokenPtr;
                state = 0;
            } else {
                currentToken[tokenIndex++] = currentChar;
            }

            // BUG: Some tokens are recognized in the start of the sentance, so we get results like this "(test"

            // Add conditions for '<' and '>'
            if (currentChar == ':') {
                currentTokenPtr = initToken(COLON, ":");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == ';') {
                currentTokenPtr = initToken(SEMI_COLON, ";");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == '[') { // Braces
                currentTokenPtr = initToken(SQ_BRACES, "[");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == ']') {
                currentTokenPtr = initToken(SQ_BRACES, "]");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == '<') {
                currentTokenPtr = initToken(TR_BRACES, "<");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == '>') {
                currentTokenPtr = initToken(TR_BRACES, ">");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == '(') {
                currentTokenPtr = initToken(RD_BRACES, "(");
                tokenHolder[count++] = currentTokenPtr;
            } else if (currentChar == ')') {
                currentTokenPtr = initToken(RD_BRACES, ")");
                tokenHolder[count++] = currentTokenPtr;
            }
        }

    }

    fclose(fptr);
    *numberOfTokens = count;

    // Create a new array of the exact size needed
    Token * finalTokens = malloc(sizeof(Token) * count);
    for (int i = 0; i < count; i++) {
        finalTokens[i] = *tokenHolder[i];
    }

    free(tokenHolder); // Free the original array
    free(currentTokenPtr);

    return finalTokens;
}

Token * initToken(int _type, char * _value) {

    Token * createdToken = malloc(sizeof(Token));
    
    if (createdToken == NULL) {
        fprintf(stderr, "[-] The token was not initialized successfully!\n");
        exit(1);
    }

    createdToken->type = _type;
    createdToken->value = malloc(strlen(_value) + 1);

    if (createdToken->value == NULL) {
        fprintf(stderr, "[-] The token's value was not initialized successfully.");
        exit(1);
    }

    strcpy(createdToken->value, _value);

    return createdToken;
}

char * getTokenStringRepresentation(Token * token) {

    char * result = malloc(sizeof(char) * (strlen(token->value) + 20)); // Adding extra for type

    if (result == NULL) {
        fprintf(stderr, "[-] Failed to allocate memory for the tokenization!\n");
        exit(1);
    }

    sprintf(result, "(\"%s\" : %d)", token->value, token->type);

    return result;
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

// ==== Main functions ====

char * trim(char * str) {
	while (isspace((unsigned char)*str)) {
		str++;
	}

	if (*str == '\0')
		return str;

	char *end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) {
		end--;
	}

	*(end + 1) = '\0';

	return str;
}

bool extensionIsNotCorrect(char * fileName, int size) {
	char check[] = { 't', 'r', 'g', 'x' };

	int i;
	int j;

	for (i = 0, j = size - 4; j < size; i++, j++)
		if (check[i] != fileName[j - 1])
				return false;

	return true;
}

bool isNum(char * stringRepresentation) {
    int size = strlen(stringRepresentation);
    for (int i = 0; i < size; i++)
        if ((int) stringRepresentation[i] < 48 || (int) stringRepresentation[i] > 57)
            if (i == 0 && stringRepresentation[i] == '-')
                continue;
            else
                return false;
    return true;
}
