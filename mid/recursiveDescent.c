#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define maxsize 20
char input[maxsize];
int lookahead = 0;

//Corresponding right recursive grammar
// 1. S → E  
// 2. E → TE'
// 3. E' → +TE' | -TE' | #
// 4. T → FT'
// 5. T' → *FT' | /FT' | #
// 6. F → (E) | -F | +F | N | V
// 7. V → x | y | z
// 8. N → D | DN
// 9. D → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

void match(char expected);
void S();
void E();
void Eprime();
void T();
void Tprime();
void F();
void V();
void N();

void match(char expected) {
    if (input[lookahead] == expected) {
        lookahead++;
    } else {
        printf("Rejected\n");
        exit(1);
    }
}

void S() {
    E();
}

void E() {
    T();
    Eprime();
}

void Eprime() {
    if (input[lookahead] == '+') {
        printf("Lookahead = %d\n", lookahead);
        match('+');
        T();
        Eprime();
    } else if (input[lookahead] == '-') {
        printf("Lookahead = %d\n", lookahead);
        match('-');
        T();
        Eprime();
    }
}

void T() {
    F();
    Tprime();
}

void Tprime() {
    if (input[lookahead] == '*') {
        printf("Lookahead = %d\n", lookahead);
        match('*');
        F();
        Tprime();
    } else if (input[lookahead] == '/') {
        printf("Lookahead = %d\n", lookahead);
        match('/');
        F();
        Tprime();
    }
}

void F() {
    if (input[lookahead] == '(') {
        match('(');
        E();
        match(')');
    } else if (input[lookahead] == '-') {
        match('-');
        F();
    } else if (input[lookahead] == '+') {
        match('+');
        F();
    } else if (isdigit(input[lookahead])) {
        N();
    } else if (input[lookahead] == 'x' || input[lookahead] == 'y' || input[lookahead] == 'z') {
        V();
    } else {
        printf("Rejected in F\n");
        exit(1);
    }
}

void N() {
    if (isdigit(input[lookahead])) {
        match(input[lookahead]);  // Match the current digit
        if (isdigit(input[lookahead])) {
            N();
        }
    } else {
        printf("Rejected in N\n");
        exit(1);
    }
}

void V() {
    if (input[lookahead] == 'x' || input[lookahead] == 'y' || input[lookahead] == 'z') {
        match(input[lookahead]);  // Match the current variable
    } else {
        printf("Rejected in  V\n");
        exit(1);
    }
}

int main() {
    printf("Give string ending with $ : \n");
    scanf("%s", input); // Removed '\n' for correct input handling
    S();
    if (input[lookahead] == '$') {
        printf("Accepted\n");
    } else {
        printf("Rejected in main\n");
    }
    return 0;
}
