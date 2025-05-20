#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define max 100

int n;
char prods[max][max];
char terminals[max];
int terminalIndex = 0;
char nonterminals[max];
int nonterminalIndex = 0;
char firsts[max][max];
int firstIndices[max];

bool notin(char arr[], int size, char ch){
    for(int i = 0; i < size;i++){
        if(arr[i] == ch){
            return false;
        }
    }
    return true;
}

void initialise(){
    for(int i = 0;i < max;i++){
        terminals[i] = '?';
        nonterminals[i] = '?';
        for(int j = 0; j < max;j++){
            firsts[i][j] = '?';
        }
    }
}

void classify(){
    for(int i = 0; i < n;i++){
        int j = 0;
        while(prods[i][j]){
            if(prods[i][j] >= 'A' && prods[i][j] <= 'Z'){
                if(notin(nonterminals, nonterminalIndex, prods[i][j]))
                    nonterminals[nonterminalIndex++] = prods[i][j];
            }else if(prods[i][j] >= 'a' && prods[i][j] <= 'z'){
                if(notin(terminals, terminalIndex, prods[i][j]))
                    terminals[terminalIndex++] = prods[i][j];
            }
            j++;
        }
    }
}

bool isterminal(char ch){
    if(ch >= 'a' && ch <= 'z'){
        return true;
    }
    return false;
}

bool isnonterminal(char ch){
    if(ch >= 'A' && ch <= 'Z'){
        return true;
    }
    return false;
}

void print(char arr[], int size){
    for(int i = 0; i < size;i++){
        printf("%c ", arr[i]);
    }printf("\n");
}

int indexof(char arr[], int size, char ch){
    for(int i = 0;i < size;i++){
        if(ch == arr[i])
            return true;
    }
    return false;
}

void unionFirsts(int lhs, int rhs){
    
}

void findFirst(char ch){
    for(int i =0; i < n;i++){
        if(prods[i][0] == ch){
            if(isterminal(prods[i][2])){    // If first value is terminal
                if(notin(firsts[i], firstIndices[i], prods[i][2])){
                    firsts[i][firstIndices[i]++] = prods[i][2];
                }
            }else if(isnonterminal(prods[i][2])){
                int rhsIndex = indexof(nonterminals,nonterminalIndex, prods[i][2]);
                unionFirsts(i, rhsIndex);
            }
        }
    }
}

int main(){
    printf("Enter n : ");
    scanf("%d", &n);
    printf("Enter productions (Single productions with equal) : \n");
    for(int i = 0; i < n; i++){
        printf("Enter production %d : ", i + 1);
        scanf("%s", &prods[i]);
    }
    initialise();
    classify();
    printf("%d %d\n", terminalIndex, nonterminalIndex);
    printf("Non terminal : \n");
    print(nonterminals, nonterminalIndex);
    printf("Terminals : \n");
    print(terminals, terminalIndex);

    for(int i = 0; i < nonterminalIndex;i++){
        findFirst(nonterminals[i]);
    }

    return 0;
}