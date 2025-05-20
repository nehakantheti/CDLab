#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PRODUCTIONS 100
#define MAX_TERMS 100

char nonterms[MAX_PRODUCTIONS][2]; 
char terms[MAX_TERMS][2];         
int nonterms_count = 0, terms_count = 0;
char productions[MAX_PRODUCTIONS][MAX_TERMS];
int prod_count = 0;
char production_dict[MAX_PRODUCTIONS][MAX_PRODUCTIONS][MAX_TERMS]; 

bool isTerm(char ter) {
    if (ter == '#') return true;
    for (int i = 0; i < terms_count; i++) {
        if (ter == terms[i][0]) {
            return true;
        }
    }
    return false;
}
bool isNTerm(char nT) {
    for (int i = 0; i < nonterms_count; i++) {
        if (nT == nonterms[i][0]) {
            return true;
        }
    }
    return false;
}
bool LeftCheck(int nT_index, char search) {
    for (int i = 0; i < MAX_PRODUCTIONS && production_dict[nT_index][i][0] != '\0'; i++) {
        if (production_dict[nT_index][i][0] == search) {
            return true;
        } else if (isNTerm(production_dict[nT_index][i][0])) {
            return LeftCheck(production_dict[nT_index][i][0] - 'A', search);
        }
    }
    return false;
}
void Left_toRight(int nT_index) {
    char bet[MAX_PRODUCTIONS][MAX_TERMS] = {{0}};
    char alpha[MAX_PRODUCTIONS][MAX_TERMS] = {{0}};
    int bet_count = 0, alpha_count = 0;

    for (int i = 0; i < MAX_PRODUCTIONS && production_dict[nT_index][i][0] != '\0'; i++) {
        if (production_dict[nT_index][i][0] == nonterms[nT_index][0]) {
            strcpy(alpha[alpha_count], &production_dict[nT_index][i][1]);
            alpha_count++;
        } else {
            strcpy(bet[bet_count], production_dict[nT_index][i]);
            bet_count++;
        }
    }

    printf("%c -> ", nonterms[nT_index][0]);
    for (int i = 0; i < bet_count; i++) {
        printf("%s%c' / ", bet[i], nonterms[nT_index][0]);
    }
    printf("\n");

    printf("%c' -> ", nonterms[nT_index][0]);
    for (int i = 0; i < alpha_count; i++) {
        printf("%s%c' / ", alpha[i], nonterms[nT_index][0]);
    }
    printf("#\n");
}


void Prod_print(int nT_index) {
    printf("%c -> ", nonterms[nT_index][0]);
    for (int i = 0; i < MAX_PRODUCTIONS && production_dict[nT_index][i][0] != '\0'; i++) {
        printf("%s / ", production_dict[nT_index][i]);
    }
    printf("\n");
}

void findTerminals() {
    for (int i = 0; i < prod_count; i++) {
        for (int j = 3; productions[i][j] != '\0'; j++) {
            bool is_terminal = true;
            for (int k = 0; k < nonterms_count; k++) {
                if (productions[i][j] == nonterms[k][0] || productions[i][j] == '#' || productions[i][j] == '/') {
                    is_terminal = false;
                    break;
                }
            }
            if (is_terminal) {
                char t[2] = {productions[i][j], '\0'};
                strcpy(terms[terms_count], t);
                terms_count++;
            }
        }
    }
}
int main() {
    int n;
    printf("Enter the number of productions: ");
    scanf("%d", &n);
    getchar();
    printf("\nRules:\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("Epsilon is represented by '#'\nProductions are of the form A->B, where 'A' is a single Non-Terminal.\n\n");

    for (int i = 0; i < n; i++) {
        printf("Enter production %d: ", i + 1);
        fgets(productions[i], MAX_TERMS, stdin);
        productions[i][strcspn(productions[i], "\n")] = '\0';
        char nonterm[2] = {productions[i][0], '\0'};
        strcpy(nonterms[nonterms_count], nonterm);
        nonterms_count++;
    }
    prod_count = n;

    findTerminals();

    printf("Non-Terminals: ");
    for (int i = 0; i < nonterms_count; i++) {
        printf("%s ", nonterms[i]);
    }
    printf("\n");

    printf("Terminals: ");
    for (int i = 0; i < terms_count; i++) {
        printf("%s ", terms[i]);
    }
    printf("\n");

    for (int i = 0; i < prod_count; i++) {
        char* token = strtok(productions[i] + 3, "/");
        int prod_index = 0;
        while (token != NULL) {
            strcpy(production_dict[i][prod_index], token);
            token = strtok(NULL, "/");
            prod_index++;
        }
    }

    bool leftcheck_dict[MAX_PRODUCTIONS] = {false};
    bool rightcheck_dict[MAX_PRODUCTIONS] = {false};

    for (int i = 0; i < nonterms_count; i++) {
        leftcheck_dict[i] = LeftCheck(i, nonterms[i][0]);
    }

    printf("\n");
    for (int i = 0; i < nonterms_count; i++) {
        if (leftcheck_dict[i]) {
            printf("%c contains Left Recursive Grammar\n", nonterms[i][0]);
        }
    }

    printf("--------------------------------\nLeft Eliminated Grammar:\n");
    for (int i = 0; i < nonterms_count; i++) {
        if (leftcheck_dict[i]) {
            Left_toRight(i);
        } else {
            Prod_print(i);
        }
    }

    return 0;
}