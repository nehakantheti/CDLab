#include <stdio.h>
#include <string.h>


#define MAX_PRODUCTIONS 10
#define MAX_LENGTH 100



void convert_left_recursion(char non_terminal, char productions[][MAX_LENGTH], int num_productions);


int main() {
    int num_productions;
    char non_terminal;
    char productions[MAX_PRODUCTIONS][MAX_LENGTH];


    printf("Enter the non-terminal (e.g., A): ");
    scanf(" %c", &non_terminal);


    printf("Enter number of productions for %c: ", non_terminal);
    scanf("%d", &num_productions);


    printf("Enter the productions (use | for alternatives, e.g., A -> Aa | b):\n");
    for (int i = 0; i < num_productions; i++) {
        printf("Production %d: %c -> ", i + 1, non_terminal);
        scanf(" %s", productions[i]);
    }


    convert_left_recursion(non_terminal, productions, num_productions);


    return 0;
}


void convert_left_recursion(char non_terminal, char productions[][MAX_LENGTH], int num_productions) {
    char alpha[MAX_PRODUCTIONS][MAX_LENGTH];  // To store AÎ± (left-recursive part)
    char beta[MAX_PRODUCTIONS][MAX_LENGTH];   // To store Î² (non-left-recursive part)
    int alpha_count = 0, beta_count = 0;


    for (int i = 0; i < num_productions; i++) {
        if (productions[i][0] == non_terminal) {
            strcpy(alpha[alpha_count], productions[i] + 1);  
            alpha_count++;
        } else {
            strcpy(beta[beta_count], productions[i]);
            beta_count++;
        }
    }


    if (alpha_count == 0) {
        printf("No left recursion detected in the grammar.\n");
        return;
    }


    printf("The grammar after removing left recursion:\n");


    for (int i = 0; i < beta_count; i++) {
        printf("%c -> %s%c'\n", non_terminal, beta[i], non_terminal);
    }


    for (int i = 0; i < alpha_count; i++) {
        printf("%c' -> %s%c'\n", non_terminal, alpha[i], non_terminal);
    }
    printf("%c' -> #\n", non_terminal); 
}