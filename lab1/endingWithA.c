#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TRANSITIONS 100

char transitions[MAX_TRANSITIONS][3];
int transition_index = 0;

bool q1(const char *s, int i);
bool q0(const char *s, int i);
bool reject(const char *s, int i);

void print(char *s)
{
    for (int i = 0; i < 5; i++)
    {
        printf("%c ", s[i]);
    }
}

bool q1(const char *s, int i)
{
    strcpy(transitions[transition_index++], "q1");
    if (i == strlen(s))
    {
        return true;
    }

    if (s[i] == 'a')
    {
        return q1(s, i + 1);
    }
}

bool q0(const char *s, int i)
{
    strcpy(transitions[transition_index++], "q0");
    if (i == strlen(s)-1)
    {
        if (s[i] == 'a')
        {
            return q1(s, i + 1);
        }
        return false;
    }

    return q0(s, i + 1);
}

int main()
{
    int length;
    printf("Enter length of string : ");
    scanf("%d", &length);
    char s[length];
    for (int i = 0; i < length; i++)
    {
        scanf(" %c", &s[i]);
    }

    // print(s);

    transition_index = 0;

    bool ans = q0(s, 0);
    if (ans)
    {
        printf("YES\n");

        for (int i = 0; i < transition_index; i++)
        {
            printf("%s ", transitions[i]);
        }
        printf("\n");
    }
    else
    {
        printf("NO\n");
    }
    return 0;
}