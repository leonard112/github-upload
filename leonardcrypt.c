#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <crypt.h>

int main(void)
{
    char* hash = get_string("hash: ");
    hash = crypt(hash, "50");
    printf("hashed: %s: ", hash);
}
