#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global variables
int lookahead = 0;
char input[100];

// Function prototypes for non-terminal parsing
void S();
void E();
void Eprime();
void T();
void Tprime();
void F();
void V();
void N();
void D();

// Function to parse S → E
void S() {
    E();
}

// E → T E'
void E() {
    T();
    Eprime();
}

// E' → +TE' | -TE' | #
void Eprime() {
    if (input[lookahead] == '+') {
        lookahead++;  // Match '+'
        T();
        Eprime();
    } else if (input[lookahead] == '-') {
        lookahead++;  // Match '-'
        T();
        Eprime();
    }
}

// T → F T'
void T() {
    F();
    Tprime();
}

// T' → *FT' | /FT' | #
void Tprime() {
    if (input[lookahead] == '*') {
        lookahead++;  // Match '*'
        F();
        Tprime();
    } else if (input[lookahead] == '/') {
        lookahead++;  // Match '/'
        F();
        Tprime();
    }
}

// F → (E) | -F | +F | N | V
void F() {
    if (input[lookahead] == '(') {
        lookahead++;  // Match '('
        E();
        if (input[lookahead] == ')') {
            lookahead++;  // Match ')'
        } else {
            printf("Rejected\n");  // Missing closing parenthesis
            exit(1);
        }
    } else if (input[lookahead] == '-') {
        lookahead++;  // Match '-'
        F();
    } else if (input[lookahead] == '+') {
        lookahead++;  // Match '+'
        F();
    } else if (isdigit(input[lookahead])) {
        N();
    } else if (input[lookahead] == 'x' || input[lookahead] == 'y' || input[lookahead] == 'z') {
        V();
    } else {
        printf("Rejected\n");  // Invalid factor
        exit(1);
    }
}

// V → x | y | z
void V() {
    if (input[lookahead] == 'x' || input[lookahead] == 'y' || input[lookahead] == 'z') {
        lookahead++;  // Match 'x', 'y', or 'z'
    } else {
        printf("Rejected\n");  // Invalid variable
        exit(1);
    }
}

// N → D N | D
void N() {
    D();
    while (isdigit(input[lookahead])) {
        D();
    }
}

// D → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
void D() {
    if (isdigit(input[lookahead])) {
        lookahead++;  // Match a digit
    } else {
        printf("Rejected\n");  // Invalid digit
        exit(1);
    }
}

// Function to remove spaces from the input string
void removeSpaces(char *source) {
    char *i = source, *j = source;
    while (*j != '\0') {
        *i = *j++;
        if (*i != ' ') {
            i++;
        }
    }
    *i = '\0';
}

// Main function to test the parser
int main() {
    char check[100];
    printf("Enter string : ");
    fgets(check, 100, stdin);
    if (check[strlen(check) - 1] == '\n')
        check[strlen(check) - 1] = '\0';    
    strcat(check, "$");

    // Remove spaces from input
    removeSpaces(check);
    strcpy(input, check);

    printf("Input: %s\n", input);

    S();  // Start parsing from S

    // If the input is fully parsed and ends with the '$' symbol
    if (input[lookahead] == '$')
        printf("Accepted\n");
    else
        printf("Rejected\n");

    return 0;
}