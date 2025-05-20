#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char lhs;
    char rhs[10];
} Production;

typedef struct {
    char lhs;
    char rhs[10];
    int dotPos;  // Position of the dot (.)
} Item;

typedef struct {
    Item items[MAX];
    int count;
} State;

Production productions[MAX];
int prodCount = 0;
char terminals[MAX], nonTerminals[MAX];
int terminalCount = 0, nonTerminalCount = 0;
State states[MAX];
int stateCount = 0;
int parseTable[MAX][MAX];  // For ACTION and GOTO table

// Function to check if a given state already exists
int stateExists(State newState) {
    for (int i = 0; i < stateCount; i++) {
        if (states[i].count == newState.count) {
            int identical = 1;
            for (int j = 0; j < states[i].count; j++) {
                if (states[i].items[j].lhs != newState.items[j].lhs ||
                    strcmp(states[i].items[j].rhs, newState.items[j].rhs) != 0 ||
                    states[i].items[j].dotPos != newState.items[j].dotPos) {
                    identical = 0;
                    break;
                }
            }
            if (identical) return i;
        }
    }
    return -1;
}

// Function to calculate the closure of a given set of items
State closure(State state) {
    State result = state;
    for (int i = 0; i < result.count; i++) {
        Item item = result.items[i];
        if (item.dotPos < strlen(item.rhs)) {
            char B = item.rhs[item.dotPos];
            if (strchr(nonTerminals, B) != NULL) {
                for (int j = 0; j < prodCount; j++) {
                    if (productions[j].lhs == B) {
                        Item newItem;
                        newItem.lhs = B;
                        strcpy(newItem.rhs, productions[j].rhs);
                        newItem.dotPos = 0;

                        // Check if the item is already in the result set
                        int exists = 0;
                        for (int k = 0; k < result.count; k++) {
                            if (result.items[k].lhs == newItem.lhs &&
                                strcmp(result.items[k].rhs, newItem.rhs) == 0 &&
                                result.items[k].dotPos == newItem.dotPos) {
                                exists = 1;
                                break;
                            }
                        }
                        if (!exists) {
                            result.items[result.count++] = newItem;
                        }
                    }
                }
            }
        }
    }
    return result;
}

// Function to perform the GOTO operation
State Goto(State state, char symbol) {
    State result = {0};
    for (int i = 0; i < state.count; i++) {
        Item item = state.items[i];
        if (item.dotPos < strlen(item.rhs) && item.rhs[item.dotPos] == symbol) {
            Item newItem = item;
            newItem.dotPos++;
            result.items[result.count++] = newItem;
        }
    }
    return closure(result);
}

// Function to construct the canonical collection of LR(0) items
void constructCanonicalCollection() {
    // Create the initial state
    State initialState = {0};
    initialState.items[0].lhs = productions[0].lhs;
    strcpy(initialState.items[0].rhs, productions[0].rhs);
    initialState.items[0].dotPos = 0;
    initialState.count = 1;
    initialState = closure(initialState);
    states[stateCount++] = initialState;

    // Construct the collection of sets of LR(0) items
    for (int i = 0; i < stateCount; i++) {
        for (int j = 0; j < terminalCount + nonTerminalCount; j++) {
            char symbol = (j < terminalCount) ? terminals[j] : nonTerminals[j - terminalCount];
            State newState = Goto(states[i], symbol);
            if (newState.count > 0) {
                int existingStateIndex = stateExists(newState);
                if (existingStateIndex == -1) {
                    states[stateCount++] = newState;
                }
            }
        }
    }
}

// Function to display the states and items
void displayStates() {
    printf("\n\nCanonical Collection of LR(0) Items:\n");
    for (int i = 0; i < stateCount; i++) {
        printf("\nI%d:\n", i);
        for (int j = 0; j < states[i].count; j++) {
            Item item = states[i].items[j];
            printf("%c -> ", item.lhs);
            for (int k = 0; k < strlen(item.rhs); k++) {
                if (k == item.dotPos) printf(".");
                printf("%c", item.rhs[k]);
            }
            if (item.dotPos == strlen(item.rhs)) printf(".");
            printf("\n");
        }
    }
}

// Function to read the grammar and augment it
void readGrammar() {
    printf("Enter the number of productions: ");
    scanf("%d", &prodCount);

    printf("Enter the productions (e.g., S->AB):\n");
    for (int i = 0; i < prodCount; i++) {
        char production[10];
        scanf("%s", production);
        productions[i].lhs = production[0];
        strcpy(productions[i].rhs, &production[3]);

        // Add unique non-terminals
        if (strchr(nonTerminals, production[0]) == NULL) {
            nonTerminals[nonTerminalCount++] = production[0];
        }

        // Add unique symbols in the RHS
        for (int j = 0; j < strlen(production); j++) {
            char symbol = production[j];
            if (symbol >= 'a' && symbol <= 'z' && strchr(terminals, symbol) == NULL) {
                terminals[terminalCount++] = symbol;
            }
        }
    }

    // Augment the grammar by adding S' -> S (assuming S is the original start symbol)
    for (int i = prodCount; i > 0; i--) {
        productions[i] = productions[i - 1];
    }
    productions[0].lhs = 'Z';  // New start symbol S'
    productions[0].rhs[0] = productions[1].lhs;
    productions[0].rhs[1] = '\0';
    prodCount++;
}

int main() {
    readGrammar();
    constructCanonicalCollection();
    displayStates();
    // Further code to construct the SLR table and check conflicts can be added here.
    return 0;
}
