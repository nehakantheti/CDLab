#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX 10


char firstSetdash[MAX];
char firstSet[MAX];
int setCount = 0;
int count = 0;


int num_Productions;
char productionSet[MAX][MAX];
int visited2[200];
char followset[MAX];
int followcount = 0;
int visited3[200];


void findFirstSet(char c, int c1, int c2)
{
    if (visited2[c] == 0)
    {
        visited2[c] = 1;
        if (!(c >= 65 && c <= 90))
        {
            firstSetdash[setCount++] = c;
            return;
        }


        for (int i = 0; i < num_Productions; i++)


        {


            if (productionSet[i][0] == c)
            {
                if (productionSet[i][2] == '#')
                {
                    if (productionSet[c1][c2] == '\0')
                    {
                        firstSetdash[setCount++] = '#';
                    }
                    else if (productionSet[c1][c2] != '\0' && (c1 != 0 || c2 != 0))
                    {
                        findFirstSet(productionSet[c1][c2], c1, c2 + 1);
                    }
                    else
                    {
                        firstSetdash[setCount++] = '#';
                    }
                }
                else if (!(productionSet[i][2] >= 65 && productionSet[i][2] <= 90))
                {
                    firstSetdash[setCount++] = productionSet[i][2];
                }
                else
                {
                    if (visited2[productionSet[i][2]] == 0)
                    {
                        findFirstSet(productionSet[i][2], i, 3);
                    }
                }
            }
        }
    }
    return;
}


void useFirstSet(char c)
{
    setCount = 0;
    count = 0;
    findFirstSet(c, 0, 0);


    int visited[200];
    for (int i = 0; i < 200; i++)
    {
        visited[i] = 0;
    }


    for (int i = 0; i < setCount; i++)
    {
        if (visited[firstSetdash[i]] == 0)
        {
            firstSet[count++] = firstSetdash[i];
            visited[firstSetdash[i]] = 1;
        }
    }
}


bool epsilonPresence(char c)
{
    // useFirstSet(c);
    for (int i = 0; i < count; i++)
    {
        if (firstSet[i] == '#')
        {
            return true;
        }
    }


    return false;
}


void findFollowSet(char c)
{


    for (int i = 0; i < num_Productions; i++)
    {
        int flag = 0;
        for (int j = 2; productionSet[i][j] != '\0'; j++)
        {


            if (c == productionSet[0][0] && visited3['$'] == 0)
            {
                printf("%c ", c);
                printf("%c ", productionSet[i][j+1]);
                printf("\n\n");
                followset[followcount++] = '$';
                visited3['$'] = 1;
            }


            if (productionSet[i][j] == c || flag)
            {


                flag = 1;
                if (!(productionSet[i][j + 1] >= 65 && productionSet[i][j + 1] <= 90) && productionSet[i][j + 1] != '\0')
                {
                    if(visited3[productionSet[i][j+1]] == 0){
                        followset[followcount++] = productionSet[i][j + 1];
                        visited3[productionSet[i][j+1]] = 1;
                    }
                    break;
                }
                else
                {


                    if (flag && productionSet[i][j + 1] != '\0')
                    {


                        useFirstSet(productionSet[i][j + 1]);


                        if (epsilonPresence(productionSet[i][j + 1]))
                        {


                            for (int k = 0; k < count; k++)
                            {
                                if (firstSet[k] != '#' && visited3[firstSet[k]] == 0)
                                    followset[followcount++] = firstSet[k];
                                visited3[firstSet[k]] = 1;
                            }
                            continue;
                        }
                        else
                        {


                            for (int k = 0; k < count; k++)
                            {
                                if (visited3[firstSet[k]] == 0)
                                {
                                    followset[followcount++] = firstSet[k];
                                    printf("%c ", firstSet[k]);
                                    flag = 0;
                                    visited3[firstSet[k]] = 1;
                                }
                            }


                            printf("\n\n");
                        }
                    }


                    if (productionSet[i][j + 1] == '\0' && productionSet[i][0] != c)
                    {
                        findFollowSet(productionSet[i][0]);
                    }
                }
            }
        }
    }
}


int main()
{


    /*Rules assumed for this program:
    1. Every terminal should be non-capital and only single character.
    2. Non-terminals should be in capital
    3. Production input should be as A=aBb
    4. Epsilon is represented by '#'   */


    printf("Enter number of productions: ");
    scanf("%d", &num_Productions);


    printf("\n\nEnter productions:\n");
    for (int i = 0; i < num_Productions; i++)
    {
        scanf("%s", productionSet[i]);
    }


    char c;
    printf("\n\nEnter the character: ");
    scanf(" %c", &c);


    for (int i = 0; i < 200; i++)
    {
        visited2[i] = 0;
    }


    for (int i = 0; i < 200; i++)
    {
        visited3[i] = 0;
    }


    findFollowSet(c);


    for (int i = 0; i < followcount; i++)
    {
        printf("%c ", followset[i]);
    }


    return 0;
}