#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TRANSITIONS 100

char transitions[MAX_TRANSITIONS][3];
int transition_index = 0;

void print(const char* s, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c ", s[i]);
    }
    printf("\n");
}

bool q2(const char* s, int i, int length);
bool q1(const char* s, int i, int length);
bool q0(const char* s, int i, int length);

bool q2(const char* s, int i, int length) {
    strcpy(transitions[transition_index++], "q1");
    if (i == length) {
        return true;
    }

    return true;
}

bool q1(const char* s, int i, int length) {
    strcpy(transitions[transition_index++], "q1");
    if (i == length) {
        return false;
    }

    return q2(s, i + 1, length);
}

bool q0(const char* s, int i, int length) {
    strcpy(transitions[transition_index++], "q0");
    if (i == length) {
        return false;
    }

    return q1(s, i + 1, length);

}

int main() {
    int length;
    scanf("%d", &length);

    char s[length];
    for (int i = 0; i < length; i++) {
        scanf(" %c", &s[i]);
    }

    print(s, length);

    transition_index = 0;

    bool ans = q0(s, 0, length);
    if (ans) {
        printf("YES\n");

        for (int i = 0; i < transition_index; i++) {
            printf("%s ", transitions[i]);
        }
        printf("\n");
    } else {
        printf("NO\n");
    }
    return 0;
}