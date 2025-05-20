#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int lookahead = 0;
char input[20];

// The following grammar has left recursion, convert it into right recursive grammar.
// 1. S → E  
// 2. E → E + T | E - T | T  
// 3. T → T * F | T / F | F  
// 4. F → ( E ) | - F | + F | N | V  
// 5. V → x | y | z  
// 6. N → D | D N  
// 7. D → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

void match(char );
void S();
void E();
void Eprime();
void T();
void Tprime();
void F();
void V();
void N();


void match(char ch){
    if(ch == input[lookahead]){
        printf("Character %c matched, moving forward!\n",ch);
        lookahead++;
            printf("%d\n",lookahead);

    }else{
        printf("Rejected\n");
        exit(1);
    }
}

void S(){
    E();
}

void E(){
    T();
    // lookahead++;
    printf("%d\n",lookahead);
    Eprime();
    // lookahead++;
    printf("%d\n",lookahead);

}

void Eprime(){
    if(input[lookahead] == '+'){
        match('+');
        // lookahead++;
            printf("%d\n",lookahead);

        T();
        Eprime();
    }else if(input[lookahead] == '-'){
        match('-');
        // lookahead++;
            printf("%d\n",lookahead);

        T();
        Eprime();
    }else{
        printf("ELSE\n");
        return ;
        // printf("Rejected\n");
        // exit(1);
    }
}


void T(){
    F();
    // lookahead++;
        printf("%d\n",lookahead);

    Tprime();
    // lookahead++;
        printf("%d\n",lookahead);

}

void Tprime(){
    if(input[lookahead] == '*'){
        match('*');
        // lookahead++;
            printf("%d\n",lookahead);

        F();
        Tprime();
    }else if(input[lookahead] == '/'){
        match('/');
        // lookahead++;
            printf("%d\n",lookahead);

        F();
        Tprime();
    }else if(input[lookahead] == '#'){
        return ;
    }else{
        printf("LOOKAHEAD + 1 : %c\n", input[lookahead+1]);
        if(input[lookahead+1] == '$'){
            printf("Rejected\n");
            exit(1);
        }else{
            printf("ELSE");
            return ;
        }
    }
}

void F(){
    if(input[lookahead] == '('){
        match('(');
        // lookahead++;
            printf("%d\n",lookahead);

        E();
        if(input[lookahead] == ')'){
            match(')');
            // lookahead++;
                printf("%d\n",lookahead);

        }
    }else if(input[lookahead] == '-'){
        match('-');
        // lookahead++;
            printf("%d\n",lookahead);

        F();
    }else if(input[lookahead] == '+'){
        match('+');
        // lookahead++;
            printf("%d\n",lookahead);

        F();
    }else if(input[lookahead] == 'x' || input[lookahead] == 'y' || input[lookahead] == 'z'){
        V();
    }else if(isdigit(input[lookahead])){
        N();
    }else{
        if(input[lookahead+1] == '$'){
            printf("ELSE\n");
            return ;
        }else{
            printf("Rejected\n");
            exit(1);
        }
    }
}

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

void N(){
    if(isdigit(input[lookahead])){
        match(input[lookahead]);
        // lookahead++;
            printf("%d\n",lookahead);

        N();
    }else if(input[lookahead] == '$'){
        return ;
    }else{
        printf("ELSE");
        return ;
        // printf("Rejected\n");
        // exit(1);
    }
}

void V(){
    if(input[lookahead] == 'x'){
        match('x');
        // lookahead++;
        printf("%d\n",lookahead);
        return ;

    }else if(input[lookahead] == 'y'){
        match('y');
        // lookahead++;
        printf("%d\n",lookahead);
        return ;
    }else if(input[lookahead] == 'z'){
        match('z');
        // lookahead++;
        printf("%d\n",lookahead);
        return ;
    }else{
        printf("ELSE");
        // return ;
        printf("Rejected\n");
        exit(1);
    }
}

int main(){
    printf("Enter string : ");
    scanf("%s", input);
    strcat(input, "$");
    printf("%s\n", input);
    S();
    printf("Look ahead : %c\n", input[lookahead]);
    printf("%d\n",lookahead);
    if(input[lookahead] == '$')
        printf("Accepted\n");
    else
        printf("Rejected\n");
    return 0;
}