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

// Initialises Productions to '?'
void initialiseProds(){
    for(int i = 0; i < maxsize;i++){
        for(int j = 0; j < maxsize;j++){
            prods[i][j] = '?';
        }
    }
    printf("Initialised successfully\n");
}

// Prints array when array & size are passed
void print(char arr[], int size){
    for(int i = 0 ; i < size; i++){
        printf("%c ", arr[i]);
    }printf("\n");
}

// Prints stored first elements
void printFirst(char arr[], int size){
    printf("{ ");
    for(int i = 1 ; i < size; i++){
        printf("%c, ", arr[i]);
    }printf("}\n");
}

// Checks if the character is in the array or not
bool notin(char arr[], int size, char ch){
    for(int i = 0 ; i < size;i++){
        if(arr[i] == ch)
            return false;
    }
    return true;
}

void initialiseFIRSTS(){
    for(int i = 0; i < nonterminalIndex;i++){
        FIRSTS[i][firstIndices[i]++] = nonterminals[i];
    }
    printf("Initialised FIRSTS array\n");
}

void classify(){
    for(int i = 0; i < n;i++){
        for(int j = 0; prods[i][j] != '\0';j++){
            if(isupper(prods[i][j])){
                if(notin(nonterminals, nonterminalIndex, prods[i][j]))
                    nonterminals[nonterminalIndex++] = prods[i][j];
            }else{  // Includes symbols into terminals as well except =
                if(notin(terminals, terminalIndex, prods[i][j]) && prods[i][j] != '=')
                    terminals[terminalIndex++] = prods[i][j];
            }
        }
    }
    printf("Terminals : ");
    print(terminals, terminalIndex);
    printf("TERMINAL INDEX : %d\n", terminalIndex);
    printf("Non terminals : ");
    print(nonterminals, nonterminalIndex);
    printf("NON TERMINAL INDEX : %d\n", nonterminalIndex);

    initialiseFIRSTS();
}

bool isTerminal(char ch){
    for(int i = 0; i < terminalIndex;i++){
        if(ch == terminals[i])
            return true;
    }
    return false;
}

void insertIntoFirsts(char nonTerminal, char firstValue){
    for(int i = 0; i < nonterminalIndex;i++){
        if(FIRSTS[i][0] == nonTerminal && notin(FIRSTS[i], firstIndices[i], firstValue)){
            FIRSTS[i][firstIndices[i]++] = firstValue;
        }
    }
}

int findIndexOfNonTerminal(char ch){
    for(int i = 0; i < nonterminalIndex;i++){
        if(nonterminals[i] == ch)
            return i;
    }
    return -1;
}

bool epsilonInNonTerminalFirst(int lhsIndex){
    for(int i = 0; i < firstIndices[lhsIndex];i++){
        if(FIRSTS[lhsIndex][i] == '#'){
            printf("EPSILON IN NON TERMINAL : %d %c\n", i, FIRSTS[lhsIndex][i]);
            return true;
        }
    }
    // printf("EPSILON NOT IN NON TERMINAL\n");
    return false;
}

void removeEpsilonFromFirst(int index){
    for(int i = 0; i < firstIndices[index];i++){
        // printf("%c>>>>>>>>>>\n", FIRSTS[index][i]);
        if(FIRSTS[index][i] == '#'){
            printf("%c \n", FIRSTS[index][i]);
            FIRSTS[index][i] = '-';
            printf("Changed successfully\n");
        }
    }
    print(FIRSTS[index] ,firstIndices[index]);
    printf("REMOVED SUCCESSFULLY\n");
}

void appendFirst(int leftIndex, int index){
    print(FIRSTS[leftIndex], firstIndices[leftIndex]);
    print(FIRSTS[index], firstIndices[index]);
    for(int i = 1;i < firstIndices[index];i++){
        if(notin(FIRSTS[leftIndex], firstIndices[leftIndex], FIRSTS[index][i])){
            FIRSTS[leftIndex][firstIndices[leftIndex]++] = FIRSTS[index][i];
        }
    }
    // FIRSTS[leftIndex][firstIndices[leftIndex]++] = '\n';
    print(FIRSTS[leftIndex], firstIndices[leftIndex]);
    printf("First Index[%c] : %d\n",FIRSTS[leftIndex][0], firstIndices[leftIndex]);
    printf("Appended Successfully\n");
}

