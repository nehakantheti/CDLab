//Triples&Indirect triple
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define size 30

char expr[size];
char stack[size];
char postfix[size];
int top = -1;

typedef struct
{
    int index;
    char operator;
    char arg1;
    char arg2;
}Triples;

Triples triples[10];
int triples_index = 0;

int precedence(char symbol)
{
    switch (symbol)
    {
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

void push(char c)
{
    if (top == size - 1)
    {
        printf("Stack Overflow\n");
        return;
    }
    top++;
    stack[top] = c;
}

char pop()
{
    char c;
    if (top == -1)
    {
        printf("Stack Underflow\n");
        exit(1);
    }
    c = stack[top];
    top = top - 1;
    return c;
}

int isEmpty()
{
    return top == -1;
}

void inToPost()
{
    int j = 0;
    char symbol, next;
    for (int i = 0; i < strlen(expr); i++)
    {
        symbol = expr[i];
        switch (symbol)
        {
        case '(':
            push(symbol);
            break;
        case ')':
            while ((next = pop()) != '(')
                postfix[j++] = next;
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            while (!isEmpty() && precedence(stack[top]) >= precedence(symbol))
                postfix[j++] = pop();
            push(symbol);
            break;
        default:
            postfix[j++] = symbol;
        }
    }

    while (!isEmpty())
        postfix[j++] = pop();
    postfix[j] = '\0';
}

void replaceSubstring(char *postfix, const char *oldSubstr, const char *newSubstr) 
{
    char buffer[size];
    char *pos;
    
    pos = strstr(postfix, oldSubstr);
    
    if (pos != NULL) 
    {
        int index = pos - postfix;
        strncpy(buffer, postfix, index);
        buffer[index] = '\0';
        strcat(buffer, newSubstr);
        strcat(buffer, pos + strlen(oldSubstr));
        strcpy(postfix, buffer);
    }
}

void get_triples()
{
    inToPost();
    printf("Postfix is %s\n",postfix);
    int index = 1;
    while(strlen(postfix) > 3)
    {
        for(int i=2; i<strlen(postfix); i++)
        {
            if(!isalnum(postfix[i]))
            {
                if(postfix[i] == '@')
                {
                    triples[triples_index].index = index;
                    triples[triples_index].operator = '*';
                    triples[triples_index].arg1 = postfix[i+1];
                    char change[] = {postfix[i], postfix[i+1],'\0'};
                    char replace[5];
                    snprintf(replace,5, "%d", index);
                    replaceSubstring(postfix,change,replace);
                    printf("replaced postfix %s\n",postfix);
                    index++;
                    triples_index++;
                    i -= 1;
                }
                else if(postfix[i] == '[')
                {
                    triples[triples_index].index = index;
                    triples[triples_index].operator = 'i';
                    triples[triples_index].arg1 = postfix[i-1];
                    triples[triples_index].arg2 = postfix[i+1];
                    char change[] = {postfix[i-1],postfix[i], postfix[i+1],postfix[i+2],'\0'};
                    char replace[5];
                    snprintf(replace,5, "%d", index);
                    replaceSubstring(postfix,change,replace);
                    printf("replaced postfix %s\n",postfix);
                    index++;
                    triples_index++;
                    i -= 2;
                }
                else
                {
                    triples[triples_index].index = index;
                    triples[triples_index].arg1 = postfix[i-2];
                    triples[triples_index].arg2 = postfix[i-1];
                    triples[triples_index].operator = postfix[i];
                    char change[] = {postfix[i-2], postfix[i-1], postfix[i],'\0'};
                    char replace[5];
                    snprintf(replace,5, "%d", index);
                    replaceSubstring(postfix,change,replace);
                    printf("replaced postfix %s\n",postfix);
                    index++;
                    triples_index++;
                    i -= 2;
                } 
            }
        }
    }
    triples[triples_index].index = index;
    triples[triples_index].arg1 = postfix[0];
    triples[triples_index].arg2 = (index-1) + '0';
    triples[triples_index].operator = '=';
    triples_index++;
}

void print_triples() 
{
    printf("\nTriples : \n");
    printf("Index\tOperation\tArg1\tArg2\n");
    for (int i = 0; i < triples_index; i++) 
    {
        printf("%d\t%c\t\t%c\t%c\n", triples[i].index, triples[i].operator, triples[i].arg1, triples[i].arg2);
    }
}

void print_indirect()
{
    int addr = 100;
    printf("\nIndirect Triples : \n");
    printf("Address\tTriple Index\n");
    for (int i = 0; i < triples_index; i++) 
    {
        printf("%d\t%d\n", addr++, triples[i].index);
    }
}

int main()
{
    printf("Give the expression : ");
    scanf("%s",expr);
    get_triples();
    print_triples();
    print_indirect();
    return 0;
}