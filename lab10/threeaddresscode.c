#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_SIZE 50

int top=-1;

char temporaryMem[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
int temporaryIndex = 0;

// char infixExpression[MAX_SIZE];
char STACK[MAX_SIZE];

int isEmpty()
{
    if (top == -1)
        return 1;
        
    else
        return 0;
}

int findPrecedence(char character)
{
    switch (character){
    case '^':
        return 3;
        
    case '/':
    case '*':
        return 2;
    
    case '+':
    case '-':
        return 1;
    
    default:
        return 0;
    }
}


char pop()
{
    char c;
    if (top == -1)
    {
        printf("Stack Underflow\n");
        exit(1);
    }
    c = STACK[top];
    top = top - 1;
    return c;
}

void push(char ch)
{
    if (top == MAX_SIZE - 1)
    {
        printf("Stack Overflow\n");
        return;
    }
    top++;
    STACK[top] = ch;
}

void print(char arr[])
{
    int i = 0;

    while (arr[i])
    {
        printf("%c", arr[i++]);
    }
    printf("\n");
}

char* convertInfixToPostfix(char infixExpression[])
{
    char* postfixExpression = malloc(MAX_SIZE * sizeof(char));
    int j = 0;
    char currentChar, next;
    for (int i = 0; i < strlen(infixExpression); i++)
    {
        currentChar = infixExpression[i];
        switch (currentChar)
        {
        case '(':
            push(currentChar);
            break;
        case ')':
            while ((next = pop()) != '(')
                postfixExpression[j++] = next;
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            while (!isEmpty() && findPrecedence(STACK[top]) >= findPrecedence(currentChar))
                postfixExpression[j++] = pop();
            push(currentChar);
            break;
        default:
            postfixExpression[j++] = currentChar;
        }
    }

    while (!isEmpty()){
        postfixExpression[j++] = pop();
    }
    postfixExpression[j] = '\0';
    
    return postfixExpression;
}

int removeSpaces(char* input){
    int index = 0;
    for(int i = 0; i < strlen(input);i++){
        if(input[i] == ' '){
            continue;
        }
        input[index++] = input[i];
    }
    input[index] = '\0';
    return index;
}

bool isOperator(char ch){
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
        return true;
    return false;
}

char* modifyString(char postfix[], int start, int end, char newChar){
    char *newString = malloc(MAX_SIZE * sizeof(char));
    strncpy(newString, postfix + 0, start);
    char ch2[] = {newChar, '\0'};
    strcat(newString, ch2);
    char * remainingString = malloc(MAX_SIZE * sizeof(char));
    strncpy(remainingString, postfix + end, strlen(postfix) - end);
    strcat(newString, remainingString);
    
    printf("Modified string is : %s\n", newString);
    return newString;
}

char* postfixToThreeAddress(char postfix[]){    // Do inplace modification
    // char *final = postfix;
    while(strlen(postfix) != 1){
        // printf("In while loop\n");
        // if(strlen(postfix) == 3 && strchr(postfix, '=') != NULL){
        //     break;
        // }
        for(int i = 0; i < strlen(postfix);i++){
            // printf("In for loop\n");
            if(isOperator(postfix[i])){
                printf("\t%c = %c %c %c\n", temporaryMem[temporaryIndex], postfix[i-2], postfix[i], postfix[i-1]);
                postfix = modifyString(postfix, i-2, i+1, temporaryMem[temporaryIndex]);
                i -= 2;
                temporaryIndex++;
            }
            // i-=2;
        }
    }
    return postfix;
}

int main(){
    
    char *input;
    bool flag = false;
    
    input = malloc(MAX_SIZE*sizeof(char));
    
    char *final;
    
    final = malloc(3*sizeof(char));
    
    printf("Enter a string with operators : ");
    fgets(input, MAX_SIZE, stdin);
    input[strlen(input) - 1] = '\0';
    
    printf("Input received is : %s\n", input);
    
    int len = removeSpaces(input);
    
    // input[len] = '\0';
    
    printf("After removing spaces : %s\n", input);
    
    char * p = strchr(input, '=');
    
    if(p != NULL){
        // printf("%s\n", p);
        int position = input - p + 2;
        // printf(" -- %d\n", position);
        strncpy(final, input + 0, position+1);
        // printf("FINAL IN IF : %s\n", final);
        strncpy(input, input + 2, strlen(input) - 2);
        input[strlen(input) - 2] = '\0';
        // printf("AFTER CUTTING : %s\n", input);
        flag = true;
    }
    
    
    // printf("Final string before appending : %s\n", final);
    
    char* postfixExpression = convertInfixToPostfix(input);
    printf("Postfix expression : %s\n", postfixExpression);
    printf("Postfix Length = %ld\n", strlen(postfixExpression));
    
    postfixExpression = postfixToThreeAddress(postfixExpression);
    printf("%s\n", postfixExpression);
    
    if(p != NULL)
        strcat(final, postfixExpression);
    
    if(!flag){
        printf("%s\n", postfixExpression);
    }else{
        printf("%s\n", final);
    }
    
    return 0;
}