#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define BUFFER 100

typedef struct {
    char lhs;
    char rhs[BUFFER][BUFFER];
    int count;
} Production;

Production grammar[MAX];
int productionCount = 0;

void addProduction(char lhs, char* rhs) {
    int i;
    for (i = 0; i < productionCount; i++) {
        if (grammar[i].lhs == lhs) {
            strcpy(grammar[i].rhs[grammar[i].count++], rhs);
            return;
        }
    }
    grammar[productionCount].lhs = lhs;
    strcpy(grammar[productionCount].rhs[grammar[productionCount].count++], rhs);
    productionCount++;
}

void inputGrammar() {
    int n;
    printf("Enter number of productions: ");
    scanf("%d", &n);
    printf("Enter productions (format: A->Aa|b):\n");

    for (int i = 0; i < n; i++) {
        char lhs, rhs[BUFFER];
        scanf(" %c->%s", &lhs, rhs);

        char* token = strtok(rhs, "|");
        while (token != NULL) {
            addProduction(lhs, token);
            token = strtok(NULL, "|");
        }
    }
}

void displayGrammar() {
    printf("\nUpdated Grammar:\n");
    for (int i = 0; i < productionCount; i++) {
        printf("%c -> ", grammar[i].lhs);
        for (int j = 0; j < grammar[i].count; j++) {
            printf("%s", grammar[i].rhs[j]);
            if (j < grammar[i].count - 1)
                printf(" | ");
        }
        printf("\n");
    }
}

void eliminateDirectRecursion(int index) {
    char lhs = grammar[index].lhs;
    char alpha[BUFFER][BUFFER], beta[BUFFER][BUFFER];
    int alphaCount = 0, betaCount = 0;

    for (int i = 0; i < grammar[index].count; i++) {
        if (grammar[index].rhs[i][0] == lhs) {
            strcpy(alpha[alphaCount++], grammar[index].rhs[i] + 1);
        } else {
            strcpy(beta[betaCount++], grammar[index].rhs[i]);
        }
    }

    if (alphaCount > 0) { 
        grammar[index].count = 0;
        char newNonTerminal = lhs + 'Z' - 'A' + 1;

        for (int i = 0; i < betaCount; i++) {
            sprintf(grammar[index].rhs[grammar[index].count++], "%s%c", beta[i], newNonTerminal);
        }

        grammar[productionCount].lhs = newNonTerminal;
        grammar[productionCount].count = 0;

        for (int i = 0; i < alphaCount; i++) {
            sprintf(grammar[productionCount].rhs[grammar[productionCount].count++], "%s%c", alpha[i], newNonTerminal);
        }
        sprintf(grammar[productionCount].rhs[grammar[productionCount].count++], "ε");
        productionCount++;
    }
}

void substitute(int fromIndex, int toIndex) {
    char newRhs[BUFFER][BUFFER];
    int newRhsCount = 0;
    char fromSymbol = grammar[fromIndex].lhs;

    for (int i = 0; i < grammar[toIndex].count; i++) {
        if (grammar[toIndex].rhs[i][0] == fromSymbol) {
            for (int j = 0; j < grammar[fromIndex].count; j++) {
                sprintf(newRhs[newRhsCount++], "%s%s", grammar[fromIndex].rhs[j], grammar[toIndex].rhs[i] + 1);
            }
        } else {
            strcpy(newRhs[newRhsCount++], grammar[toIndex].rhs[i]);
        }
    }

    grammar[toIndex].count = newRhsCount;
    for (int i = 0; i < newRhsCount; i++) {
        strcpy(grammar[toIndex].rhs[i], newRhs[i]);
    }
}

void eliminateIndirectRecursion() {
    for (int i = 0; i < productionCount; i++) {
        for (int j = 0; j < i; j++) {
            substitute(j, i);
        }
        eliminateDirectRecursion(i);
    }
}

int main() {
    inputGrammar();
    eliminateIndirectRecursion();
    displayGrammar();
    return 0;
}