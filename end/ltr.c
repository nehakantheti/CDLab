#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

#define maxsize 20

char prods[maxsize][maxsize];   // Initial productions
int n;                          // Number of productions
char terminals[maxsize];        // Terminals array
int terminalIndex = 0;          // Stores max Terminal index
char nonterminals[maxsize];     // Non terminals array
int nonterminalIndex = 0;       // Stores non terminal index
char FIRSTS[maxsize][maxsize];  // Stores firsts of non terminal - has length equal to nonteminal index
int firstIndices[maxsize] = {0};// Stores size in each firsts array
bool flagForRecalculateFirst[maxsize] = {false};    // If indirect recursion is detected

char FOLLOWS[maxsize][maxsize]; // Stores follows of non terminals
int followIndices[maxsize] = {0};

// Parsing Table
char parsingTable[maxsize][maxsize][maxsize];
int parsingTableIndices[maxsize][maxsize] = {{0}}; // Keeps track of entries in each parsing table cell

// Function declarations...

void initialiseParsingTable() {
    for(int i = 0; i < maxsize; i++) {
        for(int j = 0; j < maxsize; j++) {
            parsingTableIndices[i][j] = 0;
            for(int k = 0; k < maxsize; k++) {
                parsingTable[i][j][k] = '?';
            }
        }
    }
    printf("Parsing Table Initialized Successfully\n");
}

int findTerminalIndex(char ch) {
    for(int i = 0; i < terminalIndex; i++) {
        if(terminals[i] == ch) return i;
    }
    return -1;
}

void addProductionToParsingTable(int row, int col, int prodIndex) {
    int index = parsingTableIndices[row][col]++;
    strcpy(parsingTable[row][col] + index, prods[prodIndex]);
    printf("Added production %s at parsingTable[%c][%c]\n", prods[prodIndex], nonterminals[row], terminals[col]);
}

void fillParsingTable() {
    for (int i = 0; i < n; i++) {
        char lhs = prods[i][0];
        int lhsIndex = findIndexOfNonTerminal(lhs);

        char firstSymbol = prods[i][2];
        if (firstSymbol == '#') { // Handle epsilon (ε)
            for (int j = 1; j < followIndices[lhsIndex]; j++) {
                int col = findTerminalIndex(FOLLOWS[lhsIndex][j]);
                if (col != -1) {
                    addProductionToParsingTable(lhsIndex, col, i);
                }
            }
        } else if (isTerminal(firstSymbol)) { // Direct terminal in FIRST set
            int col = findTerminalIndex(firstSymbol);
            if (col != -1) {
                addProductionToParsingTable(lhsIndex, col, i);
            }
        } else { // Handle non-terminal in FIRST set
            int firstSymbolIndex = findIndexOfNonTerminal(firstSymbol);
            for (int j = 1; j < firstIndices[firstSymbolIndex]; j++) {
                if (FIRSTS[firstSymbolIndex][j] != '#') {
                    int col = findTerminalIndex(FIRSTS[firstSymbolIndex][j]);
                    if (col != -1) {
                        addProductionToParsingTable(lhsIndex, col, i);
                    }
                }
            }
        }
    }
}

void printParsingTable() {
    printf("\nParsing Table:\n");
    printf("NT\\T | ");
    for (int i = 0; i < terminalIndex; i++) {
        printf(" %c\t", terminals[i]);
    }
    printf("\n");

    for (int i = 0; i < nonterminalIndex; i++) {
        printf(" %c   | ", nonterminals[i]);
        for (int j = 0; j < terminalIndex; j++) {
            if (parsingTableIndices[i][j] == 0) {
                printf(" -\t");
            } else {
                printf("%s\t", parsingTable[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    initialiseProds();
    FILE *file = fopen("input.txt", "r");
    if(file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fscanf(file, "%d", &n);
    for(int i = 0; i < n ; i++) {
        fscanf(file, "%s", prods[i]);
        printf("%s\n", prods[i]);
    }
    fclose(file);

    classify();
    for(int i = 0; i < nonterminalIndex; i++) {
        findFirst(nonterminals[i]);
        if(flagForRecalculateFirst[i]) {
            findFirst(nonterminals[i]);
        }
    }

    for(int i = 0; i < nonterminalIndex; i++) {
        printf("FIRSTS[%d] %c : ", i, FIRSTS[i][0]);
        printFirst(FIRSTS[i], firstIndices[i]);
    }

    initialiseFollows();
    for(int i = 0; i < nonterminalIndex; i++) {
        findFollow(nonterminals[i]);
    }

    for(int i = 0; i < nonterminalIndex; i++) {
        printf("FOLLOWS[%d] %c : ", i, FOLLOWS[i][0]);
        printFirst(FOLLOWS[i], followIndices[i]);
    }

    initialiseParsingTable();
    fillParsingTable();
    printParsingTable();

    return 0;
}