#include <stdio.h>
#include <string.h>

#define MAX 100

char nonTerminal;        // Non-terminal (e.g., S)
char alpha[MAX][MAX];    // Left-recursive productions (e.g., Sα)
char beta[MAX][MAX];     // Non-recursive productions (e.g., β)
int alphaCount = 0;      // Number of left-recursive productions
int betaCount = 0;       // Number of non-recursive productions

void readGrammar() {
    char input[MAX];
    printf("Enter a grammar rule (e.g., S->Sa|Sb|c): ");
    scanf("%s", input); 

    nonTerminal = input[0]; // The non-terminal is the first character
    char *token = strtok(input + 3, "|"); // Skip "S->" and split by "|"

    // Separate productions into alpha and beta
    while (token != NULL) {
        if (token[0] == nonTerminal) {
            strcpy(alpha[alphaCount++], token + 1); // Left-recursive (skip the non-terminal)
        } else {
            strcpy(beta[betaCount++], token); // Non-recursive
        }
        token = strtok(NULL, "|");
    }
}

void eliminateLeftRecursion() {
    if (alphaCount > 0) { // If left recursion exists
        char newNonTerminal = nonTerminal + '1'; // New non-terminal (e.g., S1)

        // Print the non-recursive rule: S -> βS1
        printf("%c -> ", nonTerminal);
        for (int i = 0; i < betaCount; i++) {
            printf("%s%c", beta[i], newNonTerminal);
            if (i < betaCount - 1) printf(" | ");
        }
        printf("\n");

        // Print the recursive rule: S1 -> αS1 | #
        printf("%c -> ", newNonTerminal);
        for (int i = 0; i < alphaCount; i++) {
            printf("%s%c", alpha[i], newNonTerminal);
            if (i < alphaCount - 1) printf(" | ");
        }
        printf(" | #\n"); // Epsilon production
    } else { 
        printf("// No left recursion");
    }
}

int main() {
    readGrammar(); // Read the input grammar rule
    printf("Converted Grammar:\n");
    eliminateLeftRecursion(); // Eliminate left recursion and print the result
    return 0;
}