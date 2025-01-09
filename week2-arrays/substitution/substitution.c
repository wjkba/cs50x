#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // sprawdzam czy jest alfanumeryczny i czy sie powtarza
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
            return 1;

        for (int j = i + 1; j < 26; j++)
        {
            if (toupper(key[i]) == toupper(key[j]))
                return 1;
        }
    }

    // pusty array wielkosci 26
    char keyChars[26];

    int keyLength = strlen(key);

    if (keyLength == 26)
    {
        string plainText = get_string("plaintext: ");
        char cipherText[strlen(plainText) + 1];

        // populujemy array kazda literka z klucza
        for (int i = 0; i < keyLength; i++)
        {
            keyChars[i] = toupper(key[i]);
        }

        //
        printf("ciphertext: ");
        for (int i = 0, n = strlen(plainText); i < n; i++)
        {
            char c = plainText[i];
            if (isalpha(c))
            {
                int index = toupper(c) - 'A';
                if (isupper(c))
                    printf("%c", keyChars[index]);
                else
                    printf("%c", tolower(keyChars[index]));
            }
            else
                printf("%c", c);
        }

        printf("\n");
    }
}