void findFirst(char c){
    for(int i = 0; i < n;i++){
        if(prods[i][0] == c){
            if(prods[i][2] != '\0' || prods[i][2] != '?'){  // If the symbol is in productions
                if(isTerminal(prods[i][2])){    // Add to firsts set if the symbol is a non terminal
                    insertIntoFirsts(prods[i][0], prods[i][2]);
                }else{  // First char on RHS is Non Terminal
                    int prodCol = 2;
                    if(prods[i][0] == prods[i][prodCol] && !flagForRecalculateFirst[findIndexOfNonTerminal(prods[i][0])]){   // If left recursive grammar is found
                        flagForRecalculateFirst[findIndexOfNonTerminal(prods[i][0])] = true;
                        continue;
                    }
                    printf("FOUND NON TERMINAL : %c\n", prods[i][prodCol]);
                    int index = findIndexOfNonTerminal(prods[i][prodCol]);
                    printf("INDEX OF NON TERMINAL : %d\n", index);
                    if(index == -1)
                        continue;
                    if(firstIndices[index] == 1){   // If first of the non terminal is not calculated, find first of the non terminal
                        printf("FIRST OF NON TERMINAL %c IS NOT CALCULATED\n", prods[i][prodCol]);
                        findFirst(prods[i][prodCol]);   // Finding first of next non terminal
                        printf("CALCULATED FIRST OF %c\n", prods[i][prodCol]);
                        int lhsIndex = findIndexOfNonTerminal(prods[i][0]);
                        index = findIndexOfNonTerminal(prods[i][prodCol]);
                        appendFirst(lhsIndex, index);
                        printf("First Index[%c] in findFirst : %d\n",FIRSTS[lhsIndex][0], firstIndices[lhsIndex]);
                        while(epsilonInNonTerminalFirst(lhsIndex)){   // If epsilon is present in firsts set
                            // sleep(2);
                            printf("After sleep : ");
                            print(FIRSTS[lhsIndex], firstIndices[lhsIndex]);
                            if(prodCol != strlen(prods[i] - 1)){
                                // printf("LOOP 1\n");
                                removeEpsilonFromFirst(lhsIndex);
                                prodCol++;
                                if(firstIndices[findIndexOfNonTerminal(prods[i][prodCol])] == 1){
                                    findFirst(prods[i][prodCol]);
                                }
                                index = findIndexOfNonTerminal(prods[i][prodCol]);
                                appendFirst(lhsIndex, index);
                            }
                            else{
                                break;
                            }
                        }
                    }else{
                        int leftIndex = findIndexOfNonTerminal(prods[i][0]);
                        int prevIndex = findIndexOfNonTerminal(prods[i][prodCol]);
                        index = findIndexOfNonTerminal(prods[i][prodCol+1]);
                        if(leftIndex == prevIndex){
                            // printf("=================\n");
                            while(prods[i][prodCol+1] != '\0' && epsilonInNonTerminalFirst(leftIndex)){   // If epsilon is present in firsts set
                                // printf("%c=======\n", prods[i][prodCol+1]);
                                if(prods[i][prodCol+1] != '\0'){
                                    // print(FIRSTS[prevIndex], firstIndices[prevIndex]);
                                    // print(FIRSTS[index], firstIndices[index]);
                                    printf("LOOP 2");
                                    removeEpsilonFromFirst(prevIndex);
                                    // print(FIRSTS[prevIndex], firstIndices[prevIndex]);
                                    // print(FIRSTS[index], firstIndices[index]);
                                }
                                else{
                                    break;
                                }
                            }
                        }
                        appendFirst(leftIndex, index);      // Critical Test Case works here
                    }
                }
            }
        }
    }
    printf("END OF FIND FIRSTS\n");
}

void initialiseFollows(){
    for(int i = 0 ; i < maxsize; i++){
        if(i < nonterminalIndex)
            FOLLOWS[i][0] = nonterminals[i];
            followIndices[i]++;
        for(int j = 1; j < maxsize; j++){
            FOLLOWS[i][j] = '?';
            
        }
    }
    FOLLOWS[0][1] = '$';
    followIndices[0]++;
    printf("Initialised Follows successfully\n");
}

void insertIntoFollow(int ii, int jj){
    int leftIndex = findIndexOfNonTerminal(prods[ii][jj]);
    int index = findIndexOfNonTerminal(prods[ii][jj+1]);
    if(notin(FOLLOWS[leftIndex], followIndices[leftIndex], prods[ii][jj+1])){
        FOLLOWS[index][followIndices[index]++] = prods[ii][jj+1];
        printf("Follow %c inserted successfully into %c\n", prods[ii][jj+1], prods[ii][jj]);
    }else{
        printf("Follow is already present, not inserted\n");
    }
}

