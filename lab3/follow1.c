#include <ctype.h>
#include <stdio.h>
#include <string.h>

void computeFollowFirst(char nonTerminal, int productionIndex, int nextSymbolIndex);
void computeFollow(char nonTerminal);

void computeFirst(char nonTerminal, int productionIndex, int symbolIndex);

int totalProductions, numFirstSets = 0;

char firstSets[10][100];

char followSets[10][100];
int numFollowSets = 0;

char productionRules[10][10];
char followSet[10], firstSet[10];
int productionIndex;
char currentNonTerminal;
int temp;

int main()
{
    int firstSetStartIndex = 0;
    int followSetStartIndex = 0;
    int followSetCol;
    int i;
    char nonTerminal, symbol;
    printf("Enter number of productions : ");
    scanf("%d", &totalProductions);

    for(int i = 0;i < totalProductions;i++){
        scanf("%s", productionRules[i]);
    }

    int tempIndex;
    char processedFirst[totalProductions];
    int processedCount = -1;

    for (productionIndex = 0; productionIndex < totalProductions; productionIndex++) {
        for (tempIndex = 0; tempIndex < 100; tempIndex++) {
            firstSets[productionIndex][tempIndex] = '!';
        }
    }
    int firstSetRow = 0, firstSetCol, found;

    for (productionIndex = 0; productionIndex < totalProductions; productionIndex++) {
        nonTerminal = productionRules[productionIndex][0];
        firstSetCol = 0;
        found = 0;

        for (tempIndex = 0; tempIndex <= processedCount; tempIndex++)
            if (nonTerminal == processedFirst[tempIndex])
                found = 1;

        if (found == 1)
            continue;

        computeFirst(nonTerminal, 0, 0);
        processedCount += 1;

        processedFirst[processedCount] = nonTerminal;
        printf("First(%c) = { ", nonTerminal);
        firstSets[firstSetRow][firstSetCol++] = nonTerminal;

        for (i = 0 + firstSetStartIndex; i < numFirstSets; i++) {
            int alreadyPresent = 0, duplicate = 0;

            for (alreadyPresent = 0; alreadyPresent < firstSetCol; alreadyPresent++) {

                if (firstSet[i] == firstSets[firstSetRow][alreadyPresent]) {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate == 0) {
                printf("%c, ", firstSet[i]);
                firstSets[firstSetRow][firstSetCol++] = firstSet[i];
            }
        }
        printf("}\n");
        firstSetStartIndex = numFirstSets;
        firstSetRow++;
    }
    char processedFollow[totalProductions];
    processedCount = -1;

    for (productionIndex = 0; productionIndex < totalProductions; productionIndex++) {
        for (tempIndex = 0; tempIndex < 100; tempIndex++) {
            followSets[productionIndex][tempIndex] = '!';
        }
    }
    firstSetRow = 0;
    int followSetCount = 0;
    for (temp = 0; temp < totalProductions; temp++) {
        currentNonTerminal = productionRules[temp][0];
        followSetCol = 0;
        found = 0;

        for (tempIndex = 0; tempIndex <= processedCount; tempIndex++)
            if (currentNonTerminal == processedFollow[tempIndex])
                found = 1;

        if (found == 1)
            continue;
        followSetCount += 1;

        computeFollow(currentNonTerminal);
        processedCount += 1;

        processedFollow[processedCount] = currentNonTerminal;
        printf(" Follow(%c) = { ", currentNonTerminal);
        followSets[firstSetRow][followSetCol++] = currentNonTerminal;

        for (i = 0 + followSetStartIndex; i < numFollowSets; i++) {
            int alreadyPresent = 0, duplicate = 0;
            for (alreadyPresent = 0; alreadyPresent < followSetCol; alreadyPresent++) {
                if (followSet[i] == followSets[firstSetRow][alreadyPresent]) {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate == 0) {
                printf("%c, ", followSet[i]);
                followSets[firstSetRow][followSetCol++] = followSet[i];
            }
        }
        printf(" }\n");
        followSetStartIndex = numFollowSets;
        firstSetRow++;
    }
}

void computeFollow(char nonTerminal)
{
    int productionIndex, symbolIndex;

    if (productionRules[0][0] == nonTerminal) {
        followSet[numFollowSets++] = '$';
    }
    for (productionIndex = 0; productionIndex < 10; productionIndex++) {
        for (symbolIndex = 2; symbolIndex < 10; symbolIndex++) {
            if (productionRules[productionIndex][symbolIndex] == nonTerminal) {
                if (productionRules[productionIndex][symbolIndex + 1] != '\0') {
                    computeFollowFirst(productionRules[productionIndex][symbolIndex + 1], productionIndex, (symbolIndex + 2));
                }

                if (productionRules[productionIndex][symbolIndex + 1] == '\0' && nonTerminal != productionRules[productionIndex][0]) {
                    computeFollow(productionRules[productionIndex][0]);
                }
            }
        }
    }
}

void computeFirst(char nonTerminal, int productionIndex, int symbolIndex)
{
    int j;

    if (!(isupper(nonTerminal))) {
        firstSet[numFirstSets++] = nonTerminal;
    }
    for (j = 0; j < totalProductions; j++) {
        if (productionRules[j][0] == nonTerminal) {
            if (productionRules[j][2] == '$') {
                if (productionRules[productionIndex][symbolIndex] == '\0')
                    firstSet[numFirstSets++] = '$';
                else if (productionRules[productionIndex][symbolIndex] != '\0' && (productionIndex != 0 || symbolIndex != 0)) {
                    computeFirst(productionRules[productionIndex][symbolIndex], productionIndex, (symbolIndex + 1));
                }
                else
                    firstSet[numFirstSets++] = '$';
            }
            else if (!isupper(productionRules[j][2])) {
                firstSet[numFirstSets++] = productionRules[j][2];
            }
            else {
                computeFirst(productionRules[j][2], j, 3);
            }
        }
    }
}

void computeFollowFirst(char nonTerminal, int productionIndex, int symbolIndex)
{
    int k;

    if (!(isupper(nonTerminal)))
        followSet[numFollowSets++] = nonTerminal;
    else {
        int row = 0, column = 1;
        for (row = 0; row < totalProductions; row++) {
            if (firstSets[row][0] == nonTerminal)
                break;
        }

        while (firstSets[row][column] != '!') {
            if (firstSets[row][column] != '$') {
                followSet[numFollowSets++] = firstSets[row][column];
            }
            else {
                if (productionRules[productionIndex][symbolIndex] == '\0') {
                    computeFollow(productionRules[productionIndex][0]);
                }
                else {
                    computeFollowFirst(productionRules[productionIndex][symbolIndex], productionIndex, symbolIndex + 1);
                }
            }
            column++;
        }
    }
}
