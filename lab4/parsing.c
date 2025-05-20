#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define max 100

int characterCount = 0;
int wordCount = 0;
int vowelCount = 0;
int consonantCount = 0;

int main(int argc, char* args[]){
    FILE* file;
    char input[max];
    // printf("%s", args[1]);
    fopen(args[1], "r");
    int ch;
    while((ch = fgetc(file)) != EOF){
        char* token = strtok(input, "   ");
        while(token != NULL){
            printf("%s\n", token);
        }
    }
    return 0;
}