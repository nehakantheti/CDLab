#include <stdio.h>
#include <ctype.h>

#define MAXLINE 100

void countCharacters(const char *filename, int *lines, int *words, int *characters, int *vowels, int *consonants, int *digits, int *others) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    int ch;
    int inWord = 0;

    *lines = *words = *characters = *vowels = *consonants = *digits = *others = 0;

    while ((ch = fgetc(file)) != EOF) {
        (*characters)++;
        
        if (ch == '\n') {
            (*lines)++;
        }

        if (isspace(ch)) {
            if (inWord) {
                (*words)++;
                inWord = 0;
            }
        } else {
            inWord = 1;

            if (isalpha(ch)) {
                if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' || ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') {
                    (*vowels)++;
                } else {
                    (*consonants)++;
                }
            } else if (isdigit(ch)) {
                (*digits)++;
            } else {
                (*others)++;
            }
        }
    }

    if (inWord) {
        (*words)++;
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int lines, words, characters, vowels, consonants, digits, others;
    countCharacters(argv[1], &lines, &words, &characters, &vowels, &consonants, &digits, &others);

    printf("Lines: %d\n", lines+1);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", characters);
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    printf("Digits: %d\n", digits);
    printf("Others: %d\n", others);
    
    // Check that the total count of characters matches
    int total = vowels + consonants + digits + others;
    // printf("Total : %d\n", total);
    // if (total != characters) {
    //     printf("Warning: Total character count mismatch.\n");
    // }

    return 0;
}
