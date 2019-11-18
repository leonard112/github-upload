#include <cs50.h>
#include <stdio.h>

void check_card(long card_num);

int main(void)
{
    long card_num = get_long("Nubmer: ");
    check_card(card_num);
}

//check to see if card number is valid.
void check_card(long card_num)
{
    //store sums of the products every other digit starting from end.
    int first_sum = 0; 
    //store the each digit muliplied by 2 for the first sum.
    int product; 
    //if product is larger than 9, break into digits.
    int tens;
    int ones;
    
    //find first sum.
    for (long i = card_num / 10; i > 0; i /= 100)
    {
        product = (i % 10) * 2;
        
        if (product > 9)
        {
            tens = product / 10;
            ones = product % 10;
            first_sum += (tens + ones);
        }
        else
        {
            first_sum += product;
        }
    }
    
    //store the sum of remaining digits.
    int second_sum = 0;
    
    for (long i = card_num; i > 0; i /= 100)
    {
        second_sum += i % 10;
    }
    
    int total = first_sum + second_sum;
    
    if (total % 10 == 0) //if second digit is 0
    {
        if (card_num / 10000000000000 == 34 || card_num / 10000000000000 == 37)
        {
            printf("AMEX\n");
        }
        else if (card_num / 100000000000000 == 51 || 
                 card_num / 100000000000000 == 52 ||
                 card_num / 100000000000000 == 53 || 
                 card_num / 100000000000000 == 54 ||
                 card_num / 100000000000000 == 55)
        {
            printf("MASTERCARD\n");
        }
        else if (card_num / 1000000000000 == 4 || card_num / 1000000000000000 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
