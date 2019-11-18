#include <cs50.h>
#include <stdio.h>
#include <string.h>

char* vigenere_cyper(char* plain_text, char* key);
bool has_no_numbers(char* str);

//encrypts a string using vigenere encryption.
int main(int argc, string argv[])
{
    if (argc != 2) // make sure a command line argument is passed.
    {
        printf("ERROR! vigenere.c must take exactly one command line argument.\n");
        return 1;
    }
    if (!has_no_numbers(argv[1])) //if command line argument has num characters.
    {
        printf("ERROR! vigenere.c key argument must be composed of letters.\n");
        return 1;
    }
    
    char* plain_text;
    
    do
    {
        plain_text = get_string("Plaintext: ");
    }
    while(!has_no_numbers(plain_text)); //if user inputs number characters, loop.
    
    printf("ciphertext: %s\n", vigenere_cyper(plain_text, argv[1]));
    
    return 0;
}

//cyper using vigenere
char* vigenere_cyper(char* plain_text, char* key)
{
    int offset; //stores the value to offset char inside plain_text
    int key_index = 0; //stores the index of the key
    for (int i = 0; i < strlen(plain_text); i++) //iterate through plain_text
    {
        //if character is a letter.
        if ((plain_text[i] >= 65 && plain_text[i] <= 90) ||
           (plain_text[i] >= 97 && plain_text[i] <= 122))
        {
            if (key[key_index] > 90) //if key is lowercase
            {
                offset = key[key_index] - 97; //set offset.
            }
            else //if key is uppercase
            {
                offset = key[key_index] - 65; //set offset.
            }
            
            if (plain_text[i] > 90) //if plain_text is lowercase
            {
                if (plain_text[i] + offset > 122) //if out of bounds.
                {
                    offset -= 26; //adjust for out of bounds.
                }
            }
            else //if plain_text is uppercase
            {
                if (plain_text[i] + offset > 90) //if out of bounds.
                {
                    offset -= 26; //adjust for out of bounds.
                }
            }
            
            plain_text[i] = plain_text[i] + offset; //add offset.
            
            if (key_index == strlen(key)-1) //if cylcle thorugh keyword.
            {
                key_index = 0; //set to 0
            }
            else
            {
                key_index++; //increment
            }
        }
    }
    return plain_text;
}

//see if string has not number characters.
bool has_no_numbers(char* str)
{
    for (int i = 0; i < strlen(str); i++) //for each character
    {
        //is the character a number?
        if (str[i] >= 48 && str[i] <= 57)
        {
            return false;
        }
    }
    return true;
}
