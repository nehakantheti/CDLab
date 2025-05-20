#include <stdio.h>
#include <string.h>

#define MAX 10
#define MAXLEN 10

// Structure to hold grammar production
struct Production {
    char non_terminal;
    char production[MAXLEN];
};

// Function to check for left recursion
int isLeftRecursive(struct Production prods[], int n) {
    for (int i = 0; i < n; i++) {
        if (prods[i].non_terminal == prods[i].production[0]) {
            return 1; // Left recursion detected
        }
    }
    return 0;
}

// Function to find a production rule for a given non-terminal
int findProduction(char non_terminal, struct Production prods[], int n, char result[MAXLEN][MAXLEN], int *rule_count) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (prods[i].non_terminal == non_terminal) {
            strcpy(result[*rule_count], prods[i].production);
            (*rule_count)++;
            found = 1;
        }
    }
    return found;
}

// Recursive function to verify the string based on grammar rules
int parseString(char *input, int *pos, char non_terminal, struct Production prods[], int n, int input_len, int depth) {
    // Prevent infinite recursion (depth limit)
    if (depth > input_len) {
        return 0; // Stop recursion if it gets too deep
    }

    char result[MAX][MAXLEN];
    int rule_count = 0;

    // Get all the productions for the non-terminal
    if (!findProduction(non_terminal, prods, n, result, &rule_count)) {
        return 0; // No production found for the non-terminal
    }

    for (int r = 0; r < rule_count; r++) {
        int backup_pos = *pos;  // Store current position
        int match = 1;  // Assume it matches, until proven otherwise

        for (int i = 0; result[r][i] != '\0'; i++) {
            if (result[r][i] >= 'A' && result[r][i] <= 'Z') {
                // It's a non-terminal, so we recursively check it
                if (!parseString(input, pos, result[r][i], prods, n, input_len, depth + 1)) {
                    match = 0;  // If recursion fails, it's not a match
                    break;
                }
            } else {
                // It's a terminal, match it directly with the input
                if (*pos >= input_len || input[*pos] != result[r][i]) {
                    match = 0;  // Terminal doesn't match
                    break;
                }
                (*pos)++;  // Move to the next character
            }
        }

        if (match) {
            return 1;  // If one production matches, return success
        } else {
            *pos = backup_pos;  // If failed, backtrack to previous position
        }
    }

    return 0;
}

int main() {
    int n;
    printf("Enter the number of productions: ");
    scanf("%d", &n);

    struct Production prods[MAX];

    // Input grammar productions
    for (int i = 0; i < n; i++) {
        printf("Enter production %d (e.g., A->Ac): ", i + 1);
        scanf(" %c->%s", &prods[i].non_terminal, prods[i].production);
    }

    // Check for left recursion
    // if (isLeftRecursive(prods, n)) {
    //     printf("The grammar contains left recursion. Parsing cannot proceed.\n");
    //     return 1;
    // }

    char input[MAXLEN];
    printf("Enter the string to check: ");
    scanf("%s", input);

    char ch;
    printf("Enter start symbol: ");
    scanf(" %c", &ch);

    int pos = 0;
    int input_len = strlen(input);
    
    // Reset pos and make sure the parsing begins with the start symbol
    int result = parseString(input, &pos, ch, prods, n, input_len, 0); 

    // Check if parsing succeeded and entire input is consumed
    if (result && pos == input_len) {
        printf("The string '%s' is accepted by the grammar.\n", input);
    } else {
        printf("The string '%s' is rejected by the grammar.\n", input);
    }

    return 0;
}
