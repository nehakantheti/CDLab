#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

char terminals[10];
int terminalIndex = 0;
char nonTerminals[10];
int nonTerminalIndex = 0;

bool searchInLhs(int n, char lhs[], char c){
    printf("Searching in LHS \n");
    for(int i = 0;i< n;i++){
        if(lhs[i] == c)
            return true;
    }
    return false;
}

bool searchInTerminals(char c){
    printf("Searching in terminals : \n");
    for(int i = 0;i< terminalIndex;i++){
        if(terminals[i] == c)
            return true;
    }return false;
}

// void insertIntoArray(char arr[], int* size, char c){
//     printf("Inserting into array\n");
//     for(int i = 0;i<*size;i++){
//         if(arr[i] == c){
//             return ;
//         }
//         arr[*(size)++] = c;
//     }
// }

void insertIntoArray(char arr[], int* size, char c) {
    printf("Inserting '%c' into array. Current size: %d\n", c, *size);
    
    // Check if the character is already in the array
    for (int i = 0; i < *size; i++) {
        if (arr[i] == c) {
            printf("Character '%c' already in array. Skipping insertion.\n", c);
            return;  // If the character already exists, do nothing
        }
    }

    // Insert the new character and increment the size
    arr[*size] = c;
    (*size)++;

    printf("Character '%c' inserted. New size: %d\n", c, *size);
}


void printArray(char arr[], int size){
    printf("Printing Array : \n");
    for(int i = 0;i< size;i++){
        printf("%c ", arr[i]);
    }printf("\n");
}

int findIndexInTerminal(char c){
    for(int i = 0;i< terminalIndex;i++){
        if(terminals[i] == c){
            return i;
        }
    }
    return -1;
}

int findIndexInNonTerminal(char c){
    for(int i = 0;i< nonTerminalIndex;i++){
        if(nonTerminals[i] == c){
            return i;
        }
    }
    return -1;
}

void firstsOfTerminals(char** firsts, char indices[][2]){
    for(int i = 0;i< terminalIndex;i++){
        firsts[i][1] = terminals[i];
        indices[i][1] = ((indices[i][1]-'0')+1)+'0';
    }

    for(int i = 0;i< numOfProds;i++){
        for(int j = 0;j < strlen(rhs[i];j++)){
            if(rhs[i][j] == '$'){
                indexInTerminal = findIndexInNonTerminal(lhs[i]);
                if(indexInTerminal !=-1){
                    firsts[indexInterminal]
                }
            }
        }
    }
}

int main(){
    char lhs[10];
    char* rhs[10];

    int numOfProds;
    printf("Enter number of productions : \n");
    scanf("%d", & numOfProds);

    //Taking input from user
    for(int i = 0; i< numOfProds;i++){
        printf("Enter lhs : \n");
        scanf(" %c", &lhs[i]);

        rhs[i] = malloc(10 * sizeof(char));
        printf("Enter rhs : \n");
        scanf(" %s", rhs[i]);
    }

    printf("Input taken !\n");

    // dividing terminals and non temrinals
    for(int i = 0 ; i< numOfProds;i++){
        for(int j = 0;j< strlen(rhs[i]);j++){
            if(!searchInLhs(numOfProds, lhs, rhs[i][j])){
                printf("INside if : \n");
                insertIntoArray(terminals, &terminalIndex, rhs[i][j]);
                // terminals[terminalIndex++] = rhs[i][j];
            }
        }
    }

    for(int i = 0;i< numOfProds;i++){
        if(!searchInTerminals(lhs[i])){

            insertIntoArray(nonTerminals, &nonTerminalIndex, lhs[i]);
            // nonTerminals[nonTerminalIndex++] = lhs[i];
        }

        for(int j = 0;j< strlen(rhs[i]);j++){
            if(!searchInTerminals(rhs[i][j])){
                insertIntoArray(nonTerminals, &nonTerminalIndex, rhs[i][j]);
                // nonTerminals[nonTerminalIndex++] = rhs[i][j];
            }
        }
    }

    printArray(terminals, terminalIndex);
    printArray(nonTerminals, nonTerminalIndex);

    //Calculating firsts
    char** firsts;
    firsts = malloc((terminalIndex + nonTerminalIndex)*sizeof(char*));
    
    char indices[terminalIndex+nonTerminalIndex][2];

    for(int i = 0;i< terminalIndex;i++){
        firsts[i] = malloc(10 * sizeof(char));
        indices[i][0] = terminals[i];
        indices[i][1] = '0';
        firsts[i][0] = terminals[i];
        firsts[i][1] = '\0';
    }


    for(int i = 0;i< nonTerminalIndex;i++){
        firsts[i+terminalIndex] = malloc(10 * sizeof(char));
        indices[i+terminalIndex][0] = nonTerminals[i];
        indices[i+terminalIndex][1] = '0';
        firsts[i+terminalIndex][0] = nonTerminals[i];
        firsts[i+terminalIndex][1] = '\0';
    }

    printf("-----%d %d \n", terminalIndex, nonTerminalIndex);

    for(int i = 0;i< terminalIndex+nonTerminalIndex;i++){
        printf("%c %c\n", indices[i][0], indices[i][1]);
    }

    for(int i = 0; firsts[i] != NULL;i++){
        for(int j = 0;firsts[i][j]!='\0';j++){
            printf("%c ", firsts[i][j]);
        }printf("\n");
    }

    // Calculating firsts of terminals
    firstsOfTerminals(firsts,indices);

    return 0;
}