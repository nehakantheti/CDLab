#include <ctype.h>
#include <stdio.h>
#include <string.h>

void findfirst(char, int, int);

int count, n = 0;
char calc_first[10][100];


char calc_follow[10][100];
int m = 0;

char g[10][10];
char f[10], first[10];
int k;
char ck;
int e;

int main()
{
    int jm = 0;
    int km = 0;
    int i, choice;
    char c, ch;
    count = 4;
    strcpy(g[0], "S=AaBb");
    strcpy(g[1], "S=BbBa");
    strcpy(g[2], "A=#");
    strcpy(g[3], "B=#");



    int kay;
    char done[count];
    int ptr = -1;
    for (k = 0; k < count; k++) {
        for (kay = 0; kay < 100; kay++) {
            calc_first[k][kay] = '!';
        }
    }

    int p1 = 0, p2, check;

    for (k = 0; k < count; k++) {
        c = g[k][0];
        p2 = 0;
        check = 0;
        for (kay = 0; kay <= ptr; kay++)
            if (c == done[kay])
                check = 1;

        if (check == 1)
            continue;

        findfirst(c, 0, 0);
        ptr += 1;

        done[ptr] = c;
        printf("\n First(%c) = { ", c);
        calc_first[p1][p2++] = c;

        for (i = 0 + jm; i < n; i++) {
            int lark = 0, chk = 0;

            for (lark = 0; lark < p2; lark++) {

                if (first[i] == calc_first[p1][lark]) {
                    chk = 1;
                    break;
                }
            }
            if (chk == 0) {
                printf("%c, ", first[i]);
                calc_first[p1][p2++] = first[i];
            }
        }
        printf("}\n");
        jm = n;
        p1++;
    }
    printf("\n");
}
void findfirst(char c, int q1, int q2)
{
    int j;

    if (!(isupper(c))) {
        first[n++] = c;
    }
    for (j = 0; j < count; j++) {
        if (g[j][0] == c) {
            if (g[j][2] == '#') {
                if (g[q1][q2] == '\0')
                    first[n++] = '#';
                else if (g[q1][q2] != '\0'
                        && (q1 != 0 || q2 != 0)) {
                    findfirst(g[q1][q2], q1,
                            (q2 + 1));
                }
                else
                    first[n++] = '#';
            }
            else if (!isupper(g[j][2])) {
                first[n++] = g[j][2];
            }
            else {
                findfirst(g[j][2], j, 3);
            }
        }
    }
}