void appendFollowWithFirst(int leftIndex, int index){
    for(int i = 1;i < followIndices[index];i++){
        if(notin(FOLLOWS[leftIndex], followIndices[leftIndex], FOLLOWS[index][i])){
            FOLLOWS[leftIndex][followIndices[leftIndex]++] = FIRSTS[index][i];
        }
    }
    // FIRSTS[leftIndex][firstIndices[leftIndex]++] = '\n';
    print(FOLLOWS[leftIndex], followIndices[leftIndex]);
    printf("Follow Index[%c] : %d\n",FOLLOWS[leftIndex][0], followIndices[leftIndex]);
    printf("Appended Successfully\n");
}

void appendFollow(int leftIndex, int index){
    for(int i = 1;i < followIndices[index];i++){
        if(notin(FOLLOWS[leftIndex], followIndices[leftIndex], FOLLOWS[index][i])){
            FOLLOWS[leftIndex][followIndices[leftIndex]++] = FOLLOWS[index][i];
        }
    }
    // FIRSTS[leftIndex][firstIndices[leftIndex]++] = '\n';
    print(FOLLOWS[leftIndex], followIndices[leftIndex]);
    printf("Follow Index[%c] : %d\n",FOLLOWS[leftIndex][0], followIndices[leftIndex]);
    printf("Appended Successfully\n");
}

void findFollow(char ch){
    for(int i = 0 ; i < nonterminalIndex;i++){
        for(int j = 2; j < strlen(prods[i]);j++){
            if(ch == prods[i][j]){
                printf("CHECKING IF CHAR IS TERMINAL OR NOT\n");
                printf("RETURN STATUS : %d\n",isTerminal(prods[i][j+1]));
                if((isTerminal(prods[i][j+1]) || prods[i][j + 1] == '\0') && prods[i][j+1] != '?'){  // Check if next character is terminal or not
                    // insertIntoFollow(findIndexOfNonTerminal(prods[i][j]), prods[i][j+1]);
                    printf("INSERTING TERMINAL %c\n\n", prods[i][j+1]);
                    insertIntoFollow(i,j);
                }else{                          // If it is non terminal
                    printf("CHAR IS NON TERMINAL %c\n\n\n", prods[i][j+1]);
                    if(strlen(prods[i]) - 1 != j){  // If current char is  not at the end of production
                        findFollow(prods[i][j+1]);
                        int leftIndex = findIndexOfNonTerminal(prods[i][j]);
                        int index = findIndexOfNonTerminal(prods[i][j+1]);
                        appendFollowWithFirst(leftIndex, index);
                    }else{                  // If current char is at end, append follow of lhs
                        if(followIndices[prods[i][0]] == 1){
                            findFollow(prods[i][0]);
                            int leftIndex = findIndexOfNonTerminal(prods[i][j]);
                            int index = findIndexOfNonTerminal(prods[i][j+1]);
                            appendFollow(leftIndex, index);
                        }
                    }
                }
            }
        }
    }
}

int main(){
    // ADD REMOVE SPACES FUNCTION AFTER TAKING INPUT
    initialiseProds();
    // printf("Enter number of productions : ");
    // scanf("%d", &n);

    FILE *file = fopen("input.txt", "r");

    if(file == NULL){
        perror("Error in opening the file\n");
        exit(1);
    }

    fscanf(file, "%d", &n);

    for(int i = 0; i < n ; i++){
        fscanf(file, "%s", prods[i]);
        // prods[strlen(prods[i])-1] = '\0';
        // printf("prods[i] : <%s>\n", prods[i]);
        printf("%s\n", prods[i]);
    }

    fclose(file);

    classify();

    for(int i = 0 ; i < n;i++){
        printf("prods[%d] : ", i);
        printf("Len of %s is %ld\n",prods[i], strlen(prods[i]));
    }

    for(int i = 0; i < nonterminalIndex ; i++){
        findFirst(nonterminals[i]);    //Finding first for every non terminal
        if(flagForRecalculateFirst[i])
            findFirst(nonterminals[i]);
    }
    
    for(int i = 0; i < nonterminalIndex;i++){
        printf("%d ", firstIndices[i]);
        printf("FIRSTS[%d] %c : ", i, FIRSTS[i][0]);
        printFirst(FIRSTS[i], firstIndices[i]);
    }

    initialiseFollows();
    
    for(int i = 0 ; i < nonterminalIndex;i++){
        print(FOLLOWS[i],followIndices[i]);
        findFollow(nonterminals[i]);
    }

    for(int i = 0; i < nonterminalIndex;i++){
        printf("%d ", followIndices[i]);
        printf("FIRSTS[%d] %c : ", i, FOLLOWS[i][0]);
        printFirst(FOLLOWS[i], followIndices[i]);
    }

    return 0;
}