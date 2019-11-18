#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

char* crack_hash(char* hash, char* temp, char* test_hash);
char* increment_letter(char*, int index, int size);

int main(int argc, string argv[])
{
    
    
    //use to check all combinations of letters up to 5 characters.
    char* temp = malloc(6); 
    //temporary hash created from temp to compare to inputted hash.
    char* test_hash = malloc(14); 
    
    char* result = crack_hash(argv[1], temp, test_hash); // crack hash.
    printf("%s\n", result); // display result.
    free(result);
    free(temp);
    free(test_hash);
    
    return 0;
}

//crack a hash
char* crack_hash(char* hash, char* temp, char* test_hash)
{
    while (strcmp(temp,"zzzzz") != 0) //if all possiblities not tested.
    {
        increment_letter(temp, 0, 5); //increment letter.
        test_hash = crypt(temp, "50"); //hash temp
        if (strcmp(test_hash,hash) == 0) //compare test_hash to inputted hash.
        {
            return temp; //if equal, return temp
        }
    }
    return "Unable to crack hash."; //if unable to crack.
}

//increment the letters in a string by one character.
char* increment_letter(char* str, int index, int size)
{
    if (str[index] != 'z') //if haven't reached highest character.
    {
        if (str[index] == 0) //if character not set.
        {
            str[index] = 'A'; //set to first character
            return str;
        }
        else if (str[index] == 'Z') //if end of cap letters reached
        {
            str[index] = 'a'; //set character to first lowercase letter.
            return str;
        }
        else
        {
            str[index]++; //im
            return str;
        }
    }
    else if (index+1 == size)
    {
        printf("Buffer full, cannot add letters.");
        return str;
    }
    else
    {
        str[index] = 'A';
        return increment_letter(str, index+1, size);
    }
}
