#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTIONS 10
#define MAX_LENGTH 100

// Grammar represented by left-hand side (lhs) and right-hand side (rhs) arrays
char lhs[MAX_PRODUCTIONS];
char rhs[MAX_PRODUCTIONS][MAX_PRODUCTIONS][MAX_LENGTH];
int productionCount = 0;
int rhsCount[MAX_PRODUCTIONS];

void addProduction(char left, char* right) {
    int i;
    for (i = 0; i < productionCount; i++) {
        if (lhs[i] == left) {
            strcpy(rhs[i][rhsCount[i]++], right);
            return;
        }
    }
    lhs[productionCount] = left;
    strcpy(rhs[productionCount][rhsCount[productionCount]++], right);
    productionCount++;
}

void inputGrammar() {
    int n;
    printf("Enter number of productions: ");
    scanf("%d", &n);
    printf("Enter productions (format: A->Aa|b):\n");

    for (int i = 0; i < n; i++) {
        char left, right[MAX_LENGTH];
        scanf(" %c->%s", &left, right);

        char* token = strtok(right, "|");
        while (token != NULL) {
            addProduction(left, token);
            token = strtok(NULL, "|");
        }
    }
}

void displayGrammar() {
    printf("\nUpdated Grammar:\n");
    for (int i = 0; i < productionCount; i++) {
        printf("%c -> ", lhs[i]);
        for (int j = 0; j < rhsCount[i]; j++) {
            printf("%s", rhs[i][j]);
            if (j < rhsCount[i] - 1)
                printf(" | ");
        }
        printf("\n");
    }
}

void eliminateDirectRecursion(int index) {
    char left = lhs[index];
    char alpha[MAX_PRODUCTIONS][MAX_LENGTH], beta[MAX_PRODUCTIONS][MAX_LENGTH];
    int alphaCount = 0, betaCount = 0;

    // Separate alpha and beta productions
    for (int i = 0; i < rhsCount[index]; i++) {
        if (rhs[index][i][0] == left) {  // Direct left recursion
            strcpy(alpha[alphaCount++], rhs[index][i] + 1);  // Exclude the left-recursive symbol
        } else {
            strcpy(beta[betaCount++], rhs[index][i]);
        }
    }

    if (alphaCount > 0) {  // If there's direct recursion
        rhsCount[index] = 0;
        char newNonTerminal = left + 'Z' - 'A' + 1;

        // Update beta productions
        for (int i = 0; i < betaCount; i++) {
            sprintf(rhs[index][rhsCount[index]++], "%s%c", beta[i], newNonTerminal);
        }

        // Add new productions for alpha with epsilon termination
        lhs[productionCount] = newNonTerminal;
        rhsCount[productionCount] = 0;
        
        for (int i = 0; i < alphaCount; i++) {
            sprintf(rhs[productionCount][rhsCount[productionCount]++], "%s%c", alpha[i], newNonTerminal);
        }
        sprintf(rhs[productionCount][rhsCount[productionCount]++], "ε");  // Add epsilon
        productionCount++;
    }
}

void substitute(int fromIndex, int toIndex) {
    char newRhs[MAX_PRODUCTIONS][MAX_LENGTH];
    int newRhsCount = 0;
    char fromSymbol = lhs[fromIndex];

    for (int i = 0; i < rhsCount[toIndex]; i++) {
        if (rhs[toIndex][i][0] == fromSymbol) {
            // Substitute each production of fromSymbol into the toIndex production
            for (int j = 0; j < rhsCount[fromIndex]; j++) {
                sprintf(newRhs[newRhsCount++], "%s%s", rhs[fromIndex][j], rhs[toIndex][i] + 1);
            }
        } else {
            strcpy(newRhs[newRhsCount++], rhs[toIndex][i]);
        }
    }

    rhsCount[toIndex] = newRhsCount;
    for (int i = 0; i < newRhsCount; i++) {
        strcpy(rhs[toIndex][i], newRhs[i]);
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