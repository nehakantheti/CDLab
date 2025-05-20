#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int totalProductions;   // Max value is 10.
char productionRules[10][10];
char firstSet[10], followSet[10];

char finalFirst[10][100];
char finalFollow[10][100];

int processedFirstCount = -1;
int numFirstSets = 0;

void computeFirst(char nonTerminal, int productionIndex, int symbolIndex) {
    if (!isupper(nonTerminal)) {
        firstSet[numFirstSets++] = nonTerminal;
        return;
    }
    
    for (int i = 0; i < totalProductions; i++) {
        if (productionRules[i][0] == nonTerminal) {
            if (productionRules[i][2] == '#') {
                if (productionRules[productionIndex][symbolIndex] != '\0' && (productionIndex != 0 || symbolIndex != 0)) {
                    computeFirst(productionRules[productionIndex][symbolIndex], productionIndex, symbolIndex + 1);
                } else {
                    firstSet[numFirstSets++] = '#';
                }
            } else if (!isupper(productionRules[i][2])) {
                firstSet[numFirstSets++] = productionRules[i][2];
            } else {
                computeFirst(productionRules[i][2], i, 3);
            }
        }
    }
}

int main() {
    int firstSetStartIndex = 0;
    int followSetStartIndex = 0;

    printf("Enter number of productions : ");
    scanf("%d", &totalProductions);

    char processedFirsts[totalProductions];  // Array to store processed non-terminals

    printf("Enter the production rules:\n");
    for (int i = 0; i < totalProductions; i++) {
        scanf("%s", productionRules[i]);
    }

    // Initialize the finalFirst array with '?'
    for (int i = 0; i < totalProductions; i++) {
        for (int j = 0; j < 100; j++) {
            finalFirst[i][j] = '?';
        }
    }

    int firstSetRow = 0, firstSetCol, found;
    char nonTerminal;

    for (int i = 0; i < totalProductions; i++) {
        nonTerminal = productionRules[i][0];
        firstSetCol = 0;
        found = 0;

        // Checking if First of nonTerminal has already been calculated
        for (int j = 0; j <= processedFirstCount; j++) {
            if (nonTerminal == processedFirsts[j]) {
                found = 1;
                break;
            }
        }

        if (found == 1) {
            continue;
        }

        // Compute First set for nonTerminal
        computeFirst(nonTerminal, 0, 0);
        processedFirstCount++;
        processedFirsts[processedFirstCount] = nonTerminal;
        printf("\nFirst(%c) = { ", nonTerminal);
        finalFirst[firstSetRow][firstSetCol++] = nonTerminal;

        for (int i = 0 + firstSetStartIndex; i < numFirstSets; i++) {
            int duplicate = 0;
            for (int j = 0; j < firstSetCol; j++) {
                if (firstSet[i] == finalFirst[firstSetRow][j]) {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate == 0) {
                printf("%c, ", firstSet[i]);
                finalFirst[firstSetRow][firstSetCol++] = firstSet[i];
            }
        }
        printf("}\n");
        firstSetStartIndex = numFirstSets;
        firstSetRow++;
    }

    return 0;
}
