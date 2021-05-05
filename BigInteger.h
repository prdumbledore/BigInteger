#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#include "Digits.h"

#define BigInteger BigInteger

typedef enum {positive = 0, negative = 1} Sign;

typedef struct {
    Digits *digits;
    Sign sign;
} BigInteger;

typedef struct MultiplicationTable {
    BigInteger values[10];
} MultiplicationTable;

BigInteger stringToBint(char*);
BigInteger intToBint(int);
char* bintToString(BigInteger);
void printBint(BigInteger);
void printlnBint(BigInteger);

BigInteger bintAdd(BigInteger, BigInteger);            /* a + b */
BigInteger bintSub(BigInteger, BigInteger);            /* a - b */
BigInteger bintMul(BigInteger, BigInteger);            /* a * b */
BigInteger bintDiv(BigInteger, BigInteger);            /* a / b */
BigInteger bintMod(BigInteger, BigInteger);            /* a % b */

/* Логические операции */
bool bintEqual(BigInteger a, BigInteger b);         /* true if a == b */
bool bintGreaterThan(BigInteger a, BigInteger b);   /* true if a > b */
bool bintLesserThan(BigInteger a, BigInteger b);    /* true if a < b */
bool isZero(BigInteger);

BigInteger bintCpy(BigInteger);
void freeBint(BigInteger);
#endif
