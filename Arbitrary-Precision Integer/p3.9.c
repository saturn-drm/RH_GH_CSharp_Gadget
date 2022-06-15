#include <stdio.h>
#include <stdlib.h>
#include "big_integer.h"


int main(void)
{
    // bi1 = 2*10^0, const
    BigInt bi1 = malloc(sizeof(struct Node));
    bi1->Next = NULL;

    BigInt bires = malloc(sizeof(struct Node));
    bires->Next = NULL;

    int bi1_c[1] = {2};
    int bi1_e[1] = {0};

    BigIntConstructor(bi1_c, bi1_e, 1, bi1);
    BigIntConstructor(bi1_c, bi1_e, 1, bires);

    printf("BigInt 1:\n");
    PrintBigInt(bi1);

    printf("START: file %s, line %d, date %s, time %s\n", __FILE__, __LINE__, __DATE__, __TIME__);

    int cnt = 1;
    while (cnt < 4000)
    {
        BigInt bitmp = bires;
        bires = BigIntMulti(bi1, bitmp);
        DeleteBigInt(bitmp);
        cnt++;
    }

    printf("END: file %s, line %d, date %s, time %s\n", __FILE__, __LINE__, __DATE__, __TIME__);

    printf("BigInt multiplied:\n");
    PrintBigInt(bires);

    int digit_counts[10] = {0};
    int n_digits = 10;

    CountDigits(bires, digit_counts, n_digits);
    for (int i = 0; i < 10; i++)
        printf("count of digit %d: %d\n", i, digit_counts[i]);

    DeleteBigInt(bi1);
    DeleteBigInt(bires);

    return 0;
}

