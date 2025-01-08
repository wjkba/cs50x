#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int getLetterCount(string text, int textLength);
int getWordCount(string text, int textLength);
int getSentenceCount(string text, int textLength);

int main(void)
{
    string text = get_string("Text: ");
    int textLength = strlen(text);
    // printf("%s\n",text);
    int letters = getLetterCount(text, textLength);
    int words = getWordCount(text, textLength);
    int sentences = getSentenceCount(text, textLength);
    // printf("letters: %i\n words: %i\n sentences: %i\n", letters,words,sentences);

    float L = ((float) letters / words) * 100;
    float S = ((float) sentences / words) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    // printf("grade: %i\n", grade);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    if (grade >= 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    printf("Grade %i\n", grade);
}

int getLetterCount(string text, int textLength)
{
    int letterCount = 0;
    for (int i = 0; i < textLength; i++)
    {
        if (isalpha(text[i]))
        {
            letterCount++;
        }
    }
    return letterCount;
}

int getWordCount(string text, int textLength)
{
    int wordCount = 1;
    for (int i = 0; i < textLength; i++)
    {
        if (text[i] == ' ')
        {
            wordCount++;
        }
    }
    return wordCount;
}

int getSentenceCount(string text, int textLength)
{
    int sentenceCount = 0;
    for (int i = 0; i < textLength; i++)
    {
        if (text[i] == '.' || text[i] == '?')
        {
            sentenceCount++;
        }
    }
    return sentenceCount;
}
