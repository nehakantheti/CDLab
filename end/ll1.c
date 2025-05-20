// Chat gpt generated, not working
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 10
#define SIZE 100

char nonTerminals[MAX], terminals[MAX];
char first[MAX][SIZE], follow[MAX][SIZE];
char table[MAX][MAX][SIZE];
char productions[MAX][SIZE];
int numProductions, numNonTerminals, numTerminals;

int getNonTerminalIndex(char c) {
    for (int i = 0; i < numNonTerminals; i++)
        if (nonTerminals[i] == c) return i;
    return -1;
}

int getTerminalIndex(char c) {
    for (int i = 0; i < numTerminals; i++)
        if (terminals[i] == c) return i;
    return -1;
}

void addToSet(char* set, char val) {
    if (!strchr(set, val)) {
        int len = strlen(set);
        set[len] = val;
        set[len + 1] = '\0';
    }
}

void computeFirst(char symbol, char* result) {
    if (!isupper(symbol)) {
        addToSet(result, symbol);
        return;
    }
    int index = getNonTerminalIndex(symbol);
    for (int i = 0; i < numProductions; i++) {
        if (productions[i][0] == symbol) {
            if (productions[i][2] == '#') addToSet(result, '#');
            else {
                int j = 2;
                while (productions[i][j] != '\0') {
                    char temp[SIZE] = "";
                    computeFirst(productions[i][j], temp);
                    if (!strchr(temp, '#')) {
                        strcat(result, temp);
                        break;
                    }
                    strcat(result, temp);
                    j++;
                }
            }
        }
    }
}

void computeFollow(char symbol) {
    int index = getNonTerminalIndex(symbol);
    if (symbol == nonTerminals[0]) addToSet(follow[index], '$');
    for (int i = 0; i < numProductions; i++) {
        int len = strlen(productions[i]);
        for (int j = 2; j < len; j++) {
            if (productions[i][j] == symbol) {
                if (j + 1 < len) {
                    char temp[SIZE] = "";
                    computeFirst(productions[i][j + 1], temp);
                    for (int k = 0; k < strlen(temp); k++)
                        if (temp[k] != '#') addToSet(follow[index], temp[k]);
                    if (strchr(temp, '#') || j + 1 == len)
                        strcat(follow[index], follow[getNonTerminalIndex(productions[i][0])]);
                } else strcat(follow[index], follow[getNonTerminalIndex(productions[i][0])]);
            }
        }
    }
}

void computeFirstFollow() {
    for (int i = 0; i < numNonTerminals; i++) {
        computeFirst(nonTerminals[i], first[i]);
    }
    for (int i = 0; i < numNonTerminals; i++) {
        computeFollow(nonTerminals[i]);
    }
}

void constructParsingTable() {
    for (int i = 0; i < numNonTerminals; i++) {
        for (int j = 0; j < numTerminals; j++) strcpy(table[i][j], "");
    }
    for (int i = 0; i < numProductions; i++) {
        char* prod = productions[i];
        char nonTerminal = prod[0];
        char rhs[SIZE];
        strcpy(rhs, prod + 2);
        char firstSet[SIZE] = "";
        computeFirst(rhs[0], firstSet);
        for (int j = 0; j < strlen(firstSet); j++) {
            int row = getNonTerminalIndex(nonTerminal);
            int col = getTerminalIndex(firstSet[j]);
            if (firstSet[j] != '#' && col != -1) strcpy(table[row][col], rhs);
        }
        if (strchr(firstSet, '#')) {
            char* followSet = follow[getNonTerminalIndex(nonTerminal)];
            for (int j = 0; j < strlen(followSet); j++) {
                int col = getTerminalIndex(followSet[j]);
                if (col != -1) strcpy(table[getNonTerminalIndex(nonTerminal)][col], "#");
            }
        }
    }
}

void displayParsingTable() {
    printf("\nParsing Table:\n\t");
    for (int i = 0; i < numTerminals; i++) printf("%c\t", terminals[i]);
    printf("\n");
    for (int i = 0; i < numNonTerminals; i++) {
        printf("%c\t", nonTerminals[i]);
        for (int j = 0; j < numTerminals; j++) {
            if (strcmp(table[i][j], "") != 0) printf("%s\t", table[i][j]);
            else printf("-\t");
        }
        printf("\n");
    }
}

void parseString(char* input) {
    char stack[SIZE];
    int top = 0;
    stack[top] = '$';
    stack[++top] = nonTerminals[0];
    int i = 0;
    printf("\nStack\tInput\tAction\n");
    while (top >= 0) {
        printf("%s\t%s\t", stack, input + i);
        char stackTop = stack[top--];
        if (stackTop == input[i]) {
            printf("Match %c\n", stackTop);
            i++;
        } else if (isupper(stackTop)) {
            int row = getNonTerminalIndex(stackTop);
            int col = getTerminalIndex(input[i]);
            if (col != -1 && strcmp(table[row][col], "") != 0) {
                char* production = table[row][col];
                printf("%c->%s\n", stackTop, production);
                for (int j = strlen(production) - 1; j >= 0; j--) {
                    if (production[j] != '#') stack[++top] = production[j];
                }
            } else {
                printf("Error\n");
                return;
            }
        } else {
            printf("Error\n");
            return;
        }
    }
    if (input[i] == '$') printf("Accepted\n");
    else printf("Rejected\n");
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &numProductions);
    for (int i = 0; i < numProductions; i++) {
        printf("Enter production %d: ", i + 1);
        scanf("%s", productions[i]);
    }
    printf("Enter number of non-terminals: ");
    scanf("%d", &numNonTerminals);
    printf("Enter non-terminals: ");
    for (int i = 0; i < numNonTerminals; i++) {
        scanf(" %c", &nonTerminals[i]);
    }
    printf("Enter number of terminals: ");
    scanf("%d", &numTerminals);
    printf("Enter terminals: ");
    for (int i = 0; i < numTerminals; i++) {
        scanf(" %c", &terminals[i]);
    }
    computeFirstFollow();
    constructParsingTable();
    displayParsingTable();
    char input[SIZE];
    printf("Enter input string: ");
    scanf("%s", input);
    strcat(input, "$");
    parseString(input);
    return 0;
}