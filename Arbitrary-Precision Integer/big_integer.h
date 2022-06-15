#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _Big_integer_H

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode BigInt;
typedef PtrToNode Position;

int IsLast(Position P, BigInt BI);
void Insert(int c, int e, BigInt BI, Position P);
void PrintBigInt(BigInt BI);
void BigIntConstructor(int *C, int *E, int N, BigInt BI);
BigInt BigIntAdd(BigInt BI1, BigInt BI2);
BigInt BigIntMulti(BigInt BI1, BigInt BI2);
void FormatBigInt(BigInt BI);
void CountDigits(BigInt BI, int *counts, int n_digits);
void DeleteBigInt(BigInt BI);

#endif

struct Node
{
    int Coefficient;
    int Exponent;
    PtrToNode Previous;
    PtrToNode Next;
};

int IsLast(Position P, BigInt BI)
{
    return P->Next == NULL;
}

void Insert(int c, int e, BigInt BI, Position P)
{
    Position tmp;
    tmp = (Position)malloc(sizeof(struct Node));
    if (tmp == NULL)
    {
        printf("Out of space!!!");
        exit(EXIT_FAILURE);
    }
    tmp->Coefficient = c;
    tmp->Exponent = e;
    tmp->Next = P->Next;
    tmp->Previous = P;

    P->Next = tmp;
    if (!IsLast(tmp, BI))
        tmp->Next->Previous = tmp;
}

void PrintBigInt(BigInt BI)
{
    Position P = BI;
    if (IsLast(P, BI))
        return;

    int E = P->Next->Exponent;
    while (E >= 0 && !IsLast(P, BI))
    {
        if (P->Next->Exponent == E)
        {
            printf("%d_", P->Next->Coefficient);
            P = P->Next;
        }
        else
            printf("0_");
        E--;
    }
    printf("\n");
}

void BigIntConstructor(int *C, int *E, int N, BigInt BI)
{
    Position P = BI;
    while (!IsLast(P, BI))
        P = P->Next;

    for (int i = 0; i < N; i++)
    {
        Position tmp;
        tmp = (Position)malloc(sizeof(struct Node));
        if (tmp == NULL)
        {
            printf("Out of space!!!");
            exit(EXIT_FAILURE);
        }

        tmp->Coefficient = *(C + i);
        tmp->Exponent = *(E + i);
        tmp->Next = NULL;
        tmp->Previous = P;

        P->Next = tmp;
        P = P->Next;
    }
}

BigInt BigIntAdd(BigInt BI1, BigInt BI2)
{
    BigInt BI = (BigInt)malloc(sizeof(struct Node));
    BI->Next = NULL;

    Position P1 = BI1->Next;
    Position P2 = BI2->Next;
    Position P = BI;

    while (P1 != NULL && P2 != NULL)
    {
        if (P1->Exponent < P2->Exponent)
        {
            Insert(P2->Coefficient, P2->Exponent, BI, P);
            P2 = P2->Next;
        }
        else if (P1->Exponent > P2->Exponent)
        {
            Insert(P1->Coefficient, P1->Exponent, BI, P);
            P1 = P1->Next;
        }
        else
        {
            Insert(P1->Coefficient + P2->Coefficient, P1->Exponent, BI, P);
            P1 = P1->Next;
            P2 = P2->Next;
        }
        P = P->Next;
    }

    while (P1 != NULL)
    {
        Insert(P1->Coefficient, P1->Exponent, BI, P);
        P1 = P1->Next;
        P = P->Next;
    }

    while (P2 != NULL)
    {
        Insert(P2->Coefficient, P2->Exponent, BI, P);
        P2 = P2->Next;
        P = P->Next;
    }

    return BI;
}

BigInt BigIntMulti(BigInt BI1, BigInt BI2)
{
    BigInt BI = (BigInt)malloc(sizeof(struct Node));
    BI->Next = NULL;

    Position P1 = BI1->Next;
    Position P2 = BI2->Next;

    while (P1 != NULL)
    {
        BigInt TMP = (BigInt)malloc(sizeof(struct Node));
        TMP->Next = NULL;
        Position TMPP = TMP;
        BigInt BIadd = BI;

        while (P2 != NULL)
        {
            // printf("%s %d %s %s\n", __FILE__, __LINE__, __DATE__, __TIME__);
            Insert(P1->Coefficient * P2->Coefficient, P1->Exponent + P2->Exponent, TMP, TMPP);
            TMPP = TMPP->Next;
            P2 = P2->Next;
            // printf("%s %d %s %s\n", __FILE__, __LINE__, __DATE__, __TIME__);
        }

        BI = BigIntAdd(BIadd, TMP);
        P1 = P1->Next;
        P2 = BI2->Next;
        DeleteBigInt(TMP);
        DeleteBigInt(BIadd);
        FormatBigInt(BI);
    }

    return BI;
}

void FormatBigInt(BigInt BI)
{
    // go from the end of the list, carry the digits
    Position P = BI;
    while (!IsLast(P, BI))
        P = P->Next;

    // now P is the last node
    while (P->Previous != BI)
    {
        if (P->Coefficient >= 10 && P->Previous->Exponent > P->Exponent + 1)
            Insert(0, P->Exponent + 1, BI, P->Previous);
        P->Previous->Coefficient += P->Coefficient / 10;
        P->Coefficient %= 10;
        P = P->Previous;
    }

    // deal with the first digit
    if (P->Coefficient >= 10)
    {
        Insert(0, P->Exponent + 1, BI, P->Previous);
        P->Previous->Coefficient += P->Coefficient / 10;
        P->Coefficient %= 10;
    }
}

void CountDigits(BigInt BI, int *counts, int n_digits)
{
    Position P = BI->Next;
    while (P != NULL)
    {
        *(counts + P->Coefficient) += 1;
        P = P->Next;
    }
}

void DeleteBigInt(BigInt BI)
{
    Position P, tmp;
    P = BI->Next;
    BI->Next = NULL;
    while (P != NULL)
    {
        tmp = P->Next;
        free(P);
        P = tmp;
    }
}
