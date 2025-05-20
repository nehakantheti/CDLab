#include <stdio.h>
#include <string.h>

#define MAX 10
#define MAXLEN 10

// Structure to hold grammar production
struct Production {
    char non_terminal;
    char production[MAXLEN];
};

int isLeftRecursive(struct Production prods[], int n) {
    for (int i = 0; i < n; i++) {
        if (prods[i].non_terminal == prods[i].production[0]) {
            return 1; 
        }
    }
    return 0;
}

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

int parseString(char *input, int *pos, char non_terminal, struct Production prods[], int n, int input_len, int depth) {
    if (depth > input_len) {
        return 0; 
    }

    char result[MAX][MAXLEN];
    int rule_count = 0;

    if (!findProduction(non_terminal, prods, n, result, &rule_count)) {
        return 0;
    }

    for (int r = 0; r < rule_count; r++) {
        int backup_pos = *pos;
        int match = 1; 

        for (int i = 0; result[r][i] != '\0'; i++) {
            if (result[r][i] == '#') {
                continue;
            }

            if (result[r][i] >= 'A' && result[r][i] <= 'Z') {
                if (!parseString(input, pos, result[r][i], prods, n, input_len, depth + 1)) {
                    match = 0;
                    break;
                }
            } else {
                if (*pos >= input_len || input[*pos] != result[r][i]) {
                    match = 0;
                    break;
                }
                (*pos)++;  
            }
        }

        if (match) {
            return 1; 
        } else {
            *pos = backup_pos; 
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
        printf("Enter production %d (e.g., A->Ac or A-># for epsilon): ", i + 1);
        scanf(" %c->%s", &prods[i].non_terminal, prods[i].production);
    }

    char input[MAXLEN];
    printf("Enter the string to check: ");
    scanf("%s", input);

    char ch;
    printf("Enter start symbol: ");
    scanf(" %c", &ch);

    int pos = 0;
    int input_len = strlen(input);
    
    int result = parseString(input, &pos, ch, prods, n, input_len, 0); 

    if (result && pos == input_len) {
        printf("The string '%s' is accepted by the grammar.\n", input);
    } else {
        printf("The string '%s' is rejected by the grammar.\n", input);
    }

    return 0;
}