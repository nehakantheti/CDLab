// C program to illustrate the implementation of lexical analyser

#include <ctype.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAX_LENGTH 100 

int count = 0;
// Function to check for a delimiter
bool isDelimiter(char chr) 
{ 
	return (chr == ' ' || chr == '+' || chr == '-' || chr == '*'
			|| chr == '/' || chr == ',' || chr == ';' || chr == '%'
			|| chr == '>' || chr == '<' || chr == '=' || chr == '('
			|| chr == ')' || chr == '[' || chr == ']' || chr == '{'
			|| chr == '}'); 
} 

// Function to check for an operator
bool isOperator(char chr) 
{ 
	return (chr == '+' || chr == '-' || chr == '*' || chr == '/'
			|| chr == '>' || chr == '<' || chr == '=' || chr == ',' || chr == ';' || chr == '(' || chr == ')' ); 
} 

// Function to check for a valid identifier
bool isValidIdentifier(char* str) 
{ 
	return (str[0] != '0' && str[0] != '1' && str[0] != '2'
			&& str[0] != '3' && str[0] != '4' && str[0] != '5'
			&& str[0] != '6' && str[0] != '7' && str[0] != '8'
			&& str[0] != '9' && !isDelimiter(str[0])); 
} 

// Function to check for a keyword
bool isKeyword(char* str) 
{ 
	const char* keywords[] = { "auto", "break", "case", "char", 
								"const", "continue", "default", "do", 
								"double", "else", "enum", "extern", 
								"float", "for", "goto", "if", 
								"int", "long", "register", "return", 
								"short", "signed", "sizeof", "static", 
								"struct", "switch", "typedef", "union", 
								"unsigned", "void", "volatile", "while" }; 
	for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) { 
		if (strcmp(str, keywords[i]) == 0) { 
			return true; 
		} 
	} 
	return false; 
} 

// Function to check for an integer value
bool isInteger(char* str) 
{ 
	if (str == NULL || *str == '\0') { 
		return false; 
	} 
	int i = 0; 
	while (isdigit(str[i])) { 
		i++; 
	} 
	return str[i] == '\0'; 
} 

// Function to trim a substring from a given string's start and end position
char* getSubstring(char* str, int start, int end) 
{ 
	int subLength = end - start + 1; 
	char* subStr = (char*)malloc((subLength + 1) * sizeof(char)); 
	strncpy(subStr, str + start, subLength); 
	subStr[subLength] = '\0'; 
	return subStr; 
} 

// Function to parse the input
int lexicalAnalyzer(char* input) 
{ 
	int left = 0, right = 0; 
	int len = strlen(input); 

	while (right <= len && left <= right) { 
		if (!isDelimiter(input[right])) 
			right++; 

		if (isDelimiter(input[right]) && left == right) { 
			if (isOperator(input[right])) 
			{
			    count++;
			    printf("Token: Operator, Value: %c\n", input[right]); 
			}
			right++; 
			left = right; 
		} 
		else if (isDelimiter(input[right]) && left != right 
				|| (right == len && left != right)) { 
			char* subStr = getSubstring(input, left, right - 1); 

			if (isKeyword(subStr)) 
			{
			    count++;
				printf("Token: Keyword, Value: %s\n", subStr); 
			}
			else if (isInteger(subStr))
			{
			    count++;
				printf("Token: Integer, Value: %s\n", subStr); 
			}
			else if (isValidIdentifier(subStr) && !isDelimiter(input[right - 1]))
			{
			    count++;
				printf("Token: Identifier, Value: %s\n", subStr); 
			}
			else 
			{
			    count++;
				printf("Token: Unidentified, Value: %s\n", subStr); 
			}
			left = right; 
		} 
	} 
	return 0; 
} 

// Main function
int main() 
{ 
	char lex_input[MAX_LENGTH]; 
	printf("Enter an expression: "); 
	fgets(lex_input, MAX_LENGTH, stdin); 
	lex_input[strcspn(lex_input, "\n")] = '\0';  // Remove newline character
	printf("For Expression \"%s\":\n", lex_input); 
	lexicalAnalyzer(lex_input); 
	
	printf("No of tokens: %d", count);

	return 0; 
}