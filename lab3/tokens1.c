#include <ctype.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAX_LENGTH 100 

int count = 0;

bool isDelimiter(char chr) 
{ 
    return (chr == ' ' || chr == '+' || chr == '-' || chr == '*'
            || chr == '/' || chr == ',' || chr == ';' || chr == '%'
            || chr == '>' || chr == '<' || chr == '=' || chr == '('
            || chr == ')' || chr == '[' || chr == ']' || chr == '{'
            || chr == '}'); 
} 

bool isOperator(char* str) 
{ 
    return (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 || strcmp(str, "*") == 0 
            || strcmp(str, "/") == 0 || strcmp(str, ">") == 0 || strcmp(str, "<") == 0 
            || strcmp(str, "=") == 0 || strcmp(str, ",") == 0 || strcmp(str, ";") == 0 
            || strcmp(str, "(") == 0 || strcmp(str, ")") == 0 || strcmp(str, "{") == 0 
            || strcmp(str, "}") == 0 || strcmp(str, "++") == 0 || strcmp(str, "--") == 0); 
} 

bool isValidIdentifier(char* str) 
{ 
    return (str[0] != '0' && str[0] != '1' && str[0] != '2'
            && str[0] != '3' && str[0] != '4' && str[0] != '5'
            && str[0] != '6' && str[0] != '7' && str[0] != '8'
            && str[0] != '9' && !isDelimiter(str[0])); 
} 

bool isKeyword(char* str) 
{ 
    const char* keywords[] = {  "break", "case", "char", 
                                "const", "continue", "default", "do", 
                                "double", "else", "enum", 
                                "float", "for", "if", 
                                "int", "long", "register", "return", 
                                "short", "signed", "sizeof", "static", 
                                "struct", "switch", "typedef", "union", 
                                "unsigned", "void", "while" }; 
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) { 
        if (strcmp(str, keywords[i]) == 0) { 
            return true; 
        } 
    } 
    return false; 
} 

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

char* getSubstring(char* str, int start, int end) 
{ 
    int subLength = end - start + 1; 
    char* subStr = (char*)malloc((subLength + 1) * sizeof(char)); 
    strncpy(subStr, str + start, subLength); 
    subStr[subLength] = '\0'; 
    return subStr; 
} 

int lexicalAnalyzer(char* input) 
{ 
    int left = 0, right = 0; 
    int len = strlen(input); 

    while (right <= len && left <= right) { 
        if (input[right] == '/' && input[right + 1] == '/') {
            while (input[right] != '\n' && input[right] != '\0') {
                right++;
            }
            left = right;
        }
        else if (input[right] == '/' && input[right + 1] == '*') {
            right += 2;
            while (!(input[right] == '*' && input[right + 1] == '/') && input[right] != '\0') {
                right++;
            }
            right += 2;
            left = right;
        }
        else if (input[right] == '#') {
            while (input[right] != '\n' && input[right+1] != '\0') {
                right++;
            }
            left = right;
        }
        else if (input[right] == '"') {
            right++;
            while (input[right] != '"' && input[right+1] != '\0') {
                right++;
            }
            right++;

            char* subStr = getSubstring(input, left, right - 1);
            count++;
            printf("Token: String, Value: %s\n", subStr);
            free(subStr);

            left = right;
        } else if (!isDelimiter(input[right])) {    //traverses through the loop with this
            right++; 
        }

        else if ((input[right] == '+' && input[right + 1] == '+') || (input[right] == '-' && input[right + 1] == '-')) {
            char operatorStr[3];
            operatorStr[0] = input[right];
            operatorStr[1] = input[right + 1];
            operatorStr[2] = '\0';
            
            count++;
            printf("Token: Operator, Value: %s\n", operatorStr);
            
            right += 2;
            left = right;
        }

        if (isDelimiter(input[right]) && left == right) { 
            if (isOperator((char[]){input[right], '\0'})) 
            {
                count++;
                printf("Token: Operator, Value: %c\n", input[right]); 
            }
            right++; 
            left = right; 
        } 
        else if ((isDelimiter(input[right]) && left != right) 
                || (right == len && left != right)) { 
            char* subStr = getSubstring(input, left, right - 1); 
            
            if (strlen(subStr) > 0) {
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
            }

            free(subStr);
            left = right; 
        }
    } 
    return 0; 
} 

int main() 
{ 
    char lex_input[MAX_LENGTH]; 
    FILE *file = fopen("random.txt", "r");
    
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    while (fgets(lex_input, MAX_LENGTH, file) != NULL) {
        lex_input[strcspn(lex_input, "\n")] = '\0';
        lexicalAnalyzer(lex_input); 
    }

    printf("\nTotal number of tokens: %d\n", count);

    fclose(file);
    return 0; 
}
