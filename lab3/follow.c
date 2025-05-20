#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Functions to calculate Follow
void computeFollowFirst(char nonTerminal, int productionIndex, int nextSymbolIndex);
void computeFollow(char nonTerminal);

// Function to calculate First
void computeFirst(char nonTerminal, int productionIndex, int symbolIndex);

int totalProductions, numFirstSets = 0;

// Stores the final result of the First Sets
char firstSets[10][100];

// Stores the final result of the Follow Sets
char followSets[10][100];
int numFollowSets = 0;

// Stores the production rules
char productionRules[10][10];
char followSet[10], firstSet[10];
int prodIndex;  // Renamed to avoid conflict with 'index' function in string.h
char currentNonTerminal;
int temp;

int main(int argc, char** argv)
{
    int firstSetStartIndex = 0;
    int followSetStartIndex = 0;
    int followSetCol; // Declare followSetCol here
    int i;
    char nonTerminal, symbol;
    totalProductions = 8;

    // The Input grammar
    strcpy(productionRules[0], "X=TnS");
    strcpy(productionRules[1], "X=Rm");
    strcpy(productionRules[2], "T=q");
    strcpy(productionRules[3], "T=#");
    strcpy(productionRules[4], "S=p");
    strcpy(productionRules[5], "S=#");
    strcpy(productionRules[6], "R=om");
    strcpy(productionRules[7], "R=ST");

    int tempIndex;
    char processedFirst[totalProductions];
    int processedCount = -1;

    // Initializing the firstSets array
    for (prodIndex = 0; prodIndex < totalProductions; prodIndex++) {
        for (tempIndex = 0; tempIndex < 100; tempIndex++) {
            firstSets[prodIndex][tempIndex] = '!';
        }
    }
    int firstSetRow = 0, firstSetCol, found;

    for (prodIndex = 0; prodIndex < totalProductions; prodIndex++) {
        nonTerminal = productionRules[prodIndex][0];
        firstSetCol = 0;
        found = 0;

        // Checking if First of nonTerminal has already been calculated
        for (tempIndex = 0; tempIndex <= processedCount; tempIndex++)
            if (nonTerminal == processedFirst[tempIndex])
                found = 1;

        if (found == 1)
            continue;

        // Function call
        computeFirst(nonTerminal, 0, 0);
        processedCount += 1;

        // Adding nonTerminal to the processed list
        processedFirst[processedCount] = nonTerminal;
        printf("\n First(%c) = { ", nonTerminal);
        firstSets[firstSetRow][firstSetCol++] = nonTerminal;

        // Printing the First Sets of the grammar
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
    printf("\n");
    printf("-----------------------------------------------\n\n");
    char processedFollow[totalProductions];
    processedCount = -1;

    // Initializing the followSets array
    for (prodIndex = 0; prodIndex < totalProductions; prodIndex++) {
        for (tempIndex = 0; tempIndex < 100; tempIndex++) {
            followSets[prodIndex][tempIndex] = '!';
        }
    }
    firstSetRow = 0;
    int followSetCount = 0;
    for (temp = 0; temp < totalProductions; temp++) {
        currentNonTerminal = productionRules[temp][0];
        followSetCol = 0;
        found = 0;

        // Checking if Follow of currentNonTerminal has already been calculated
        for (tempIndex = 0; tempIndex <= processedCount; tempIndex++)
            if (currentNonTerminal == processedFollow[tempIndex])
                found = 1;

        if (found == 1)
            continue;
        followSetCount += 1;

        // Function call
        computeFollow(currentNonTerminal);
        processedCount += 1;

        // Adding currentNonTerminal to the processed list
        processedFollow[processedCount] = currentNonTerminal;
        printf(" Follow(%c) = { ", currentNonTerminal);
        followSets[firstSetRow][followSetCol++] = currentNonTerminal;

        // Printing the Follow Sets of the grammar
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
        printf(" }\n\n");
        followSetStartIndex = numFollowSets;
        firstSetRow++;
    }
}

void computeFollow(char nonTerminal)
{
    int productionIndex, symbolIndex;

    // Adding "$" to the follow set of the start symbol
    if (productionRules[0][0] == nonTerminal) {
        followSet[numFollowSets++] = '$';
    }
    for (productionIndex = 0; productionIndex < 10; productionIndex++) {
        for (symbolIndex = 2; symbolIndex < 10; symbolIndex++) {
            if (productionRules[productionIndex][symbolIndex] == nonTerminal) {
                if (productionRules[productionIndex][symbolIndex + 1] != '\0') {
                    // Calculate the First of the next Non-Terminal in the production
                    computeFollowFirst(productionRules[productionIndex][symbolIndex + 1], productionIndex, (symbolIndex + 2));
                }

                if (productionRules[productionIndex][symbolIndex + 1] == '\0' && nonTerminal != productionRules[productionIndex][0]) {
                    // Calculate the Follow of the Non-Terminal in the L.H.S. of the production
                    computeFollow(productionRules[productionIndex][0]);
                }
            }
        }
    }
}

void computeFirst(char nonTerminal, int productionIndex, int symbolIndex)
{
    int j;

    // The case where we encounter a Terminal
    if (!(isupper(nonTerminal))) {
        firstSet[numFirstSets++] = nonTerminal;
    }
    for (j = 0; j < totalProductions; j++) {
        if (productionRules[j][0] == nonTerminal) {
            if (productionRules[j][2] == '#') {
                if (productionRules[productionIndex][symbolIndex] == '\0')
                    firstSet[numFirstSets++] = '#';
                else if (productionRules[productionIndex][symbolIndex] != '\0' && (productionIndex != 0 || symbolIndex != 0)) {
                    // Recursion to calculate First of New Non-Terminal we encounter after epsilon
                    computeFirst(productionRules[productionIndex][symbolIndex], productionIndex, (symbolIndex + 1));
                }
                else
                    firstSet[numFirstSets++] = '#';
            }
            else if (!isupper(productionRules[j][2])) {
                firstSet[numFirstSets++] = productionRules[j][2];
            }
            else {
                // Recursion to calculate First of New Non-Terminal we encounter at the beginning
                computeFirst(productionRules[j][2], j, 3);
            }
        }
    }
}

void computeFollowFirst(char nonTerminal, int productionIndex, int symbolIndex)
{
    int k;

    // The case where we encounter a Terminal
    if (!(isupper(nonTerminal)))
        followSet[numFollowSets++] = nonTerminal;
    else {
        int row = 0, column = 1;
        for (row = 0; row < totalProductions; row++) {
            if (firstSets[row][0] == nonTerminal)
                break;
        }

        // Including the First set of the Non-Terminal in the Follow of the original query
        while (firstSets[row][column] != '!') {
            if (firstSets[row][column] != '#') {
                followSet[numFollowSets++] = firstSets[row][column];
            }
            else {
                if (productionRules[productionIndex][symbolIndex] == '\0') {
                    // Case where we reach the end of a production
                    computeFollow(productionRules[productionIndex][0]);
                }
                else {
                    // Recursion to the next symbol in case we encounter a "#"
                    computeFollowFirst(productionRules[productionIndex][symbolIndex], productionIndex, symbolIndex + 1);
                }
            }
            column++;
        }
    }
}
