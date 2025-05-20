#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define maxSize 128

char inputgrammar[10][10];
char terminals[20];
int terminalIndex = 0;
char nonterminals[20];
int nonterminalIndex = 0;
char slrItems[20][20][20];
int numOfItems[30] = {0};
int stateIndex = 0, closureIndex = 0;
int numOfProductions;
int numOfStates = 0;

bool check(char arr[], int size, char c){
    for(int i = 0; i < size;i++){
        if(arr[i] == c){
            return true;
        }
    }
    return false;
}

void extractTerminalsAndNonterminals(int numOfTerminals){
    for(int i = 1 ; i < numOfTerminals ; i++){
        for(int j = 0; inputgrammar[i][j] != '\0';j++){
            if(inputgrammar[i][j] >= 'a' && inputgrammar[i][j] <= 'z' && !check(terminals, terminalIndex, inputgrammar[i][j])){
                terminals[terminalIndex++] = inputgrammar[i][j];
            }else if(inputgrammar[i][j] >= 'A' && inputgrammar[i][j] <= 'Z' && !check(nonterminals, nonterminalIndex, inputgrammar[i][j])){
                nonterminals[nonterminalIndex++] = inputgrammar[i][j];
            }
        }
    }
    terminals[terminalIndex] = '$'; //Appending $ to terminals since it is considered as non temrinal at table construction.
    terminalIndex++;
    terminals[terminalIndex] = '\0';
    nonterminals[nonterminalIndex] = '\0';
}

//FOLLOW

void follow(char s) {
	if(s == nonterminals[0]) 
		add_symbol(1, FOLLOW[0], "$");

	else if(s == nonterminals[1])
		add_symbol(1, FOLLOW[1], "$");
	
	int i, j;
	for(i = 0; i < numOfProductions; i++) {
		for(j = 3; j < strlen(inputgrammar[i]); j++) {
			epsilon_flag = 0;

			if(inputgrammar[i][j] == s) {
				char next_sym = inputgrammar[i][j+1];

				if(next_sym != '\0') {		//If current symbol is not the last symbol of production body.
					if(isterminal(next_sym))	//For terminals.
						add_symbol(1, FOLLOW[get_pos(1, s)], FIRST[0][get_pos(0, next_sym)]);
					else {		//For non-terminals.
						add_symbol(1, FOLLOW[get_pos(1, s)], FIRST[1][get_pos(1, next_sym)]);

						if(epsilon_flag) {	//If FIRST[next_sym] has epsilon, find FOLLOW[next_sym].
							follow(next_sym);
							add_symbol(1, FOLLOW[get_pos(1, s)], FOLLOW[get_pos(1, next_sym)]);
						}
					}
				}

				else {		//If current symbol is the last symbol of production body.
					follow(inputgrammar[i][0]);	//Follow of production head.
					add_symbol(1, FOLLOW[get_pos(1, s)], FOLLOW[get_pos(1, inputgrammar[i][0])]);
				}
			}
		}
	}
}

void compute_follow() {
	int i;
	
	for(i = 0; i < nonterminalIndex; i++)
		follow(nonterminals[i]);

//	for(i = 0; i < nonterminalIndex; i++)
//		printf("%s\n", FOLLOW[get_pos(1, nonterminals[i])]);
}

struct Stack {	//Holds states.
	int states[100];
	int top;
} stack;

void push(int a) {
	stack.top++;
	stack.states[stack.top] = a;
}

void pop() {
	int a = stack.states[stack.top];
	stack.top--;
}

int get_top() {		//Returns top of stack state.
	return stack.states[stack.top];
}


void print(char arr[], int size){
    for(int i = 0; i < size;i++){
        printf("%c ", arr[i]);
    }
    printf("\n");
}

void findClosure(char arr[]){
    int i , j;
    for(i = 0; arr[i] != '.';i++);
    i++;
    if(!item_found(arr)){
        strcpy(slrItems[stateIndex][closureIndex], arr);
        closureIndex++;
    }

    if(arr[i] == arr[0] && arr[i-2] == '>')
        return ;
    if(isterminal(arr[i]))
        return ;
    else{
        for(j = 0; j < numOfProductions;j++){
            char temp[100];
            if(inputgrammar[j][0] == arr[i]){
                generateItems(inputgrammar[j], temp);
                closure(temp);
            }
        }
    }
}

void generateItems(char str[], char temp[]){
    int i;
    for(i = 0; i < 3; i++){
        temp[i] = str[i];
    }
    temp[i] = '.'; //Initializing slrItems.
    if(str[i] != '#')
        for( ; i < strlen(str);i++){
            temp[i+1] = str[i];
        }
    temp[i+1] = '\0';
}

void closureAndGoto(){
    int i , j;
    char currentString[20][20], transitionItems[20];
    int transitionIndex, gotoIndex;

    generateItems(inputgrammar[0], currentString[0]);
    findClosure(currentString[0]);

    numOfItems[stateIndex] = closureIndex;
    closureIndex = 0;
    stateIndex++;

    while(gotoIndex < 30){
        transitionIndex = 0;
        transitionItems[transitionIndex] = '\0';
        for(i = 0; i < numOfItems[gotoIndex]; i++) {
			for(j = 0; slrItems[gotoIndex][i][j] != '.'; j++);
			j++;

			if(!transition_item_found(transitionItems, slrItems[gotoIndex][i][j], transitionIndex)) {
				transitionItems[transitionIndex] = slrItems[gotoIndex][i][j];
				transitionIndex++;
			}
		}
        transitionItems[transitionIndex] = '\0';
        for(i = 0; i < transitionIndex; i++) {
			int add_flag = 0;
            char temp[100];

			gotoIndex = Goto1(transitionItems[i], temp);
	
			for(j = 0; j < gotoIndex; j++) {
				if(!state_found(temp[j])) {
					add_flag = 1;
					closure(temp[j]);
				}
				else
					break;
			}
			if(add_flag) {
				numOfItems[stateIndex] = closureIndex;
				closureIndex = 0;
				stateIndex++;
			}
		}
        numOfStates = stateIndex;
        gotoIndex++;
    }
}

void compute_first() {
	int i;

	for(i = 0; i < terminalIndex; i++)
		first(terminals[i]);
	
	for(i = 0; i < nonterminalIndex; i++)
		first(nonterminals[i]);

//	for(i = 0; i < nonterminalIndex; i++)
//		printf("%s\n", FIRST[1][get_pos(1, nonterminals[i])]);
}

int main(){
    printf("Enter number of productions : ");
    scanf("%d", &numOfProductions);

    printf("Input Format : \"S->AB\"\n");
    for(int i = 1; i <= numOfProductions; i++){
        printf("Prodution number %d : ", i);
        scanf("%s", inputgrammar[i]);
    }

    strcpy(inputgrammar[0], "Q->");
    char buffer[maxSize];
    buffer[0] = inputgrammar[1][0];
    buffer[1] = '\0';
    strcat(inputgrammar[0], buffer);
    numOfProductions++;

    for(int i = 0; i < numOfProductions;i++){
        printf("%s\n", inputgrammar[i]);
    }

    extractTerminalsAndNonterminals(numOfProductions);

    printf("Terminals : \n");
    print(terminals, terminalIndex);


    printf("Non terminals : \n");
    print(nonterminals, nonterminalIndex);



    return 0;
}