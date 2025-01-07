#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

int get_string_length(string s);
int get_string_score(string s);
int punkty[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    string p1 = get_string("Player 1: ");
    string p2 = get_string("Player 2: ");
    int p1_score = get_string_score(p1);
    int p2_score = get_string_score(p2);
    // printf("%i\n",p1_score);
    // printf("%i\n",p2_score);

    if (p1_score > p2_score)
        printf("Player 1 wins!\n");
    if (p1_score < p2_score)
        printf("Player 2 wins!\n");
    if (p1_score == p2_score)
        printf("Tie!\n");
}

int get_string_length(string s)
{
    int n = 0;
    while (s[n] != '\0')
    {
        n++;
    }
    return n;
}

int get_string_score(string s)
{
    int score = 0;
    int string_length = get_string_length(s);
    for (int i = 0; i < string_length; i++)
    {
        char c = toupper(s[i]);
        if (isalpha(c))
        {
            score += punkty[c - 'A'];
        }
    }
    return score;
}
