#include <cs50.h>
#include <stdio.h>

void draw_pyramid(int h);
void print_hashes(int x);
void print_spaces(int x);

int main(void)
{
    int height;
    
    //get hight of pyramid of 1-8
    do
    {
        height = get_int("Height: ");
    }
    while(height < 1 || height > 8); //if out of bounds loop
    
    draw_pyramid(height);
}

//draw pyramid
void draw_pyramid(int h)
{
    for (int i = 0; i < h; i++)
    {
        print_spaces(h-(i+1));
        print_hashes(i+1);
        printf("  ");
        print_hashes(i+1);
        printf("\n");
    }
}

// print specified num of hashes.
void print_hashes(int x)
{
    for (int i = 0; i < x; i++)
    {
        printf("#");
    }
}

// print specified num of blank spaces.
void print_spaces(int x)
{
    for (int i = 0; i < x; i++)
    {
        printf(" ");
    }
}
